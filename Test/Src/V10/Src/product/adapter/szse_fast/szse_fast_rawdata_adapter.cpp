#include "szse_fast_rawdata_adapter.h"
 #include "szse_fast_template_decoder.h"
 #include "szse_fast_rawdata_converter.h"
#include "fast_szse_template_object.h"


wmdf::SZSEFASTRawDataAdapter::SZSEFASTRawDataAdapter( ProductSource *product_source)
:ProductAdapter(product_source)
{
   data_source_decoder_ = new SZSEFastTemplateDecoder();
   szse_fast_converter_ = new SZSEFastRawDataConverter();
}


wmdf::SZSEFASTRawDataAdapter::~SZSEFASTRawDataAdapter()
{
  if (NULL != szse_fast_converter_)
  {
    delete szse_fast_converter_;
    szse_fast_converter_ = NULL;
  }

  if(data_source_decoder_ != NULL)
  {
    delete data_source_decoder_;
    data_source_decoder_ = NULL;
  }
}

uint32_t wmdf::SZSEFASTRawDataAdapter::ParseMessage( const uint32_t length )
{
  int32_t offset = data_source_decoder_->DecodeRawData((uint8_t*)message_buffer_,length);
  for (uint32_t index=0;index< data_source_decoder_->msg_items().size();index++)
  {
    WindFastMessage* msg = data_source_decoder_->msg_items()[index];
    
    switch (msg->MsgType)
    {
    case SZSE_INDEX_TYPE:
      szse_fast_converter_->convertIndexData(msg,packet_id_);
      break;
		case SZSE_ORDERS_TYPE:
			szse_fast_converter_->convertOrders(msg,packet_id_);
			break;
    case SZSE_SNAPSHOT_TYPE:
      szse_fast_converter_->convertMarketsnapshot(msg,packet_id_);
			if(szse_fast_converter_->Orderqueue_count()>0)
			{
				szse_fast_converter_->convertOrderqueue(msg,packet_id_);
			}
      break;
    case SZSE_TRANSACTION_TYPE:
      szse_fast_converter_->convertTransaction(msg,packet_id_);
      break;
		case SZSE_STOCKINFO_TYPE:
			szse_fast_converter_->convertTradeStatus(msg);
			break;
    default:
      break;
    }
    ReleaseFastMessage(msg);
  }
  return offset;
}

void wmdf::SZSEFASTRawDataAdapter::ReleaseFastMessage(WindFastMessage*& curr_msg_)
{
  if(curr_msg_==NULL)
    return;

  switch(curr_msg_->MsgType)
  {
  case SZSE_TRANSACTION_TYPE:
    delete [] (SZSEL2_Trade_202*)curr_msg_->Data;
    break;
  case SZSE_INDEX_TYPE:
    delete [] (SZSEL2_Index_104*)curr_msg_->Data;
    break;
  case SZSE_ORDERS_TYPE:
    delete [] (SZSEL2_Order_201*)curr_msg_->Data;
    break;
  case SZSE_SNAPSHOT_TYPE:
    {
      SZSEL2_Snapshot_103* snapshot_ptr = (SZSEL2_Snapshot_103*)curr_msg_->Data;
      for(int i=0;i<curr_msg_->Size;i++)
      {
        if(snapshot_ptr[i].nOfferPriceLevelNum>0)
        {
          ptrBidOfferPriceLevel_103 tmp_price_level = snapshot_ptr[i].pOfferPriceLevel;
          if(tmp_price_level->nSequenceLength>0)
            delete[] tmp_price_level->pChildSequence;
          delete[] tmp_price_level;
        }//end if
        if(snapshot_ptr[i].nBidPriceLevelNum>0)
        {
          ptrBidOfferPriceLevel_103 tmp_price_level = snapshot_ptr[i].pBidPriceLevel;
          if(tmp_price_level->nSequenceLength>0)
            delete[] tmp_price_level->pChildSequence;
          delete[] tmp_price_level;
        }//end if
      }
      delete [] snapshot_ptr;
      break;
    }
  case SZSE_STOCKINFO_TYPE:
    {
      SZSEL2_StockInfo_101* stock_info = (SZSEL2_StockInfo_101*) curr_msg_->Data;
      for(int i=0;i<curr_msg_->Size;++i)
      {
        if (stock_info[i].nSecurityAltIDsNum > 0)
        {
          delete stock_info[i].pSecurityAltIDs;
        }
        if (stock_info[i].nIndicesParticipatedNum > 0)
        {
          delete stock_info[i].pIndicesParticipated;
        }
        if (stock_info[i].nMiscFeesNum > 0)
        {
          delete stock_info[i].pMiscFees;
        }
      }
      delete stock_info;
      break;
    }

  default:
    break;
  }
  delete curr_msg_;
  curr_msg_ = NULL;

}

void wmdf::SZSEFASTRawDataAdapter::ResetContext()
{
  if (NULL != data_source_decoder_)
  {
    data_source_decoder_->ResetContext();
  }
}

