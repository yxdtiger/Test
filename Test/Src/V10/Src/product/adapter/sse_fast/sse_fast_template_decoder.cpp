#include "sse_fast_template_decoder.h"
#include "sse_fast_indexdata_3113_template.h"
#include "sse_fast_marketdata_3102_template.h"
#include "sse_fast_orderqueue_3111_template.h"
#include "sse_fast_virtualauctionprice_3107_template.h"
#include "sse_fast_transaction_3101_template.h"
#include "sse_fast_indexdata_1113_template.h"
#include "sse_fast_virtualauctionprice_1107_template.h"
#include "sse_fast_orderqueue_1111_template.h"
#include "sse_fast_transaction_1101_template.h"
#include "sse_fast_marketdata_1102_template.h"
#include "sse_fast_indexdata_3113_v30_template.h"
#include "sse_fast_marketdata_3202_template.h"
#include "sse_fast_transaction_3201_template.h"
#include "../common/rawdata_object.h"

wmdf::SSEFastTemplateDecoder::SSEFastTemplateDecoder(int32_t trade_date)
{
  exchange_msg_min_len_ = 78;  //市场最小包大小
  exchange_msg_head_len_ = 15;//消息头大小
  exchange_mag_tail_len_ = 7;//消息尾字节数

  sse_marketdata_3102_template_ = new SSEFastMarketData3102Template();
  sse_orderqueue_3111_template_ = new SSEFastOrderQueue3111Template();
  if (20111211 == trade_date)
  {
    sse_indexdata_3113_template_ = new SSEFastIndexData3113Template();
  }
  else
  {
    sse_indexdata_3113_template_ = new SSEFastIndexData3113_V30Template();
  }
  
  sse_virtualauctionprice_3107_template_ = new SSEFastVirtualAuctionPrice3107Template();
  sse_transaction_3101_template_ = new SSEFastTransaction3101Template();
  sse_indexdata_1113_template_ = new SSEFastIndexData1113Template();
  sse_virtualauctionprice_1107_template_ = new SSEFastVirtualAuctionPrice1107Template();
  sse_orderqueue_1111_template_ = new SSEFastOrderQueue1111Template();
  sse_transaction_1101_template_ = new SSEFastTransaction1101Template();
  sse_marketdata_1102_template_ = new SSEFastMarketData1102Template();	

  sse_marketdata_3202_template_ = new SSEFastMarketData3202Template();
  sse_transaction_3201_template_ = new SSEFastTransaction3201Template();
  
}

wmdf::SSEFastTemplateDecoder::~SSEFastTemplateDecoder()
{
	delete sse_marketdata_3102_template_;
	delete sse_orderqueue_3111_template_;
	delete sse_indexdata_3113_template_;
	delete sse_virtualauctionprice_3107_template_;
	delete sse_transaction_3101_template_;
	delete sse_indexdata_1113_template_;
	delete sse_virtualauctionprice_1107_template_;
	delete sse_orderqueue_1111_template_;
	delete sse_transaction_1101_template_;
	delete sse_marketdata_1102_template_;
  delete sse_transaction_3201_template_;
  delete sse_marketdata_3202_template_;
}

