#include "sse_step_virtualauctionprice_template.h"


wmdf::SSEStepVirtualAuctionPriceTemplate::SSEStepVirtualAuctionPriceTemplate()
{

}


wmdf::SSEStepVirtualAuctionPriceTemplate::~SSEStepVirtualAuctionPriceTemplate()
{

}

wmdf::WindStepMessage* wmdf::SSEStepVirtualAuctionPriceTemplate::DecodeRaw( uint8_t* message_start,uint8_t* message_end )
{
	int ret = 0;
	step_decoder_->ReSet((char*)message_start,(char*)message_end);
	MarketData* data_array = new MarketData();
	for(size_t i=0; i!=10; ++i)
	{
		data_array->ask_price[i] = WMDF_CONSTS_DOUBLE_NIL_VALUE;
		data_array->ask_size[i] = WMDF_CONSTS_32BIT_NIL_VALUE;
		data_array->bid_price[i] = WMDF_CONSTS_DOUBLE_NIL_VALUE;
		data_array->bid_size[i] = WMDF_CONSTS_32BIT_NIL_VALUE;
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
	data_array->peratio2 = WMDF_CONSTS_DOUBLE_NIL_VALUE;
	memset(data_array->wind_code,0,sizeof(data_array->wind_code));

	double price = WMDF_CONSTS_DOUBLE_NIL_VALUE;
	int32_t virtualauctionqty = WMDF_CONSTS_32BIT_NIL_VALUE;
	int32_t bidleaveqty = WMDF_CONSTS_32BIT_NIL_VALUE;
	int32_t askleaveqty = WMDF_CONSTS_32BIT_NIL_VALUE;

	if(FIND_TAG("10072"))
		step_decoder_->DecodeInt32();
	if(FIND_TAG("10178"))
		data_array->trade_time = step_decoder_->DecodeInt32();
  if(FIND_TAG("10121"))
  {
    delete data_array;
    return NULL;
  }  
	if(FIND_TAG("48"))
		step_decoder_->DecodeString(data_array->wind_code,sizeof(data_array->wind_code));
	if(FIND_TAG("44"))
		price = step_decoder_->DecodeDouble();
	if(FIND_TAG("10127"))
		virtualauctionqty = step_decoder_->DecodeInt32();
	if(FIND_TAG("10125"))
		bidleaveqty = step_decoder_->DecodeInt32();
	if(FIND_TAG("10126"))
		askleaveqty = step_decoder_->DecodeInt32();

	if (bidleaveqty != WMDF_CONSTS_32BIT_NIL_VALUE )
	{
		data_array->bid_price[0] = price;
		data_array->bid_price[1] = 0;
    data_array->ask_price[0] = 0;
    data_array->ask_price[1] = 0;
		data_array->bid_size[0] = virtualauctionqty;
		data_array->bid_size[1] = bidleaveqty;
	}
	else if(askleaveqty != WMDF_CONSTS_32BIT_NIL_VALUE )
	{
		data_array->ask_price[0] = price;
		data_array->ask_price[1] = 0;
    data_array->bid_price[0] = 0;
    data_array->bid_price[1] = 0;
		data_array->ask_size[0] = virtualauctionqty;
		data_array->ask_size[1] = askleaveqty;
	}

	WindStepMessage* msg = new WindStepMessage();
	msg->MsgType = SSE_MARKETDATA_1102_TYPE;
	msg->Data = data_array;
	return msg;
}

