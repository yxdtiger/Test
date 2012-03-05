#include "szse_fast_template_decoder.h"
#include "fast_szse_index_template.h"
#include "fast_szse_orders_template.h"
#include "fast_szse_snapshot_template.h"
#include "fast_szse_stockinfo_template.h"
#include "fast_szse_trade_template.h"
#include "../common/rawdata_object.h"
#include "fast_szse_template_object.h"

wmdf::SZSEFastTemplateDecoder::SZSEFastTemplateDecoder()
{
  exchange_msg_min_len_ = 85;  //市场最小包大小
  exchange_msg_head_len_ = 15;//消息头大小
  exchange_mag_tail_len_ = 7;//消息尾字节数
  fast_szse_index_template_ = new FastSzseIndexTemplate();
  fast_szse_orders_template_ = new FastSzseOrdersTemplate();
  fast_szse_snapshot_template_ = new FastSzseSnapshotTemplate();
  fast_szse_stockinfo_template_ = new FastSzseStockInfoTemplate();
  fast_szse_trade_template_ = new FastSzseTradeTemplate();
}

wmdf::SZSEFastTemplateDecoder::~SZSEFastTemplateDecoder()
{
	delete fast_szse_index_template_;
	delete fast_szse_orders_template_;
	delete fast_szse_snapshot_template_;
	delete fast_szse_stockinfo_template_;
	delete fast_szse_trade_template_;
}

void wmdf::SZSEFastTemplateDecoder::Decode96Field( uint8_t* tid,uint8_t*& field96_start,uint8_t* field96_end )
{
  int32_t field96_len = 0;
  if(strcmp((char*)tid,SZSE_TRANSACTION_STR)==0)
  {
    if (FindContext(field96_start,field96_end,field96_len))
    {
      WindFastMessage* msg =  fast_szse_trade_template_->DecodeRaw(field96_start,field96_len);
      if (NULL != msg)
      {
        msg_items_.push_back(msg);
      }
    }
    fast_content_count_++;
  }

  else if(strcmp((char*)tid,SZSE_SNAPSHOT_STR)==0)
  {
    if (FindContext(field96_start,field96_end,field96_len))
    {
      WindFastMessage* msg = fast_szse_snapshot_template_->DecodeRaw(field96_start,field96_len);
      if (NULL != msg)
      {
        msg_items_.push_back(msg);
      }
    }
    fast_content_count_++;
  }

  else if(strcmp((char*)tid,SZSE_INDEX_STR)==0)
  {
    if (FindContext(field96_start,field96_end,field96_len))
    {
      WindFastMessage* msg =  fast_szse_index_template_->DecodeRaw(field96_start,field96_len);
      if (NULL != msg)
      {
        msg_items_.push_back(msg);
      }
    }
    fast_content_count_++;
  }
  else if(strcmp((char*)tid,SZSE_ORDERS_STR)==0)
  {
    if (FindContext(field96_start,field96_end,field96_len))
    {
      WindFastMessage* msg =  fast_szse_orders_template_->DecodeRaw(field96_start,field96_len);
      if (NULL != msg)
      {
        msg_items_.push_back(msg);
      }
    }
    fast_content_count_++;
  }
  else if(strcmp((char*)tid,SZSE_STOCKINFO_STR)==0)
  {
    if (FindContext(field96_start,field96_end,field96_len))
    {
      WindFastMessage* msg =  fast_szse_stockinfo_template_->DecodeRaw(field96_start,field96_len);
      if (NULL != msg)
      {
        msg_items_.push_back(msg);
      }
    }
    fast_content_count_++;
  }

}
