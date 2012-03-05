#include "szse_step_rawdata_adapter.h"
#include "szse_step_rawdata_converter.h"
#include "../../../protocol/mdf_message.h"
#include "../../../include/date_time.h"
#include "szse_step_template_decoder.h"


wmdf::SZSEStepRawDataAdapter::SZSEStepRawDataAdapter( ProductSource* product_source )
:ProductAdapter(product_source)
{
  rawdata_converter_ = new SZSEStepRawDataConverter();
	data_source_decoder_ = new SZSEStepTemplateDecoder();
}


wmdf::SZSEStepRawDataAdapter::~SZSEStepRawDataAdapter()
{
  if (NULL != rawdata_converter_)
  {
	  delete rawdata_converter_;
	  rawdata_converter_ = NULL;
  }
  if(data_source_decoder_ != NULL)
  {
    delete data_source_decoder_;
    data_source_decoder_ = NULL;
  }
}


uint32_t wmdf::SZSEStepRawDataAdapter::ParseMessage( const uint32_t length)
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

	if(rawdata_converter_->orders_count()!=0)
	{
		LaunchL2Orders();
	}
  ReleaseRawData(data_source_decoder_->msg_items());
  return offset;
}



void wmdf::SZSEStepRawDataAdapter::ReleaseRawData(std::vector<WindStepMessage*>& msgs)
{
  for(std::vector<WindStepMessage*>::iterator iter =msgs.begin();iter!=msgs.end();++iter)
  {
    WindStepMessage* msg =(*iter);
    switch(msg->MsgType)
    {
    case SZSE_TRANSACTION_202_TYPE:
      delete (Transaction*)msg->Data;
      break;
    case SZSE_INDEXDATA_104_TYPE:
      delete (IndexData*)msg->Data;
      break;
    case SZSE_MARKETDATA_103_TYPE:
      delete (MarketData*)msg->Data;
      break;
    case TEMPLATE_SZSE_ORDERS_201_TYPE:
      delete (Orders*)msg->Data;
      break;
    case SZSE_ORDERQUEUE_TYPE:
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
        delete temp->order_queue_levels;
        temp->order_queue_levels = NULL;
      }

      delete temp;
      temp=NULL;
      break;
    }
    delete msg;
    msg=NULL;
  }
}



void wmdf::SZSEStepRawDataAdapter::LaunchL2MarketSnapshot()
{
	MdfMessage* mdf_msg = new MdfMessage();
	mdf_msg->set_local_time(DateTime::get_current_time());
	mdf_msg->set_socket_id(0);
	mdf_msg->set_product_id(PRODUCT_SZSE_LEVEL2_ID);
	mdf_msg->set_message_id(SZSEL2_MARKETDATA_ID);
	mdf_msg->set_major_version(MAJOR_VERSION_SZSE_LEVEL2_ID);
	mdf_msg->set_minor_version(MINOR_VERSION_SZSE_LEVEL2_ID);
	mdf_msg->set_record_count(rawdata_converter_->marketsnapshot_count());


	MarketMessageHeader* message_header = new MarketMessageHeader();
	message_header->place_holder = 0;
	mdf_msg->set_packet_num(++packet_id_);
	mdf_msg->set_header(message_header);
	mdf_msg->set_body(rawdata_converter_->get_marketsnapshot());
	//Utils::PrintMDFMessageData(mdf_msg,os_test);
	PublishMdfMessage(mdf_msg);
}

void wmdf::SZSEStepRawDataAdapter::LaunchL2Transactions()
{
	MdfMessage* mdf_msg = new MdfMessage();
	mdf_msg->set_local_time(DateTime::get_current_time());
	mdf_msg->set_socket_id(0);
	mdf_msg->set_product_id(PRODUCT_SZSE_LEVEL2_ID);
	mdf_msg->set_message_id(SZSEL2_TRANSACTION_ID);
	mdf_msg->set_major_version(MAJOR_VERSION_SZSE_LEVEL2_ID);
	mdf_msg->set_minor_version(MINOR_VERSION_SZSE_LEVEL2_ID);
	mdf_msg->set_record_count(rawdata_converter_->transaction_count());


	MarketMessageHeader* message_header = new MarketMessageHeader();
	message_header->place_holder = 0;
	mdf_msg->set_packet_num(++packet_id_);
	mdf_msg->set_header(message_header);
	mdf_msg->set_body(rawdata_converter_->get_transaction());
	//Utils::PrintMDFMessageData(mdf_msg,os_test);
	PublishMdfMessage(mdf_msg);
}

