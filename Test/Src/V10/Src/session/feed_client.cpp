#include <iostream>
#include "feed_client.h"

#include "../include/date_time.h"
#include "../environment/utils.h"
#include "../product/product_source.h"
#include "../monitor/logger.h"

using namespace std;
using namespace wmdf;

/**********************************************************/
/*********************IClientCallback   *******************/
/**********************************************************/
wmdf::IClientCallback::IClientCallback()
{

}

wmdf::IClientCallback::~IClientCallback()
{

}

void OnCloseCallback(EventCallbackArgs& args)
{
  assert(args.state != NULL);
  Logger::instance()->WriteLog(LL_INFO,args.event_msg->Context);
  FeedClient* client = (FeedClient*)args.state;
  client->DoCloseCallback(args.event_msg);
  ReleaseEventMsg(args.event_msg);
}
void OnEventCallback(EventCallbackArgs& args)
{
  assert(args.state != NULL);
  Logger::instance()->WriteLog(LL_INFO,args.event_msg->Context);
  FeedClient* client = (FeedClient*)args.state;
  client->DoEventCallback(args.event_msg);
  ReleaseEventMsg(args.event_msg);
}

void OnRecvCallback(MsgCallbackArgs& args)
{
  assert(args.state != NULL);
  FeedClient* client = (FeedClient*)args.state;
  client->DoRecvCallback(args.msg);
  ReleaseMessage(args.msg);
}


/**********************************************************/
/*********************FeedClient***************************/
/**********************************************************/
FeedClient::FeedClient(int32_t client_type)
{
  is_connected_ = false;
  client_type_ = client_type;
  max_packet_size_=512*1024;
  max_buf_msg_=50;
  thread_num_=1;
	client_ = new Client(max_packet_size_,max_buf_msg_,thread_num_);
	client_->SetCloseCallback(&OnCloseCallback);
	client_->SetEventCallback(&OnEventCallback);
	client_->SetRecvCallback(&OnRecvCallback);
	client_->SetState(this);
	client_->Init(client_type_);
}

FeedClient::~FeedClient()
{
  DisConnect();
}

bool FeedClient::Connect(const string& server_address, const int& server_port,const int32_t retry_times, int32_t timeout)
{
  server_address_ = server_address;
  server_port_ = server_port;

  is_connected_ = false;
  //ÖØÁ¬
  int32_t connect_times = 1;
  while ((!is_connected_) && (connect_times <= retry_times))
  {
    int ret = client_->Connect(server_address.c_str(), server_port, timeout);
    is_connected_ = (ret != 0);
    if (!is_connected_)
    {
      Logger::instance()->WriteFormatEventLog(LL_INFO,MEI_SOURCE_NOTAVAILABLE,"feed client","Connection failed (%s). Delaying for retry #%d ",server_address.c_str(),connect_times);
      if(connect_times != retry_times)
        Utils::SleepTime(3000);
    }
    connect_times++;
  }
  return is_connected_;
}

void wmdf::FeedClient::SendMsg( Message* msg )
{
  client_->SendMsg(msg);
}

void wmdf::FeedClient::DisConnect()
{
  if ((NULL != client_) && (is_connected_))
  {
    client_->Close();
  }
  is_connected_= false;
}

void wmdf::FeedClient::DoRecvCallback( Message* msg )
{
  if (NULL != client_callback_)
  {
    client_callback_->OnRecvMsg(msg);
  }
}

void wmdf::FeedClient::DoCloseCallback( EventMsg* msg )
{
  if (NULL != client_callback_)
  {
    client_callback_->OnCloseCallback(msg);
  }
  is_connected_ = false;    
}

void wmdf::FeedClient::DoEventCallback( EventMsg* msg )
{
  if (NULL != client_callback_)
  {
    client_callback_->OnEventCallback(msg);
  }
}

void wmdf::FeedClient::SetClientCallback( IClientCallback* callback )
{
  client_callback_ = callback;
}






