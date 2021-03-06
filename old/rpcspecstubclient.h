/**
 * THIS FILE IS GENERATED BY jsonrpcstub, DO NOT CHANGE IT!!!!!
 */

#ifndef _RPCSPECSTUBCLIENT_H_
#define _RPCSPECSTUBCLIENT_H_

#include <jsonrpc/rpc.h>

class RPCSpecStubClient
{
    public:
        RPCSpecStubClient(jsonrpc::AbstractClientConnector* conn)
        {
            this->client = new jsonrpc::Client(conn);
        }
        ~RPCSpecStubClient()
        {
            delete this->client;
        }

        std::string ShowMessage(const std::string& friendlyName, const std::string& msg, const int& x, const int& y) throw (jsonrpc::JsonRpcException)
        {
            Json::Value p;
            p["friendlyName"] = friendlyName; 
p["msg"] = msg; 
p["x"] = x; 
p["y"] = y; 

            Json::Value result = this->client->CallMethod("ShowMessage",p);
    if (result.isString())
        return result.asString();
     else 
         throw jsonrpc::JsonRpcException(jsonrpc::Errors::ERROR_CLIENT_INVALID_RESPONSE, result.toStyledString());

        }

    private:
        jsonrpc::Client* client;
};
#endif //_RPCSPECSTUBCLIENT_H_