void wmdf::SSEFastTemplateDecoder::Decode96Field(uint8_t* tid,uint8_t*& field96_start,uint8_t* field96_end)
{
  int32_t field96_len = 0;
  if(strcmp((char*)tid,SSE_MARKETDATA_3202)==0)
  {
    if (FindContext(field96_start,field96_end,field96_len))
    {
      WindFastMessage* msg =  sse_marketdata_3202_template_->DecodeRaw(field96_start,field96_len);
      if(NULL != msg) {msg_items_.push_back(msg);}
    }
    fast_content_count_++;
  }

  else if(strcmp((char*)tid,SSE_TRANSACTION_3201)==0)
  {
    if (FindContext(field96_start,field96_end,field96_len))
    {
      WindFastMessage* msg =  sse_transaction_3201_template_->DecodeRaw(field96_start,field96_len);
      if(NULL != msg) {msg_items_.push_back(msg);}
    }
    fast_content_count_++;
  }
  else if(strcmp((char*)tid,SSE_INDEXDATA_3113)==0)
  {
    if (FindContext(field96_start,field96_end,field96_len))
    {
      WindFastMessage* msg = sse_indexdata_3113_template_->DecodeRaw(field96_start,field96_len);
      if(NULL != msg) {msg_items_.push_back(msg);}
    }
    fast_content_count_++;
  }

  else if(strcmp((char*)tid,SSE_VIRTUALAUCTIONPRICE_3107)==0)
  {
    if (FindContext(field96_start,field96_end,field96_len))
    {
      WindFastMessage* msg =  sse_virtualauctionprice_3107_template_->DecodeRaw(field96_start,field96_len);
      if(NULL != msg) {msg_items_.push_back(msg);}
    }
    fast_content_count_++;
  }

  else if(strcmp((char*)tid,SSE_MARKETDATA_3102)==0)
  {
    if (FindContext(field96_start,field96_end,field96_len))
    {
      WindFastMessage* msg =  sse_marketdata_3102_template_->DecodeRaw(field96_start,field96_len);
      if(NULL != msg) {msg_items_.push_back(msg);}
    }
    fast_content_count_++;
  }
  else if(strcmp((char*)tid,SSE_ORDERQUEUE_3111)==0)
  {
    if (FindContext(field96_start,field96_end,field96_len))
    {
      WindFastMessage* msg =  sse_orderqueue_3111_template_->DecodeRaw(field96_start,field96_len);
      if(NULL != msg) {msg_items_.push_back(msg);}
    }
    fast_content_count_++;
  }
  else if(strcmp((char*)tid,SSE_TRANSACTION_3101)==0)
  {
    if (FindContext(field96_start,field96_end,field96_len))
    {
      WindFastMessage* msg = sse_transaction_3101_template_->DecodeRaw(field96_start,field96_len);
      if(NULL != msg) {msg_items_.push_back(msg);}
    }
    fast_content_count_++;
  }
  else if(strcmp((char*)tid,SSE_TRANSACTION_1101)==0)
  {
    if (FindContext(field96_start,field96_end,field96_len))
    {
      WindFastMessage* msg =  sse_transaction_1101_template_->DecodeRaw(field96_start,field96_len);
      if(NULL != msg) {msg_items_.push_back(msg);}
    }
    fast_content_count_++;
  }
  else if(strcmp((char*)tid,SSE_MARKETDATA_1102)==0)
  {
    if (FindContext(field96_start,field96_end,field96_len))
    {
      WindFastMessage* msg = sse_marketdata_1102_template_->DecodeRaw(field96_start,field96_len);
      if(NULL != msg) {msg_items_.push_back(msg);}
    }
    fast_content_count_++;
  }
  else if(strcmp((char*)tid,SSE_INDEXDATA_1113)==0)
  {
    if (FindContext(field96_start,field96_end,field96_len))
    {
      WindFastMessage* msg =  sse_indexdata_1113_template_->DecodeRaw(field96_start,field96_len);
      if(NULL != msg) {msg_items_.push_back(msg);}
    }
    fast_content_count_++;
  }
  else if(strcmp((char*)tid,SSE_ORDERQUEUE_1111)==0)
  {
    if (FindContext(field96_start,field96_end,field96_len))
    {
      WindFastMessage* msg = sse_orderqueue_1111_template_->DecodeRaw(field96_start,field96_len);
      if(NULL != msg) {msg_items_.push_back(msg);}
    }
    fast_content_count_++;
  }
  else if(strcmp((char*)tid,SSE_VIRTUALAUCTIONPRICE_1107)==0)
  {
    if (FindContext(field96_start,field96_end,field96_len))
    {
      WindFastMessage* msg = sse_virtualauctionprice_1107_template_->DecodeRaw(field96_start,field96_len);
      if(NULL != msg) {msg_items_.push_back(msg);}
    }
    fast_content_count_++;
  }

}

