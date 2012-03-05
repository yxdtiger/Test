#include "rawdata_adapter.h"
#include "../../session/feed_client.h"
#include "../../environment/server_environment.h"
#include "../../protocol/mdf_message.h"
#include "../../session/message_publisher.h"
#include "../../monitor/monitor_state.h"
#include "../../monitor/productdata_logger.h"
#include "../../monitor/event_pool.h"
#include "../../monitor/feed_event.h"
#include "../../protocol/protocol_description.h"

#include "net_module.h"
#include "rawdata_manager.h"



wmdf::RawDataAdapter::RawDataAdapter(ProductSource* product_source)
:IProductAdapter(product_source)
{
	is_working_=false;
	FH_data_manager_=new FHDataManager();
  ProtocolDescription* pd = new ProtocolDescription(
    product_source->product_id(),0,0,CT_RAWDATA);
  logger_->RegisterProduct(pd);
}


wmdf::RawDataAdapter::~RawDataAdapter()
{
	if (is_working_)
  {
    Stop();
  }
  product_source_ =  NULL;
	
	delete FH_data_manager_;
}

void wmdf::RawDataAdapter::Start()
{
	if (!is_working_)
	{
    is_working_=true;
    data_receiver_ = new FeedClient(RELAY_CLIENT);
    data_receiver_->SetClientCallback(this);
    data_receiver_->Connect( product_source_->ip_address(),product_source_->port(),3,5);
	}
}

void wmdf::RawDataAdapter::Stop()
{
	if (is_working_)
	{
		is_working_=false;
		data_receiver_->DisConnect();
    if (NULL != data_receiver_)
    {
      delete data_receiver_;
    }
	}
}


void wmdf::RawDataAdapter::PublishMdfMessage( MdfMessage* mdf_message )
{
	SharedMdfMessage message = SharedMdfMessage(mdf_message);
	MessagePublisher::PublishOneMessage(message);
}

void wmdf::RawDataAdapter::OnRecvMsg( Message* msg )
{
	int32_t pack_length=msg->msg_len- MSG_HEADER_LEN;

  //记录接收到的数据大小
  receive_size_  += msg->msg_len;
  receive_packet_count_+=1;

  if (ServerEnvironment::mointor_state()->logging())
  {
    uint8_t* data= new uint8_t[msg->msg_len];
    memcpy(data,msg,msg->msg_len);
    ProductRawData* raw_data = new ProductRawData(msg->product_id,data,msg->msg_len);
    logger_->AddToBuffer(raw_data);
  }

	ParseMessage(msg->data,pack_length);
	ReleaseFHData();

}

void wmdf::RawDataAdapter::OnCloseCallback( EventMsg* args )
{
}

void wmdf::RawDataAdapter::OnEventCallback( EventMsg* args )
{
  
}

bool wmdf::RawDataAdapter::is_connected()
{
  return data_receiver_->is_connected();
}
