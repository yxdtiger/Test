#include "sse_step_rawdata_adapter.h"
#include "sse_step_rawdata_converter.h"
#include "../../../protocol/mdf_message.h"
#include "../../../include/date_time.h"
#include "sse_step_template_decoder.h"
#include "../common/rawdata_object.h"




wmdf::SSEStepRawDataAdapter::SSEStepRawDataAdapter( ProductSource *product_source )
:ProductAdapter(product_source)
{
  rawdata_converter_ = new SSEStepRawDataConverter();
  data_source_decoder_ = new SSEStepTemplateDecoder();
}

wmdf::SSEStepRawDataAdapter::~SSEStepRawDataAdapter()
{
  if (NULL != rawdata_converter_)
  {
	  delete rawdata_converter_;
	  rawdata_converter_ = NULL;
  }
  if (NULL != data_source_decoder_)
  {
    delete data_source_decoder_;
    data_source_decoder_ = NULL;
  }
}

uint32_t wmdf::SSEStepRawDataAdapter::ParseMessage( const uint32_t length )
{
  int32_t offset = data_source_decoder_->DecodeRawData((uint8_t*)message_buffer_,length);
  rawdata_converter_->ConvertData(data_source_decoder_->msg_items());

  if(rawdata_converter_->orderqueue_count()!=0)
  {
    LaunchL2OrderQueue();
  }

  if(rawdata_converter_->transaction_count()!=0)
  {
    LaunchL2Transactions();
  }

  if(rawdata_converter_->marketsnapshot_count()!=0)
  {
    LaunchL2MarketSnapshot();
  }

  if(rawdata_converter_->indexdata_count()!=0)
  {
    LaunchL2IndexData();
  }
  ReleaseMessages(data_source_decoder_->msg_items());
  return offset;
}

void wmdf::SSEStepRawDataAdapter::LaunchL2MarketSnapshot()
{
	MdfMessage* mdf_msg = new MdfMessage();
	mdf_msg->set_local_time(DateTime::get_current_time());
	mdf_msg->set_socket_id(0);
	mdf_msg->set_product_id(PRODUCT_SSE_LEVEL2_ID);
	mdf_msg->set_message_id(SSEL2_MARKETDATA_ID);
	mdf_msg->set_major_version(MAJOR_VERSION_SSE_LEVEL2_ID);
	mdf_msg->set_minor_version(MINOR_VERSION_SSE_LEVEL2_ID);
	mdf_msg->set_record_count(rawdata_converter_->marketsnapshot_count());


	MarketMessageHeader* message_header = new MarketMessageHeader();
	message_header->place_holder = 0;
	mdf_msg->set_packet_num(++packet_id_);
	mdf_msg->set_header(message_header);
	mdf_msg->set_body(rawdata_converter_->get_marketsnapshot());
	PublishMdfMessage(mdf_msg);
}

void wmdf::SSEStepRawDataAdapter::LaunchL2Transactions()
{
	MdfMessage* mdf_msg = new MdfMessage();
	mdf_msg->set_local_time(DateTime::get_current_time());
	mdf_msg->set_socket_id(0);
	mdf_msg->set_product_id(PRODUCT_SSE_LEVEL2_ID);
	mdf_msg->set_message_id(SSEL2_TRANSACTION_ID);
	mdf_msg->set_major_version(MAJOR_VERSION_SSE_LEVEL2_ID);
	mdf_msg->set_minor_version(MINOR_VERSION_SSE_LEVEL2_ID);
	mdf_msg->set_record_count(rawdata_converter_->transaction_count());


	MarketMessageHeader* message_header = new MarketMessageHeader();
	message_header->place_holder = 0;
	mdf_msg->set_packet_num(++packet_id_);
	mdf_msg->set_header(message_header);
	mdf_msg->set_body(rawdata_converter_->get_transaction());
	PublishMdfMessage(mdf_msg);
}

