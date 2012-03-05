#ifndef SUBSCRIBER_MANAGER_H
#define SUBSCRIBER_MANAGER_H

#include <string>
#include <map>
#include <vector>
#include <boost/thread/mutex.hpp>
#include <boost/function.hpp>
#include "../protocol/mdf_message.h"


namespace wmdf
{
  class Subscriber;
  class SubscriberGroup;
  class FeedServer;

  class SubscriberManager
  {
  public:
    SubscriberManager(FeedServer* owner);
    virtual ~SubscriberManager(void);
    void Clear();
    void RegisterSubscriber(int32_t socket_id,int32_t user_id,std::string& ip_address);
    void ProcessLoginResponse( SharedMdfMessage resp_msg);
    void RemoveSubscriber(int32_t socket_id);
    std::string GetIpAddress(int32_t socket_id);
    Subscriber* FindSubscriber(int32_t socket_id);
    void PublishMessage(SharedMdfMessage message);
    void PublishMessage(SharedRelayMdfMessage message);
    void for_each_subscriber(boost::function<void(Subscriber*)> handle);  
    int32_t subscriber_size(){return (int32_t)subscribers_.size();}
    int32_t subscriber_group_count(){return (int32_t)subscriber_groups_.size();}
    SubscriberGroup* subscriber_groups(int32_t index){return subscriber_groups_[index];}
  private:
    SubscriberGroup* FindSubscriberGroup(int64_t protocol_unique_id,int16_t message_id);
  private:
    FeedServer* owner_;
    //groupid in windnet::connmanager;
    std::vector<SubscriberGroup*> subscriber_groups_;
    //socketid
    std::map<int32_t,Subscriber*> subscribers_;
    boost::mutex mutex_;
  };
}

#endif
