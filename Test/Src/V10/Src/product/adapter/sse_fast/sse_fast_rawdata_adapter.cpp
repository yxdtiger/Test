#include "sse_fast_rawdata_adapter.h"
#include "sse_fast_template_decoder.h"
#include "sse_fast_rawdata_converter.h"
#include "../common/rawdata_object.h"
#include <assert.h>

wmdf::SSEFASTRawDataAdapter::SSEFASTRawDataAdapter( ProductSource *product_source,int32_t tradedate)
:ProductAdapter(product_source)
{
  data_source_decoder_ = new SSEFastTemplateDecoder(tradedate);
  sse_fast_converter_ = new SSEFastRawDataConverter();
}


wmdf::SSEFASTRawDataAdapter::~SSEFASTRawDataAdapter()
{
  if (NULL != sse_fast_converter_)
  {
    delete sse_fast_converter_;
    sse_fast_converter_ = NULL;
  }

  if(data_source_decoder_ != NULL)
  {
    delete data_source_decoder_;
    data_source_decoder_ = NULL;
  }
}

uint32_t wmdf::SSEFASTRawDataAdapter::ParseMessage( const uint32_t length )
{
  int32_t offset = data_source_decoder_->DecodeRawData((uint8_t*)message_buffer_,length);
  for (uint32_t index=0;index< data_source_decoder_->msg_items().size();index++)
  {
    WindFastMessage* msg = data_source_decoder_->msg_items()[index];

    switch (msg->MsgType)
    {
    case SSE_INDEXDATA_1113_TYPE:
    case SSE_INDEXDATA_3113_TYPE:
      sse_fast_converter_->convertIndexData(msg,packet_id_);
      break;
    case SSE_MARKETDATA_1102_TYPE:
    case SSE_MARKETDATA_3102_TYPE:
      sse_fast_converter_->convertMarketsnapshot(msg,packet_id_);
      break;
    case SSE_ORDERQUEUE_3111_TYPE:
    case SSE_ORDERQUEUE_1111_TYPE:
      sse_fast_converter_->convertOrderqueue(msg,packet_id_);
      break;
    case SSE_TRANSACTION_1101_TYPE:
    case SSE_TRANSACTION_3101_TYPE:
		case SSE_TRANSACTION_3201_TYPE:
      sse_fast_converter_->convertTransaction(msg,packet_id_);
      break;
    case SSE_VIRTUALAUCTIONPRICE_1107_TYPE:
    case SSE_VIRTUALAUCTIONPRICE_3107_TYPE:
      sse_fast_converter_->convertVirtualauctionprice(msg,packet_id_);
			break;
		case SSE_MARKETDATA_3202_TYPE:
			{
				sse_fast_converter_->convertMarketdata3202(msg,packet_id_);
				sse_fast_converter_->convertOrderqueue3202(msg,packet_id_);
			}
			break;
    default:
      break;
    }
    ReleaseFastMessage(msg);
  }
  return offset;
}

void wmdf::SSEFASTRawDataAdapter::ReleaseFastMessage(WindFastMessage*& curr_msg_)
{
  if(curr_msg_==NULL)
    return;

  switch(curr_msg_->MsgType)
  {
  case SSE_INDEXDATA_1113_TYPE:
  case SSE_INDEXDATA_3113_TYPE:
    delete[] (IndexData*)curr_msg_->Data;
    break;
  case SSE_MARKETDATA_1102_TYPE:
  case SSE_MARKETDATA_3102_TYPE:
    delete[] (MarketData*)curr_msg_->Data;
    break;
  case SSE_TRANSACTION_1101_TYPE:
  case SSE_TRANSACTION_3101_TYPE:
	case SSE_TRANSACTION_3201_TYPE:
    delete[] (Transaction*)curr_msg_->Data;
    break;
  case SSE_VIRTUALAUCTIONPRICE_1107_TYPE:
  case SSE_VIRTUALAUCTIONPRICE_3107_TYPE:
    delete[] (VirtualAuctionPrice*)curr_msg_->Data;
    break;
  case SSE_ORDERQUEUE_1111_TYPE:
  case SSE_ORDERQUEUE_3111_TYPE:
    {
      OrderQueueImage* temp = (OrderQueueImage*)curr_msg_->Data;
      for(int32_t i=0;i!=curr_msg_->Size;++i)
      {
        if(temp[i].order_queue_levels!=NULL)
        {
          OrderQueuePriceLevel* pricelevel = temp[i].order_queue_levels;
          for(int j=0;j!=temp[i].price_level_count;++j)
          {
            if(pricelevel[j].order_queue_items!=NULL)
              delete[] pricelevel[j].order_queue_items;
          }
          delete[] pricelevel;
        }
      }
      delete[] temp;
    }
    break;
    case SSE_MARKETDATA_3202_TYPE:
      {
        MarketData_3202* temp = (MarketData_3202*)curr_msg_->Data;
        for(int32_t i=0;i!=curr_msg_->Size;++i)
        {
          if(temp[i].offerlevels!=NULL)
          {
            PriceLevel_3202* pricelevel = temp[i].offerlevels;
            for(int j=0;j!=temp[i].num_of_offer_level;++j)
            {
              if(pricelevel[j].no_order!=NULL)
                delete[] pricelevel[j].order_items;
            }
            delete[] pricelevel;
          }
          if(temp[i].bidlevels!=NULL)
          {
            PriceLevel_3202* pricelevel = temp[i].bidlevels;
            for(int j=0;j!=temp[i].num_of_bid_level;++j)
            {
              if(pricelevel[j].no_order!=NULL)
                delete[] pricelevel[j].order_items;
            }
            delete[] pricelevel;
          }
        }
        delete[] temp;
      }
      break;
  default:
    break;
  }

  delete curr_msg_;
  curr_msg_=NULL;

}

void wmdf::SSEFASTRawDataAdapter::ResetContext()
{
  if (NULL != sse_fast_converter_)
  {
    sse_fast_converter_->Reset();
  }
  if (NULL != data_source_decoder_)
  {
    data_source_decoder_->ResetContext();
  }
}
