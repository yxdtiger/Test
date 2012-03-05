#include "product_adapter.h"
#include "../../monitor/monitor_state.h"
#include "../../monitor/productdata_logger.h"
#include "../../environment/server_environment.h"
#include "../../environment/utils.h"
#include "../../monitor/logger.h"
#include "../../environment/strutils.h"
#include "../../protocol/protocol_description.h"

using namespace wmdf;

ProductAdapter::ProductAdapter(ProductSource* product_source)
:IProductAdapter(product_source)
{
  data_receiver_ = new FeedClient(RELAY_CLIENT);
  data_receiver_->SetClientCallback(this);
  ProtocolDescription* pd = new ProtocolDescription(
    product_source->product_id(),0,0,CT_FEEDHANDLER);
  logger_->RegisterProduct(pd);
  buffer_end_=0;
  memset(message_buffer_,0,sizeof(message_buffer_));
}

ProductAdapter::~ProductAdapter()
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
  product_source_ = NULL;
}


void ProductAdapter::Start()
{
  if (!is_working_)
  {
    is_working_=true;
    
    ResetContext();
    buffer_end_=0;
    memset(message_buffer_,0,sizeof(message_buffer_));
    product_source_->set_which_is_in_use(0);
    if (!Connect(product_source_->ip_address(),product_source_->port(),
      product_source_->retry_times(),product_source_->timeout()))
    {
      //切换到备源
      for (int32_t index =0; index < product_source_->backup_source_count();index++)
      {
        product_source_->set_which_is_in_use(index+1);
        if (Connect(product_source_->backup_sources(index)->ip_address(),
          product_source_->backup_sources(index)->port(),
          product_source_->retry_times(),product_source_->timeout()))
        {
          break;
        }
      }
    }
  }
}

bool wmdf::ProductAdapter::Connect(const std::string& server_address, const int32_t& server_port, const int32_t retry_times/*=3*/,int32_t timeout/*=0*/ )
{
  if (data_receiver_->Connect(server_address,server_port,retry_times,timeout ))
  {
    Logger::instance()->WriteFormatEventLog(LL_INFO,MEI_SOURCE_CONNECTED,"product adapter", 
      "Connection successfully established, ip address:%s, port:%d,product id:%d",
      server_address.c_str(),server_port,product_source_->product_id());
    return true;
  }
  else
  {
    Logger::instance()->WriteFormatEventLog(LL_ERROR,MEI_SOURCE_NOTAVAILABLE,"product adapter", 
      "Unable to connect to server, ip address:%s, port:%d,product id:%d",
      server_address.c_str(),server_port,product_source_->product_id());   
    return false;
  }
}

void ProductAdapter::Stop()
{
  if (is_working_)
  {
    is_working_=false;
    data_receiver_->DisConnect();
    receive_size_  =0;
    receive_packet_count_=0;
    last_heart_beat_time_ = 0;
    product_source_->set_which_is_in_use(0);
  }
}


void wmdf::ProductAdapter::OnRecvMsg( Message* msg )
{
  int32_t pack_length=msg->msg_len- MSG_HEADER_LEN;
  memcpy(message_buffer_+buffer_end_,msg->data,pack_length);
  buffer_end_+=pack_length;

  //记录接收到的数据大小
  receive_size_  += msg->msg_len;
  receive_packet_count_+=1;

  if (ServerEnvironment::mointor_state()->logging())
  {
    uint8_t* data= new uint8_t[msg->msg_len];
    memcpy(data,msg,msg->msg_len);
    ProductRawData* raw_data = new ProductRawData(product_source_->product_id(),data,msg->msg_len);
    logger_->AddToBuffer(raw_data);
  }
  //解码
  uint32_t end_pos = ParseMessage(buffer_end_);
  if(end_pos<buffer_end_)
  {
    memmove(message_buffer_,message_buffer_+end_pos,buffer_end_-end_pos);
  }
  buffer_end_-=end_pos;
}

void wmdf::ProductAdapter::OnCloseCallback( EventMsg* args )
{
  is_working_=false;
  Logger::instance()->WriteFormatEventLog(LL_WARN,MEI_SOURCE_DISCONNECTED,"product adapter", 
    "Connection disconnected, ip address:%s, port:%d,product id:%d",
    product_source_->ip_address().c_str(),product_source_->port(),product_source_->product_id());
}

void wmdf::ProductAdapter::OnEventCallback( EventMsg* args )
{
  Logger::instance()->WriteEventLog(LL_INFO,MEI_NOMAL_EVENT,
    "product adapter", args->Context );
}

bool wmdf::ProductAdapter::is_connected()
{
  if ( NULL != data_receiver_ )
  {
    return data_receiver_->is_connected();
  }
  return false;
  
}

