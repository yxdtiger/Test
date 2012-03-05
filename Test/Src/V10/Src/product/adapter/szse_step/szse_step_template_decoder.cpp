#include "szse_step_template_decoder.h"
#include "szse_step_transaction_template.h"
#include "szse_step_indexdata_template.h"
#include "szse_step_marketdata_template.h"
#include "szse_step_orders_template.h"
#include "../common/step_codec.h"
#include "../common/rawdata_object.h"
#include "../../../environment/strutils.h"

wmdf::SZSEStepTemplateDecoder::SZSEStepTemplateDecoder()
{
  //strlen("8=STEP.1.0.0 9=107 35=5 49=101D01 56=101S02 34=4774 52=20071022-10:00:00.000 347=GBK ");
  exchange_msg_min_len_  = 85;
  //strlen("8=STEP.1.0.0 9=")
  exchange_msg_head_len_ = 15;
  // strlen("10=123 ")
  exchange_mag_tail_len_ = 7;
	szse_step_transaction_template_ = new SZSEStepTransactionTemplate();
	szse_step_marketdata_template_ = new SZSEStepMarketDataTemplate();
	szse_step_indexdata_template_ = new SZSEStepIndexDataTemplate();
	szse_step_orders_template_ = new SZSEStepOrdersTemplate();
}

wmdf::SZSEStepTemplateDecoder::~SZSEStepTemplateDecoder()
{
	if(NULL != szse_step_transaction_template_)
	{
		delete szse_step_transaction_template_;
		szse_step_transaction_template_ = NULL;
	}

	if(NULL != szse_step_marketdata_template_)
	{
		delete szse_step_marketdata_template_;
		szse_step_marketdata_template_ = NULL;
	}

	if(NULL != szse_step_indexdata_template_)
	{
		delete szse_step_indexdata_template_;
		szse_step_indexdata_template_ = NULL;
	}

	if(NULL != szse_step_orders_template_)
	{
		delete szse_step_orders_template_;
		szse_step_orders_template_ = NULL;
	}
}

void wmdf::SZSEStepTemplateDecoder::DecodeMessage( uint8_t* tid,uint8_t* message_start,uint8_t* message_end )
{
  if(strcmp((char*)tid,TEMPLATE_SZSE_TRANSACTION_202)==0)
  {
    WindStepMessage* msg = szse_step_transaction_template_->DecodeRaw(message_start,message_end);
    if (NULL != msg)
    {
      msg_items_.push_back(msg);
    }
  }
  else if(strcmp((char*)tid,SZSE_MARKETDATA_103)==0)
  {
    WindStepMessage* msg = szse_step_marketdata_template_->DecodeRaw(message_start,message_end);
    if (NULL != msg)
    {
      msg_items_.push_back(msg);
    }
    if (NULL != szse_step_marketdata_template_->ask_order_queue_msg())
    {
      msg_items_.push_back(szse_step_marketdata_template_->ask_order_queue_msg());
    }
    if (NULL != szse_step_marketdata_template_->bid_order_queue_msg())
    {
      msg_items_.push_back(szse_step_marketdata_template_->bid_order_queue_msg());
    }
  }
  else if(strcmp((char*)tid,SZSE_INDEXDATA_104)==0)
  {
    WindStepMessage* msg = szse_step_indexdata_template_->DecodeRaw(message_start,message_end);
    if (NULL != msg)
    {
      msg_items_.push_back(msg);
    }
  }
  else if(strcmp((char*)tid,TEMPLATE_SZSE_ORDERS_201)==0)
  {
    WindStepMessage* msg = szse_step_orders_template_->DecodeRaw(message_start,message_end);
    if (NULL != msg)
    {
      msg_items_.push_back(msg);
    }
  }
}
