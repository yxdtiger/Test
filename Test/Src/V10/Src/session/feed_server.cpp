#include "feed_server.h"
#include <iostream>
#include "system_message_processor.h"
#include "../include/wmdf_structs.h"
#include "../environment/splitter.h"
#include "subscriber.h"
#include "subscriber_manager.h"
#include "system_message_processor_center.h"
#include "system_message_processor_factory.h"
#include "../environment/system_config.h"
#include "../environment/server_environment.h"
#include "../protocol/system_header_codec.h"
#include "../protocol/protocol_description.h"
#include "../protocol/protocol_processor.h"
#include "../protocol/protocol_processor_factory.h"
#include "../protocol/mdf_message.h"
#include "../include/date_time.h"
#include "../monitor/logger.h"
#include "../environment/utils.h"

using namespace wmdf;

namespace
{
  void OnConnectedCallback(EventCallbackArgs& args)
  {
    Splitter splitter(args.event_msg->Context,":");
    //the first string is ip address;
    if (splitter.size() >= 1)
    {
      Logger::instance()->WriteFormatEventLog(LL_INFO,MEI_CLIENT_CONNECTED,"feed server","client connected:%s,socket id :%d",splitter[0].c_str(),args.socket_id);
      if (NULL != args.state)
      {
        //用户ID还不知道暂时赋值为-1，受到Longin消息后再进行更新
        ((FeedServer*)args.state)->subscriber_manager()->RegisterSubscriber(args.socket_id,-1,splitter[0]);
      }
    }
    ReleaseEventMsg(args.event_msg);
  }

  void OnCloseCallback(EventCallbackArgs& args)
  {
    Splitter splitter(args.event_msg->Context,":");
    //the first string is ip address;
    if (splitter.size() >= 1)
    {
      Logger::instance()->WriteFormatEventLog(LL_WARN,MEI_CLIENT_DISCONNECTED,"feed server","client disconnected:%s,socket id :%d",splitter[0].c_str(),args.socket_id);
    }
    if (NULL != args.state)
    {
      //release subscriber
      ((FeedServer*)args.state)->subscriber_manager()->RemoveSubscriber(args.socket_id);    
    }
    ReleaseEventMsg(args.event_msg);
  }

  void OnRecvCallback(MsgCallbackArgs& args)
  {
    if ( NULL != args.state )
    {
      FeedServer* fs = (FeedServer*)args.state; 
      int32_t length=0;
      SystemHeaderCodec system_header_decoder;
      SystemMessageHeader* sys_header = (SystemMessageHeader*)system_header_decoder.DecodeData(args.msg->data,length);
      wmdf::ProtocolDescription* pd = new wmdf::ProtocolDescription(PRODEUCT_SYSTEM_ID,sys_header->major_version,sys_header->minor_version,CT_BINARY);
      wmdf::ProtocolProcessor* processor = ProtocolProcessorFactory::CreateProtocolCodec(pd);
      if (processor != NULL)
      {
        SharedMdfMessage req_msg = SharedMdfMessage(processor->Decode(args.msg));
        req_msg->set_socket_id(args.socket_id);
        SystemMessageProcessor* system_message_processor = fs->system_message_processor_center()->GetSystemMessageProcessor(pd->unique_id());
        if ((NULL != req_msg) && (NULL != system_message_processor))
        {          
          system_message_processor->ProcessMessage(req_msg,fs,processor);
        }
        delete processor;
      }
			delete sys_header;
    }
    ReleaseMessage(args.msg);
  }

  void OnEventCallback(EventCallbackArgs& args)
  {
    // add your code here
    FeedServer* fs = (FeedServer*)args.state;
    Subscriber* subscriber = fs->subscriber_manager()->FindSubscriber(args.socket_id);
    if (NULL != subscriber)
    {
      Logger::instance()->WriteFormatEventLog(LL_INFO,MEI_NOMAL_EVENT,"feed server","socket id:%d,user id:%d,context:%s",args.socket_id,subscriber->user_id(),args.event_msg->Context);
    }
    ReleaseEventMsg(args.event_msg);
  }  
}

wmdf::FeedServer::FeedServer(void)
{
  is_server_running_ = false;
  system_message_processor_center_ = new SystemMessageProcessorCenter();
  subscriber_manager_ = new SubscriberManager(this);
}

wmdf::FeedServer::~FeedServer(void)
{
  if (feed_server_ != NULL)
  {
    delete feed_server_;
    feed_server_ = NULL;
  }
 
  if (subscriber_manager_ != NULL)
  {
    delete subscriber_manager_;
    subscriber_manager_ = NULL;
  }
  if (system_message_processor_center_ != NULL)
  {
    delete system_message_processor_center_;
    system_message_processor_center_ = NULL;
  }
}

