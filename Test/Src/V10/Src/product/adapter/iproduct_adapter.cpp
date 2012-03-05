#include "iproduct_adapter.h"
#include "../../monitor/productdata_logger.h"
#include "../../session/message_publisher.h"
#include "../../include/date_time.h"
#include "../../session/feed_client.h"

wmdf::IProductAdapter::IProductAdapter( ProductSource* product_source)
{
  logger_ = new ProductDataLogger();
  product_source_ =  product_source;
	packet_id_ = 0;
  receive_size_ = 0;
  receive_packet_count_ = 0;
  last_heart_beat_time_ = 0;
	is_working_ = false;
}

wmdf::IProductAdapter::~IProductAdapter()
{
	product_source_ =  NULL;
  if (NULL != logger_)
  {
    delete logger_;
    logger_ = NULL;
  }

}

void wmdf::IProductAdapter::StartLogger()
{
  logger_->Start();
}

void wmdf::IProductAdapter::StopLogger()
{
  logger_->Stop();
}

void wmdf::IProductAdapter::PublishMdfMessage( MdfMessage* mdf_message )
{
  SharedMdfMessage message = SharedMdfMessage(mdf_message);
  MessagePublisher::PublishOneMessage(message);
}

void wmdf::IProductAdapter::PublishMdfMessage( RelayMdfMessage* mdf_message )
{
  SharedRelayMdfMessage shared_message(mdf_message);
  MessagePublisher::PublishOneMessage(shared_message);
}

bool wmdf::IProductAdapter::is_market_time( int32_t curr_time )
{
  if (product_source_->market_time_count() > 0)
  {
    if ((curr_time >= product_source_->market_times(0)->begin_time()) &&
      (curr_time <= product_source_->market_times(0)->end_time()))
    {
      return true;
    }
    else
    {
      return false;
    }
  }
  return true;
}

bool wmdf::IProductAdapter::is_connected()
{
  return false;
}

void wmdf::IProductAdapter::Clear()
{
  receive_size_ = 0;
}

void wmdf::IProductAdapter::ResetContext()
{

}