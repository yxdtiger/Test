#ifndef FEED_SERVER_
#define FEED_SERVER_

#include "server.h"
#include "net_module.h"
#include "../protocol/system_header_codec.h"
#include "../include/wmdf_structs.h"
#include <boost/thread.hpp>

namespace wmdf
{
  class SubscriberManager;
  class SystemMessageProcessorCenter;

  class FeedServer
  {
  public:
    FeedServer(void);
    virtual ~FeedServer(void);
    void Start(int32_t server_port);
    void Stop();
    void NotifyLogout();
    void SendMsg(int32_t socket_id,Message* msg);
    void BroadcastMsg(int32_t group_id,Message* msg);
    int32_t CreateConnectionGroup();
    void AddConnection(int32_t group_id,int32_t socket_id);
    Connection* GetConnection(int32_t socket_id);
    //仅仅在Server中关闭连接
    void CloseConnection(int32_t socket_id);
    void KickSubscriber(int32_t socket_id);
    int32_t GetConnectionCount(int group_id);
    bool is_server_running() {return is_server_running_;}
    SubscriberManager* subscriber_manager() {return subscriber_manager_;}
    SystemMessageProcessorCenter* system_message_processor_center() {return system_message_processor_center_;}
  protected:
    void SendHeartbeat();
  private:
    Server* feed_server_;
    SubscriberManager* subscriber_manager_;
    SystemMessageProcessorCenter* system_message_processor_center_;
    bool is_server_running_;
    boost::thread* heartbeat_thread_;

  };
}


#endif