void wmdf::FeedServer::SendMsg( int socket_id,Message* msg )
{
  feed_server_->SendMsg(socket_id,msg);
}

void wmdf::FeedServer::BroadcastMsg( int group_id,Message* msg )
{
  feed_server_->BroadcastMsg(group_id,msg);
}

void wmdf::FeedServer::Start(int32_t server_port)
{
  if (!is_server_running_)
  {
    SystemMessageProcessorFactory::InitSysMessageProcessorCenter(system_message_processor_center_);
    SystemConfig* sc = ServerEnvironment::system_config();
    feed_server_ = new Server("0.0.0.0",
      server_port,
      sc->server_thread_num(),
      sc->max_connection_count(),
      sc->max_packet_size(),
      sc->max_cache_msg_count());
    feed_server_->SetConnectedCallback(&OnConnectedCallback);
    feed_server_->SetEventCallback(&OnEventCallback);
    feed_server_->SetCloseCallback(&OnCloseCallback);
    feed_server_->SetRecvCallback(&OnRecvCallback);
    feed_server_->SetState(this);
    int32_t result = feed_server_->Start();
    is_server_running_ = (result == 1);
    heartbeat_thread_ = new boost::thread(boost::bind(&FeedServer::SendHeartbeat,this));
  }
}

void wmdf::FeedServer::Stop()
{
  if (feed_server_ != NULL)
  {
    is_server_running_ = false;
    feed_server_->Stop();
    delete feed_server_;
    feed_server_ = NULL;
    subscriber_manager_->Clear();
  }
}

int32_t wmdf::FeedServer::CreateConnectionGroup()
{
  return feed_server_->GetConnManager()->AddConnectionGroup();
  
}

void wmdf::FeedServer::AddConnection( int32_t group_id,int32_t socket_id )
{
  feed_server_->GetConnManager()->AddConnection(group_id,socket_id);
}

void wmdf::FeedServer::CloseConnection( int32_t socket_id )
{
  feed_server_->CloseConnection(socket_id);
}

int32_t wmdf::FeedServer::GetConnectionCount( int group_id )
{
  return feed_server_->GetConnManager()->GetConnectionCount(group_id);
}

void wmdf::FeedServer::NotifyLogout()
{
  Logout* logout = new Logout();
  logout->logout_type = LT_SERVERNOTIFY;
  MdfMessage* mdf_message =new MdfMessage();
  mdf_message->set_local_time(DateTime::get_current_time());
  mdf_message->set_socket_id(0);
  mdf_message->set_product_id(PRODEUCT_SYSTEM_ID);
  mdf_message->set_message_id(SYSTEM_LOGOUT_ID);
  mdf_message->set_major_version(MAJOR_VERSION_SYSTEM_ID);
  mdf_message->set_minor_version(MINOR_VERSION_SYSTEM_ID);
  mdf_message->set_record_count(1);
  mdf_message->set_body(logout);
  SharedMdfMessage shared_msg=SharedMdfMessage(mdf_message);
  subscriber_manager_->PublishMessage(shared_msg);
}

Connection* wmdf::FeedServer::GetConnection( int32_t socket_id )
{
  return feed_server_->GetConnManager()->GetConnection(socket_id);
}

void wmdf::FeedServer::KickSubscriber( int32_t socket_id )
{
  subscriber_manager_->RemoveSubscriber(socket_id);
  feed_server_->CloseConnection(socket_id);
}

void wmdf::FeedServer::SendHeartbeat()
{
  while (is_server_running_)
  {
    HeartBeat* heartbeat = new HeartBeat();
    heartbeat->place_holder = 1;
    MdfMessage* mdf_message =new MdfMessage();
    mdf_message->set_local_time(DateTime::get_current_time());
    mdf_message->set_socket_id(0);
    mdf_message->set_product_id(PRODEUCT_SYSTEM_ID);
    mdf_message->set_message_id(SYSTEM_HEARTBEAT_ID);
    mdf_message->set_major_version(MAJOR_VERSION_SYSTEM_ID);
    mdf_message->set_minor_version(MINOR_VERSION_SYSTEM_ID);
    mdf_message->set_record_count(1);
    mdf_message->set_body(heartbeat); 
    SharedMdfMessage shared_msg=SharedMdfMessage(mdf_message);
    subscriber_manager_->PublishMessage(shared_msg);
    Utils::SleepTime(ServerEnvironment::system_config()->heart_beat_interval());
  }
}