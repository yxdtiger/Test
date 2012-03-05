#ifndef WIND_NET_MESSAGE_FACTORY_
#define WIND_NET_MESSAGE_FACTORY_


#include "net_module.h"
#include <memory>

#define LTIME(h,m,s,ms)			   ((h)*3600000+(m)*60000+(s)*1000+(ms))

namespace wmdf{

NET_API class MessageFactory
{
public:
  static const uint8_t MSG_START_TAG=1;

  NET_API static Message* CreateMessage(size_t size);

  NET_API static Message* CreateMessage(size_t size,uint8_t* data);

  NET_API static void DestoryMessage(Message* msg);

  static Message* CreateRawMessage(size_t size);

	static uint8_t CheckSum(uint8_t* data,int size);

  static uint32_t  GetTimestamp();
private:
  MessageFactory(){}
  MessageFactory(const MessageFactory&msg){}
};

} // namespace

#endif

