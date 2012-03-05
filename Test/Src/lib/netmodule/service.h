#ifndef WIND_NET_SERVICE_
#define WIND_NET_SERVICE_

#include "net_module.h"

namespace wmdf{
NET_API class Service
{
public:
  virtual void OnRecvMsg(int socket_id, Message* msg)=0;
  virtual void OnSendMsg(int socket_id, Message* msg)=0;
  virtual void OnClose(int socket_id, EventMsg* event_msg)=0;
  virtual void OnConnected(int socket_id, EventMsg* event_msg)=0;
  //注意：OnClose和OnConnected为两种特殊的OnEvent
  virtual void OnEvent(int socket_id, EventMsg* event_msg)=0;
};

}//end namespace
#endif

