#include "relay_adapter.h"
#include "../../environment/utils.h"
#include "../product_source.h"
#include "../../session/message_publisher.h"
#include "../../include/wmdf_structs.h"
#include "../../protocol/protocol_processor.h"
#include "../../environment/server_environment.h"
#include "../../monitor/monitor_state.h"
#include "../../session/feed_client.h"
#include "../../protocol/protocol_description.h"
#include "../../monitor/productdata_logger.h"
#include "../../monitor/logger.h"
#include "../../session/login_proxy.h"
#include "../../protocol/protocol_processor_controller.h"
#include "../../protocol/protocol_processor_factory.h"
#include "../../include/date_time.h"

using namespace std;

wmdf::RelayAdapter::RelayAdapter(ProductSource* product_source)
:IProductAdapter(product_source)
{
  login_proxy_ = new LoginProxy();
  protocol_processor_controller_ = new ProtocolProcessorController();
  last_heart_beat_time_ = 0;
  data_receiver_ = new FeedClient(MDF_CLIENT);
  data_receiver_->SetClientCallback(this);
}

wmdf::RelayAdapter::~RelayAdapter(void)
{
  if (is_working_)
  {
    Stop();
  }
  if (NULL != data_receiver_)
  {
    delete data_receiver_;
    data_receiver_ = NULL;
  }
  if (NULL != login_proxy_)
  {
    delete login_proxy_;
    login_proxy_ = NULL;
  }
  if (NULL != protocol_processor_controller_)
  {
    delete protocol_processor_controller_;
    protocol_processor_controller_ = NULL;
  }
}

void wmdf::RelayAdapter::Start()
{
  if (!is_working_)
  {
		is_working_ = true;

    product_source_->set_which_is_in_use(0);
    if (!Connect(product_source_->ip_address(),product_source_->port(),
      product_source_->retry_times(),product_source_->timeout(),
      product_source_->user_name(),product_source_->pass_word()))
    {
      //切换到备源
      for (int32_t index =0; index < product_source_->backup_source_count();index++)
      {
        product_source_->set_which_is_in_use(index+1);
        if (Connect(product_source_->backup_sources(index)->ip_address(),
                    product_source_->backup_sources(index)->port(),
                    product_source_->retry_times(),product_source_->timeout(),
                    product_source_->backup_sources(index)->user_name(),
                    product_source_->backup_sources(index)->pass_word()))
        {
          break;
        }
      }
    }
  }
}

bool wmdf::RelayAdapter::Connect( const std::string& server_address, const int32_t& server_port, 
                                 const int32_t retry_times,int32_t timeout,std::string username,std::string password )
{
  if (data_receiver_->Connect(server_address,server_port,retry_times,timeout))
   {
     if (!login_proxy_->RequestLogin(product_source_,(FeedClient*)data_receiver_,username,password))
     {
       Logger::instance()->WriteFormatEventLog(LL_WARN,MEI_AUTHORITY_NONCONFORM,"relay adapter", 
         "Login failed, user:%s, server ip:%s, port:%d",username.c_str(),server_address.c_str(),server_port);
       data_receiver_->DisConnect();

       return false;
     }
     else
     {
       Logger::instance()->WriteFormatEventLog(LL_INFO,MEI_SOURCE_CONNECTED,"relay adapter", 
         "Connection successfully established, ip address:%s, port:%d,product id:%d",
         server_address.c_str(),server_port,product_source_->product_id());
       return true;
     }
   }
  else
  {
    Logger::instance()->WriteFormatEventLog(LL_ERROR,MEI_SOURCE_DISCONNECTED,"relay adapter", 
      "Unable to connect to server, ip address:%s, port:%d,product id:%d",
      server_address.c_str(),server_port,product_source_->product_id());
  }
  return false;
}

void wmdf::RelayAdapter::Stop()
{
  if (is_working_)
  {
    is_working_ = false;
    data_receiver_->DisConnect();
    receive_size_  =0;
    receive_packet_count_=0;
    last_heart_beat_time_ = 0;
    product_source_->set_which_is_in_use(0);
  }
}

void wmdf::RelayAdapter::ProcessLoginResponse( Message* msg )
{
  MdfMessage* mdf_message = login_proxy_->ResponseLogin(msg);
  if (mdf_message != NULL)
  {
    LoginResponse* login_resp = (LoginResponse*)mdf_message->body();
    if ( login_resp->ack_type!=AUTHORITY_VALID ) return;
    for (int32_t index=0; index< login_resp->product_count;index++)
    {
      if (login_resp->product_status[index].product_status == AUTHORITY_VALID)
      {
        ProtocolDescription* pd = new ProtocolDescription(
          login_resp->product_status[index].product_id,
          login_resp->product_status[index].major_version,
          login_resp->product_status[index].minor_version,
          (CodecType)login_resp->product_status[index].codec);
        logger_->RegisterProduct(pd);

        ProtocolDescription* pd1 = new ProtocolDescription(
          login_resp->product_status[index].product_id,
          login_resp->product_status[index].major_version,
          login_resp->product_status[index].minor_version,
          (CodecType)login_resp->product_status[index].codec);
        ProtocolProcessor* protocol_processor = ProtocolProcessorFactory::CreateProtocolCodec(pd1);
        protocol_processor_controller_->AddProtocolProcessor(protocol_processor);
      }
    }
  }  
}

void wmdf::RelayAdapter::OnRecvMsg( Message* msg )
{
  //the first message is login response, process login response
  if (msg->product_id == PRODEUCT_SYSTEM_ID) 
  {
    if (msg->message_id==SYSTEM_LOGIN_RESPONSE_ID)
    {
      ProcessLoginResponse(msg);
      return;
    }
    else if (msg->message_id == SYSTEM_HEARTBEAT_ID)
    {
//       Message* relay_msg = CreateMessage(msg->msg_len-MSG_HEADER_LEN); 
//       memcpy(relay_msg,msg,msg->msg_len);
//       this->data_receiver_->SendMsg(relay_msg);
      last_heart_beat_time_ = DateTime::get_current_time();
      return;
    }
  }
  last_heart_beat_time_ = DateTime::get_current_time();

  //记录接收到的数据大小
  receive_size_  += msg->msg_len;
  receive_packet_count_+=1;

  //日志记录
  if (ServerEnvironment::mointor_state()->logging())
  {
    uint8_t* data= new uint8_t[msg->msg_len];
    memcpy(data,msg,msg->msg_len);
    ProductRawData* raw_data = new ProductRawData(msg->product_id,data,msg->msg_len);
    logger_->AddToBuffer(raw_data);
  }
  //decode msg
  MdfMessage* mdf_message = protocol_processor_controller_->Decode(msg);
  if (NULL != mdf_message) 
  {
    //根据不同的订阅进行编码并发布。
    PublishMdfMessage(mdf_message);
  }
  
}

void wmdf::RelayAdapter::OnCloseCallback( EventMsg* args )
{
  is_working_ = false;
  Logger::instance()->WriteFormatEventLog(LL_WARN,MEI_SOURCE_DISCONNECTED,"relay adapter", 
    "Connection was closed, ip address:%s, port:%d,product id:%d",
    product_source_->ip_address().c_str(),product_source_->port(),product_source_->product_id());
}

void wmdf::RelayAdapter::OnEventCallback( EventMsg* args )
{
  Logger::instance()->WriteEventLog(LL_INFO,MEI_NOMAL_EVENT,
    "relay adapter", args->Context );
}

bool wmdf::RelayAdapter::is_connected()
{
  if ( NULL != data_receiver_ )
  {
    return data_receiver_->is_connected();
  }
  return false;
}