void wmdf::SZSEStepRawDataAdapter::LaunchL2OrderQueue()
{
	MdfMessage* mdf_msg = new MdfMessage();
	mdf_msg->set_local_time(DateTime::get_current_time());
	mdf_msg->set_socket_id(0);
	mdf_msg->set_product_id(PRODUCT_SZSE_LEVEL2_ID);
	mdf_msg->set_message_id(SZSEL2_ORDERQUEUE_ID);
	mdf_msg->set_major_version(MAJOR_VERSION_SZSE_LEVEL2_ID);
	mdf_msg->set_minor_version(MINOR_VERSION_SZSE_LEVEL2_ID);
	mdf_msg->set_record_count(rawdata_converter_->orderqueue_count());

	MarketMessageHeader* message_header = new MarketMessageHeader();
	message_header->place_holder = 0;
	mdf_msg->set_packet_num(++packet_id_);
	mdf_msg->set_header(message_header);
	mdf_msg->set_body(rawdata_converter_->get_orderqueue());
	//Utils::PrintMDFMessageData(mdf_msg,os_test);
	PublishMdfMessage(mdf_msg);	
}

void wmdf::SZSEStepRawDataAdapter::LaunchL2IndexData()
{
	MdfMessage* mdf_msg = new MdfMessage();
	mdf_msg->set_local_time(DateTime::get_current_time());
	mdf_msg->set_socket_id(0);
	mdf_msg->set_product_id(PRODUCT_SZSE_LEVEL2_ID);
	mdf_msg->set_message_id(SZSEL2_INDEXDATA_ID);
	mdf_msg->set_major_version(MAJOR_VERSION_SZSE_LEVEL2_ID);
	mdf_msg->set_minor_version(MINOR_VERSION_SZSE_LEVEL2_ID);
	mdf_msg->set_record_count(rawdata_converter_->indexdata_count());

	MarketMessageHeader* message_header = new MarketMessageHeader();
	message_header->place_holder = 0;
	mdf_msg->set_packet_num(++packet_id_);
	mdf_msg->set_header(message_header);
	mdf_msg->set_body(rawdata_converter_->get_indexdata());
	//Utils::PrintMDFMessageData(mdf_msg,os_test);
	PublishMdfMessage(mdf_msg);	
}

void wmdf::SZSEStepRawDataAdapter::LaunchL2Orders()
{
	MdfMessage* mdf_msg = new MdfMessage();
	mdf_msg->set_local_time(DateTime::get_current_time());
	mdf_msg->set_socket_id(0);
	mdf_msg->set_product_id(PRODUCT_SZSE_LEVEL2_ID);
	mdf_msg->set_message_id(SZSEL2_SINGLEORDER_ID);
	mdf_msg->set_major_version(MAJOR_VERSION_SZSE_LEVEL2_ID);
	mdf_msg->set_minor_version(MINOR_VERSION_SZSE_LEVEL2_ID);
	mdf_msg->set_record_count(rawdata_converter_->orders_count());

	MarketMessageHeader* message_header = new MarketMessageHeader();
	message_header->place_holder = 0;
	mdf_msg->set_packet_num(++packet_id_);
	mdf_msg->set_header(message_header);
	mdf_msg->set_body(rawdata_converter_->get_orders());
	//Utils::PrintMDFMessageData(mdf_msg,os_test);
	PublishMdfMessage(mdf_msg);	
}

void wmdf::SZSEStepRawDataAdapter::ResetContext()
{
  if (NULL != data_source_decoder_)
  {
    data_source_decoder_->ResetContext();
  }
}












