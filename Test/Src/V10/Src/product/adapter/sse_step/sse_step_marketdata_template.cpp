#include "sse_step_marketdata_template.h"

wmdf::SSEStepMarketDataTemplate::SSEStepMarketDataTemplate()
{

}

wmdf::SSEStepMarketDataTemplate::~SSEStepMarketDataTemplate()
{

}

wmdf::WindStepMessage* wmdf::SSEStepMarketDataTemplate::DecodeRaw( uint8_t* message_start,uint8_t* message_end )
{
  int ret=0;
  step_decoder_->ReSet((char*)message_start,(char*)message_end);
  MarketData* data_array = new MarketData();
  for(size_t i=0; i!=10; ++i)
  {
    data_array->ask_price[i] = 0;
    data_array->ask_size[i] = 0;
    data_array->bid_price[i] = 0;
    data_array->bid_size[i] = 0;
  }
  data_array->l2_total_bid_qty = WMDF_CONSTS_64BIT_NIL_VALUE;
  data_array->l2_total_offer_qty = WMDF_CONSTS_64BIT_NIL_VALUE;
  data_array->new_price = WMDF_CONSTS_DOUBLE_NIL_VALUE;
  data_array->pre_close = WMDF_CONSTS_DOUBLE_NIL_VALUE;
  data_array->today_high = WMDF_CONSTS_DOUBLE_NIL_VALUE;
  data_array->today_low = WMDF_CONSTS_DOUBLE_NIL_VALUE;
  data_array->today_open = WMDF_CONSTS_DOUBLE_NIL_VALUE;
  data_array->total_amount = WMDF_CONSTS_DOUBLE_NIL_VALUE;
  data_array->trade_date = WMDF_CONSTS_32BIT_NIL_VALUE;
  data_array->trade_time = WMDF_CONSTS_32BIT_NIL_VALUE;
  data_array->transactions_count = WMDF_CONSTS_64BIT_NIL_VALUE;
  data_array->volume = WMDF_CONSTS_64BIT_NIL_VALUE;
  data_array->weighted_average_best_ask = WMDF_CONSTS_DOUBLE_NIL_VALUE;
  data_array->weighted_average_best_bid = WMDF_CONSTS_DOUBLE_NIL_VALUE;
  data_array->peratio1 = WMDF_CONSTS_DOUBLE_NIL_VALUE;
  data_array->peratio2 = WMDF_CONSTS_DOUBLE_NIL_VALUE;
  data_array->trade_status = WMDF_CONSTS_8BIT_NIL_VALUE;
  memset(data_array->wind_code,0,sizeof(data_array->wind_code));


  int32_t nobidpricelevel;
  int32_t noofferpricelevel;

  if(FIND_TAG("10072"))
    step_decoder_->DecodeInt32();
  if(FIND_TAG("10178"))
    data_array->trade_time = step_decoder_->DecodeInt32();
  if(FIND_TAG("10121")) //data status
  {
    delete data_array;
    return NULL;
  }
  if(FIND_TAG("48"))
    step_decoder_->DecodeString(data_array->wind_code,sizeof(data_array->wind_code));


  if(FIND_TAG("10018"))
  {
    data_array->today_open = step_decoder_->DecodeDouble();
    if(data_array->today_open == 0)
      data_array->today_open = WMDF_CONSTS_DOUBLE_NIL_VALUE;
  }
  if(FIND_TAG("332"))
  {
    data_array->today_high = step_decoder_->DecodeDouble();
    if(data_array->today_high == 0)
      data_array->today_high = WMDF_CONSTS_DOUBLE_NIL_VALUE;
  }

  if(FIND_TAG("333"))
  {
    data_array->today_low = step_decoder_->DecodeDouble();
    if(data_array->today_low == 0)
      data_array->today_low = WMDF_CONSTS_DOUBLE_NIL_VALUE;
  }
  if(FIND_TAG("31"))
  {
    data_array->new_price = step_decoder_->DecodeDouble();
    if(data_array->new_price == 0)
      data_array->new_price = WMDF_CONSTS_DOUBLE_NIL_VALUE;
  }
  if(FIND_TAG("10068"))
  {
    nobidpricelevel = step_decoder_->DecodeInt32();

    for(size_t i=0; i!= nobidpricelevel; ++i)
    {
      if(FIND_TAG("134"))
        data_array->bid_size[i] = step_decoder_->DecodeInt32();
      if(FIND_TAG("132"))
        data_array->bid_price[i] = step_decoder_->DecodeDouble();
    }
  }

  if(FIND_TAG("10069"))
  {
    noofferpricelevel = step_decoder_->DecodeInt32();

    for(size_t i=0; i!=noofferpricelevel; ++i)
    {
      if(FIND_TAG("135"))
        data_array->ask_size[i] = step_decoder_->DecodeInt32();
      if(FIND_TAG("133"))
        data_array->ask_price[i] = step_decoder_->DecodeDouble();
    }
  }

  if(FIND_TAG("8503"))
  {
    data_array->transactions_count = step_decoder_->DecodeInt32();
    if(data_array->transactions_count == 0)
      data_array->transactions_count = WMDF_CONSTS_64BIT_NIL_VALUE;
  }
  if(FIND_TAG("387"))
  {
    data_array->volume = step_decoder_->DecodeInt32();
    if(data_array->volume == 0)
      data_array->volume = WMDF_CONSTS_64BIT_NIL_VALUE;
  }
  if(FIND_TAG("8504"))
  {
    data_array->total_amount = step_decoder_->DecodeDouble();
    if(data_array->total_amount == 0)
      data_array->total_amount = WMDF_CONSTS_DOUBLE_NIL_VALUE;
  }
  if(FIND_TAG("10043"))
    data_array->l2_total_bid_qty = step_decoder_->DecodeInt32();
  if(FIND_TAG("10116"))
    data_array->weighted_average_best_bid = step_decoder_->DecodeDouble();
  if(FIND_TAG("10044"))
    data_array->l2_total_offer_qty = step_decoder_->DecodeInt32();
  if(FIND_TAG("10117"))
    data_array->weighted_average_best_ask = step_decoder_->DecodeDouble();
  if(FIND_TAG("10057"))
  {
    data_array->peratio2 = step_decoder_->DecodeDouble();
    if(data_array->peratio2 == 0)
      data_array->peratio2 = WMDF_CONSTS_DOUBLE_NIL_VALUE;
  }
  if(FIND_TAG("10060"))
  {
    data_array->peratio2 = step_decoder_->DecodeDouble();	
    if(data_array->peratio2 == 0)
      data_array->peratio2 = WMDF_CONSTS_DOUBLE_NIL_VALUE;
  }

  WindStepMessage* msg = new WindStepMessage();
  msg->MsgType = SSE_MARKETDATA_1102_TYPE;
  msg->Data = data_array;
  return msg;
}
