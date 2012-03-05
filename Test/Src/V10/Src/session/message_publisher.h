#ifndef MESSAGE_PUBLISHER_H
#define MESSAGE_PUBLISHER_H

#include "feed_server.h"
#include "subscriber_manager.h"

namespace wmdf
{

  //用于发布结构体的mdf message
  class MessagePublisher
  {
  public:
    MessagePublisher();
    virtual ~MessagePublisher(void);

    //发布数据产品的消息
    static void PublishOneMessage(SharedMdfMessage message);
    static void PublishOneMessage(SharedRelayMdfMessage message);

    //发布监控产品的消息
    static void PublishMonitorMessage(SharedMdfMessage message);

  };


}

#endif
