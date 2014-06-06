/*
ScrollingMsg.hpp
John O'Neil
Tuesday, June 3rd 2014

Scrolling msg display superimposed on video stream.

Controller allows multiple messages to be managed.


*/
#ifndef __MSG_SCROLL_HPP__
#define __MSG_SCROLL_HPP__

#include <string>
#include <map>
#include <cairo.h>


class ScrollingMsg
{
public:
  ScrollingMsg();
  ScrollingMsg(const int width, 
    const int height,
    const std::string& font, 
    const std::string& friendly_name, 
    const int& loop, 
    const std::string& msg, 
    const double& scroll_time, 
    const int& y_pos);

public:
  int CurrentLoop(void)const{return m_current_loop;};

public:
  void Resize(const int width, const int height);
  void Update(const float dt);;
  void Draw(cairo_t* context, const float dt);

private:
  int m_current_w, m_current_h;
  std::string m_friendly_name;
  std::string m_msg;
  int m_loops;
  int m_current_loop;
  std::string m_fontfamily;
  int m_ypos;
  int m_xpos;
  int m_scroll_time;
};

class ScrollingMsgController
{
public:
  ScrollingMsgController();

public:
  void AddMsg(const int width, 
    const int height, 
    const std::string& font, 
    const std::string& friendly_name, 
    const int& loop, 
    const std::string& msg, 
    const double& scroll_time, 
    const int& y_pos);
  void RemoveMsg(const std::string& friendly_name);

public:
  void Update(float dt);
  void Draw(cairo_t* context, const float dt);
  void Resize(const int width, const int height);

private:
  std::map< std::string, ScrollingMsg > m_msgs;
};

#endif