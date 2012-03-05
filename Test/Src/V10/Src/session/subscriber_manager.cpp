#include "subscriber_manager.h"
#include "../include/wmdf_structs.h"
#include "../include/date_time.h"
#include "../monitor/logger.h"
#include "subscriber.h"
#include "subscriber_group.h"
#include "../include/wmdf_api.h"
#include "../protocol/protocol_processor_factory.h"
#include "feed_server.h"
#include <algorithm>

wmdf::SubscriberManager::SubscriberManager(FeedServer* owner)
{
  owner_ = owner;
}

wmdf::SubscriberManager::~SubscriberManager(void)
{
  Clear();
  owner_ = NULL;
}

void wmdf::SubscriberManager::RemoveSubscriber( int32_t socket_id )
{
  boost::mutex::scoped_lock lock(mutex_);
	
  std::map<int32_t, Subscriber*>::iterator iter = subscribers_.find(socket_id);
  if (iter != subscribers_.end())
  {
    delete iter->second;
    iter->second = NULL;
    subscribers_.erase(iter);
  }

}

void wmdf::SubscriberManager::ProcessLoginResponse( SharedMdfMessage resp_msg )
{
  boost::mutex::scoped_lock lock(mutex_);
  if (resp_msg->message_id() != SYSTEM_LOGIN_RESPONSE_ID) { return; }
  LoginResponse* response = (LoginResponse*)resp_msg->body();
  //收到登录消息时Subscriber应该已经注册
  Subscriber* subscriber = FindSubscriber(resp_msg->socket_id());
  if (subscriber == NULL)
  {
    Logger::instance()->WriteFormatEventLog(LL_ERROR,MEI_CLIENT_NOTEXIST,"subscriber manager",
      "connection has not existed  : %s,%d \n",subscriber->ip_address().c_str(),response->user_id);  
    return;
  }

  subscriber->set_user_id(response->user_id);
  //如果授权无效或未订阅产品则踢掉该用户
  if(response->ack_type != AUTHORITY_VALID) 
  {
    //这个时候还在callback里 不能踢掉，先置一个状态，在守护线程中去踢
    subscriber->set_to_be_kicked(true);
    return;
  }

  
  for(int i=0;i<response->product_count;i++)
  {
    //产品无效授权则跳到下一个
    if (response->product_status[i].product_status == AUTHORITY_INVALID)
    {
      continue;
    }

    //产品有效注册订阅及分组
    ProtocolDescription* pd = new ProtocolDescription(response->product_status[i].product_id,
                                                      response->product_status[i].major_version,
                                                      response->product_status[i].minor_version,
                                                      (CodecType)response->product_status[i].codec);
    subscriber->AddSubscribedProduct(pd);
    int64_t protocol_unique_id = pd->unique_id();
    
    for (int j=0;j<response->product_status[i].message_count;j++)
    {
      int16_t message_id = response->product_status[i].message_info[j].message_id;
      SubscriberGroup* group = FindSubscriberGroup(protocol_unique_id,message_id);
      if (group != NULL)
      {
        owner_->AddConnection(group->group_id(),resp_msg->socket_id());
      }
      else
      {
        HeaderCodec* header_codec = ProtocolProcessorFactory::CreateHeaderCodec(protocol_unique_id);
        MessageCodec* message_codec = ProtocolProcessorFactory::CreateMessageCodec(protocol_unique_id,message_id);
        if ((NULL != header_codec) && (NULL != message_codec))
        {
          //if group not exist then create a group 
          int32_t group_id = owner_->CreateConnectionGroup();
          owner_->AddConnection(group_id,resp_msg->socket_id());
          SubscriberGroup* group = new SubscriberGroup();
          group->set_header_codec(header_codec);
          group->set_message_codec(message_codec);
          group->set_protocol_unique_id(protocol_unique_id);
          group->set_group_id(group_id);
          group->set_product_id(response->product_status[i].product_id);
          group->set_message_id(message_id);
          subscriber_groups_.push_back(group);
        }
      }
    }
  }    
}

wmdf::Subscriber* wmdf::SubscriberManager::FindSubscriber(int32_t socket_id)
{
  std::map<int32_t, Subscriber*>::iterator iter = subscribers_.find(socket_id);
  if (iter != subscribers_.end())
  {
    return iter->second;
  }
  return NULL;
}

wmdf::SubscriberGroup* wmdf::SubscriberManager::FindSubscriberGroup( int64_t protocol_unique_id,int16_t message_id )
{
  for(uint32_t i=0;i<subscriber_groups_.size();i++)
  {
    if ((subscriber_groups_[i]->protocol_unique_id() == protocol_unique_id) 
      &&(subscriber_groups_[i]->message_id() == message_id))
    {
      return subscriber_groups_[i];
    }
  }
  return NULL;
}

void wmdf::SubscriberManager::PublishMessage( SharedMdfMessage message )
{
  boost::mutex::scoped_lock lock(mutex_);
  for(uint32_t i=0;i<subscriber_groups_.size();i++)
  {
    if ((subscriber_groups_[i]->product_id() == message->product_id())
      && (subscriber_groups_[i]->message_id() == message->message_id()) )
    {
      subscriber_groups_[i]->PublishMessage(message,owner_);
    }
  }
}

void wmdf::SubscriberManager::PublishMessage( SharedRelayMdfMessage message )
{
  boost::mutex::scoped_lock lock(mutex_);
  for(uint32_t i=0;i<subscriber_groups_.size();i++)
  {
    if ((subscriber_groups_[i]->product_id() == message->product_id) 
      && (subscriber_groups_[i]->message_id() == message->message_id)) 
    {
      subscriber_groups_[i]->PublishMessage(message,owner_);
    }
  }
}

void wmdf::SubscriberManager::Clear()
{
  boost::mutex::scoped_lock lock(mutex_);
  for(uint32_t i=0;i<subscriber_groups_.size();i++)
  {
    delete subscriber_groups_[i];
  }
  subscriber_groups_.clear();


  std::map<int32_t,Subscriber*>::iterator iter_sub=subscribers_.begin();
  while (iter_sub != subscribers_.end())
  {
    if(NULL != iter_sub->second)
    {
      delete iter_sub->second;
      iter_sub->second = NULL;
    }
    iter_sub++;
  }
  subscribers_.clear();
}

void wmdf::SubscriberManager::RegisterSubscriber( int32_t socket_id,int32_t user_id,std::string& ip_address )
{
  Subscriber* subscriber = FindSubscriber(socket_id);
  if (subscriber == NULL)
  {
    subscriber = new Subscriber(socket_id,user_id,ip_address,DateTime::get_current_time());
    subscriber->set_connection(owner_->GetConnection(socket_id));
    subscribers_.insert(make_pair(subscriber->socket_id(),subscriber));
  }
}

std::string wmdf::SubscriberManager::GetIpAddress( int32_t socket_id )
{
  Subscriber* subscriber = FindSubscriber(socket_id);
  if (NULL != subscriber )
  {  
    return subscriber->ip_address();
  }
  return "";
}

void wmdf::SubscriberManager::for_each_subscriber( boost::function<void(Subscriber*)> handle )
{
  boost::mutex::scoped_lock lock(mutex_);
  std::map<int32_t,Subscriber*>::iterator iter = subscribers_.begin();
  while (iter != subscribers_.end())
  {
    handle(iter->second);
    iter++;
  }
}