void wmdf::SSEStepRawDataAdapter::LaunchL2OrderQueue()
{
	MdfMessage* mdf_msg = new MdfMessage();
	mdf_msg->set_local_time(DateTime::get_current_time());
	mdf_msg->set_socket_id(0);
	mdf_msg->set_product_id(PRODUCT_SSE_LEVEL2_ID);
	mdf_msg->set_message_id(SSEL2_ORDERQUEUE_ID);
	mdf_msg->set_major_version(MAJOR_VERSION_SSE_LEVEL2_ID);
	mdf_msg->set_minor_version(MINOR_VERSION_SSE_LEVEL2_ID);
	mdf_msg->set_record_count(rawdata_converter_->orderqueue_count());

	MarketMessageHeader* message_header = new MarketMessageHeader();
	message_header->place_holder = 0;
	mdf_msg->set_packet_num(++packet_id_);
	mdf_msg->set_header(message_header);
	mdf_msg->set_body(rawdata_converter_->get_orderqueue());
	PublishMdfMessage(mdf_msg);	
}

void wmdf::SSEStepRawDataAdapter::LaunchL2IndexData()
{
	MdfMessage* mdf_msg = new MdfMessage();
	mdf_msg->set_local_time(DateTime::get_current_time());
	mdf_msg->set_socket_id(0);
	mdf_msg->set_product_id(PRODUCT_SSE_LEVEL2_ID);
	mdf_msg->set_message_id(SSEL2_INDEXDATA_ID);
	mdf_msg->set_major_version(MAJOR_VERSION_SSE_LEVEL2_ID);
	mdf_msg->set_minor_version(MINOR_VERSION_SSE_LEVEL2_ID);
	mdf_msg->set_record_count(rawdata_converter_->indexdata_count());

	MarketMessageHeader* message_header = new MarketMessageHeader();
	message_header->place_holder = 0;
	mdf_msg->set_packet_num(++packet_id_);
	mdf_msg->set_header(message_header);
	mdf_msg->set_body(rawdata_converter_->get_indexdata());
	PublishMdfMessage(mdf_msg);	
}

void wmdf::SSEStepRawDataAdapter::ReleaseMessages(std::vector<WindStepMessage*>& msgs)
{
  for(uint32_t index=0;index<msgs.size();index++)
  {
    WindStepMessage* msg =msgs[index];
    switch(msg->MsgType)
    {
		case SSE_INDEXDATA_1113_TYPE:
			delete (IndexData*)msg->Data;
			break;
    case SSE_TRANSACTION_1101_TYPE:
      delete (Transaction*)msg->Data;
      break;
//     case SSE_MARKETOVERVIEW_1115_TYPE:
//       delete (MarketOverView1115*)msg->Data;
//       break;
    case SSE_MARKETDATA_1102_TYPE:
      delete (MarketData*)msg->Data;
      break;
    case SSE_VIRTUALAUCTIONPRICE_1107_TYPE:
      delete (VirtualAuctionPrice*)msg->Data;
      break;
    case SSE_ORDERQUEUE_1111_TYPE:
      {
        OrderQueueImage* temp = (OrderQueueImage*)msg->Data;
        if (temp->price_level_count != 0)
        {
          for (int j=0; j<temp->price_level_count;j++)
          {
            if (temp->order_queue_levels[j].order_queue_items!=NULL)
            {
              delete[] temp->order_queue_levels[j].order_queue_items;
              temp->order_queue_levels[j].order_queue_items= NULL;
            }
          }
          delete[] temp->order_queue_levels;
          temp->order_queue_levels = NULL;
        }
        delete temp;
        temp = NULL;
      }
      break;
    default:
      break;
    }
    delete msg;
    msg=NULL;
  }
  msgs.clear();
}

void wmdf::SSEStepRawDataAdapter::ResetContext()
{
  if (NULL != data_source_decoder_)
  {
    data_source_decoder_->ResetContext();
  }
}


