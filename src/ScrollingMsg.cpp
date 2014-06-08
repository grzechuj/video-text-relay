#include "ScrollingMsg.hpp"
#include <pango/pangocairo.h>
#include "utilities.hpp"
#include <iostream>
using std::cout;
using std::endl;

ScrollingMsg::ScrollingMsg()
  :m_current_w(0)
  ,m_current_h(0)
  ,m_friendly_name("None")
  ,m_msg("None")
  ,m_loops(0)
  ,m_current_loop(0)
  ,m_fontfamily("Sans Bold 12")
  ,m_ypos(300)
  ,m_xpos(0)
  ,m_scroll_time(12.0f)
  ,m_dropshadow(false)
  ,m_underlay(false)
{

};
ScrollingMsg::ScrollingMsg( const int width, 
                            const int height,
                            const std::string& font, 
                            const std::string& friendly_name, 
                            const int& loop, 
                            const std::string& msg, 
                            const double& scroll_time,
                            const int& y_pos,
                            const bool dropshadow,
                            const bool underlay)
  :m_current_w(width)
  ,m_current_h(height)
  ,m_friendly_name(friendly_name)
  ,m_msg(msg)
  ,m_loops(loop)
  ,m_current_loop(0)
  ,m_fontfamily(font)
  ,m_ypos(y_pos)
  ,m_xpos(width)
  ,m_scroll_time(scroll_time)
  ,m_dropshadow(dropshadow)
  ,m_underlay(underlay)
{

};

void ScrollingMsg::Resize(const int width, const int height) {
  m_current_w = width;
  m_current_h = height;
};
void ScrollingMsg::Update(const float dt)
{
  //TODO: separate update and drawing of msg to facilitate different ordering.
};

void ScrollingMsg::Draw(cairo_t* context, const float dt)
{
  cairo_save(context);

  PangoLayout *pango_layout;
  PangoFontDescription *pango_fontdesc;

  pango_layout = pango_cairo_create_layout(context);
  pango_fontdesc = pango_font_description_from_string(m_fontfamily.c_str());
  PangoAttrList* pTextAttributes = pango_attr_list_new();
  PangoAttrList* no_color_attributes = 0;
  gchar *text = 0;//stupidly gchar disallows deallocation, but it's not locked off in code.
  std::string displayed_text = m_msg;
  if(!pango_parse_markup(m_msg.c_str(),
                    -1,//null terminated text string above
                    0,//no accellerated marker
                    &pTextAttributes,
                    &text,
                    NULL,
                    NULL)) 
  {
    //Failure to parse markup, so displayed text is empty. Reset it to default text
    displayed_text = m_msg.c_str();
  }else{
    displayed_text = text;
  }

  no_color_attributes = utilities::remove_color_attributes(pTextAttributes);
  pango_layout_set_text(pango_layout, displayed_text.c_str(), -1);
  pango_layout_set_attributes (pango_layout, pTextAttributes);
  pango_layout_set_font_description(pango_layout, pango_fontdesc);


  PangoRectangle ink_rect, logical_rect;
  pango_layout_get_pixel_extents(pango_layout, &ink_rect, &logical_rect);

  //cout<<"text width "<<ink_rect.width<<" "<<logical_rect.width<<endl;

  //d_pos = d/t * dt
  m_xpos -= ((m_current_w + ink_rect.width)/m_scroll_time)*dt;
  if(m_xpos<(-1.0f*ink_rect.width)) {//wraparound
    m_xpos = m_current_w;
    m_current_loop += 1;
    if(m_current_loop==m_loops) {
      m_current_loop = -1;//indicates controller should remove this msg.
    }
  }

  //possibly draw simple dropshadow to make text readable
  if(m_dropshadow) {
    pango_layout_set_attributes(pango_layout, no_color_attributes);
    cairo_set_source_rgb (context, 0.0, 0.0, 0.0);
    cairo_move_to(context, m_xpos+2, m_ypos+2);
    pango_cairo_update_layout(context, pango_layout);
    pango_cairo_show_layout(context, pango_layout);
  }

  pango_layout_set_attributes (pango_layout, pTextAttributes);
  cairo_set_source_rgb (context, 1.0, 1.0, 1.0);
  cairo_move_to(context, m_xpos, m_ypos);
  pango_cairo_update_layout(context, pango_layout);
  pango_cairo_show_layout(context, pango_layout);

  //cleanup messy as fuck C api
  pango_attr_list_unref(pTextAttributes);
  pango_attr_list_unref(no_color_attributes);
  pango_font_description_free(pango_fontdesc);
  g_object_unref(pango_layout);
  cairo_restore(context);
}



ScrollingMsgController::ScrollingMsgController()
{

}

void ScrollingMsgController::AddMsg(const int width, 
  const int height, 
  const std::string& font, 
  const std::string& friendly_name, 
  const int& loop, 
  const std::string& msg, 
  const double& scroll_time, 
  const int& y_pos,
  const bool dropshadow,
  const bool underlay)
{
    m_msgs[friendly_name]=ScrollingMsg(width, height, font, friendly_name, loop, msg, scroll_time, y_pos, dropshadow, underlay);
}

void ScrollingMsgController::RemoveMsg(const std::string& friendly_name)
{
  std::map< std::string, ScrollingMsg >::iterator msg = m_msgs.find(friendly_name);
  if(msg!=m_msgs.end())
  {
    m_msgs.erase(msg);
  }
};

void ScrollingMsgController::Update(float dt) {
  for(std::map< std::string, ScrollingMsg >::iterator imsg=m_msgs.begin();
    imsg!=m_msgs.end();)
  {
    imsg->second.Update(dt);
    //remove those msgs that are 'done'
    if(imsg->second.CurrentLoop()<0) {
      imsg = m_msgs.erase(imsg);
    }else{
      ++imsg;
    }
  }
};
void ScrollingMsgController::Draw(cairo_t* context, const float dt) {
  for(std::map< std::string, ScrollingMsg >::iterator imsg=m_msgs.begin();
    imsg!=m_msgs.end(); ++imsg)
  {
    imsg->second.Draw(context, dt);
  }
}

void ScrollingMsgController::Resize(const int width, const int height) {
  for(std::map< std::string, ScrollingMsg >::iterator imsg=m_msgs.begin();
    imsg!=m_msgs.end(); ++imsg)
  {
    imsg->second.Resize(width, height);
  }
}
