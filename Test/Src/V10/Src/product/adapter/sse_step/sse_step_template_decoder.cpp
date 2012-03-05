#include "sse_step_template_decoder.h"
#include "sse_step_transaction_template.h"
#include "sse_step_indexdata_template.h"
#include "sse_step_marketdata_template.h"
#include "sse_step_virtualauctionprice_template.h"
#include "sse_step_orderqueue_template.h"
#include "../common/rawdata_object.h"
#include "../../../environment/strutils.h"

const int SSE_MSG_HEADER_LEN = 75;
const int SSE_MAG_TAILER_LEN = 9;//
const int SSE_MSG_LEN_STRRT = 15;//


wmdf::SSEStepTemplateDecoder::SSEStepTemplateDecoder()
{
  // strlen("20061213 07:55:55|8=STEP.1.0.09=5935=A49=VDE56=VDR34=052=347=UTF-8 ");
  exchange_msg_min_len_  = 75;
  //strlen("8=STEP.1.0.0 9=")
  exchange_msg_head_len_ = 15;
  //strlen("10=255  ")
  exchange_mag_tail_len_ = 9;
  sse_step_indexdata_template_ = new SSEStepIndexDataTemplate();
  sse_step_virtualauctionprice_template_ = new SSEStepVirtualAuctionPriceTemplate();
  sse_step_marketdata_template_ = new SSEStepMarketDataTemplate();
  sse_step_orderqueue_template_ = new SSEStepOrderQueueTemplate();
  sse_step_transaction_template_ = new SSEStepTransactionTemplate();
}

wmdf::SSEStepTemplateDecoder::~SSEStepTemplateDecoder()
{
  if (NULL != sse_step_indexdata_template_)
  {
    delete sse_step_indexdata_template_;
    sse_step_indexdata_template_ = NULL;
  }

  if (NULL != sse_step_virtualauctionprice_template_)
  {
    delete sse_step_virtualauctionprice_template_;
    sse_step_virtualauctionprice_template_ = NULL;
  }

  if (NULL != sse_step_marketdata_template_)
  {
    delete sse_step_marketdata_template_;
    sse_step_marketdata_template_ = NULL;
  }

  if (NULL != sse_step_orderqueue_template_)
  {
    delete sse_step_orderqueue_template_;
    sse_step_orderqueue_template_ = NULL;
  }

  if (NULL != sse_step_transaction_template_)
  {
    delete sse_step_transaction_template_;
    sse_step_transaction_template_ = NULL;
  }
}

void wmdf::SSEStepTemplateDecoder::DecodeMessage( uint8_t* tid,uint8_t* message_start,uint8_t* message_end )
{
  if(strcmp((char*)tid,SSE_VIRTUALAUCTIONPRICE_1107)==0)
  {
    WindStepMessage* msg = sse_step_virtualauctionprice_template_->DecodeRaw(message_start,message_end);
    if (NULL != msg)
    {
      msg_items_.push_back(msg);
    }
  }
  else if(strcmp((char*)tid,SSE_ORDERQUEUE_1111)==0)
  {
    WindStepMessage* msg = sse_step_orderqueue_template_->DecodeRaw(message_start,message_end);
    if (NULL != msg)
    {
      msg_items_.push_back(msg);
    }
  }
  else if(strcmp((char*)tid,SSE_MARKETDATA_1102)==0)
  {
    WindStepMessage* msg = sse_step_marketdata_template_->DecodeRaw(message_start,message_end);
    if (NULL != msg)
    {
      msg_items_.push_back(msg);
    }
  }
  else if(strcmp((char*)tid,SSE_TRANSACTION_1101)==0)
  {
    WindStepMessage* msg = sse_step_transaction_template_->DecodeRaw(message_start,message_end);
    if (NULL != msg)
    {
      msg_items_.push_back(msg);
    }
  }
  else if(strcmp((char*)tid,SSE_INDEXDATA_1113)==0)
  {
    WindStepMessage* msg = sse_step_indexdata_template_->DecodeRaw(message_start,message_end);
    if (NULL != msg)
    {
      msg_items_.push_back(msg);
    }
  }

}
