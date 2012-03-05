#include "szse_step_marketdata_template.h"
#include "../common/step_codec.h"
#include "../common/rawdata_object.h"

using namespace std;

void ReleaseOrderQueue(wmdf::OrderQueueImage *orderqueue)
{
	wmdf::OrderQueueImage *temp = orderqueue;
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
}

wmdf::SZSEStepMarketDataTemplate::SZSEStepMarketDataTemplate()
{

}

wmdf::SZSEStepMarketDataTemplate::~SZSEStepMarketDataTemplate()
{
	for(map<string,CompareStruct*>::iterator iter = compare_map_.begin();iter!=compare_map_.end();++iter)
	{
		CompareStruct *temp = iter->second;
		delete temp;
		temp = NULL;
	}
	compare_map_.clear();
}


wmdf::WindStepMessage* wmdf::SZSEStepMarketDataTemplate::DecodeRaw( uint8_t* message_start,uint8_t* message_end )
{
  int ret=0;
	step_decoder_->ReSet((char*)message_start,(char*)message_end);
  bid_order_queue_msg_ = NULL;
  ask_order_queue_msg_ = NULL;
	char place_str[20];
	int32_t nobidpricelevel;
	int32_t noofferpricelevel;

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

	memset(data_array->wind_code,0,sizeof(data_array->wind_code));


	if(FIND_TAG("10178"))
		data_array->trade_time = step_decoder_->DecodeSZSETimeStamp();
	if(FIND_TAG("48"))
		step_decoder_->DecodeString(data_array->wind_code,sizeof(data_array->wind_code));

	if(FIND_TAG("140"))
	{
		data_array->pre_close = step_decoder_->DecodeDouble();
		if(data_array->pre_close == 0)
			data_array->pre_close = WMDF_CONSTS_DOUBLE_NIL_VALUE;
	}
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
		if(data_array->today_low > 999999 || data_array->today_low == 0)
			data_array->today_low = WMDF_CONSTS_DOUBLE_NIL_VALUE;
	}
	if(FIND_TAG("31"))
	{
		data_array->new_price = step_decoder_->DecodeDouble();
		if(data_array->new_price ==0)
			data_array->new_price = WMDF_CONSTS_DOUBLE_NIL_VALUE;
	}

	if(FIND_TAG("8503"))
		data_array->transactions_count = step_decoder_->DecodeInt32();
	if(FIND_TAG("387"))
		data_array->volume = step_decoder_->DecodeInt64();
	if(FIND_TAG("8504"))
		data_array->total_amount = step_decoder_->DecodeDouble();

	if(FIND_TAG("8506"))//±»ºöÂÔ
		int64_t totallongposition = step_decoder_->DecodeInt64();
	if(FIND_TAG("8524"))//±»ºöÂÔ
	{
		data_array->peratio1 = step_decoder_->DecodeDouble();
		if(data_array->peratio1 == 0)
			data_array->peratio1 = WMDF_CONSTS_DOUBLE_NIL_VALUE;
	}
	if(FIND_TAG("8525"))//±»ºöÂÔ
	{
		data_array->peratio2 = step_decoder_->DecodeDouble();
		if(data_array->peratio2 == 0)
			data_array->peratio2 = WMDF_CONSTS_DOUBLE_NIL_VALUE;
	}
	if(FIND_TAG("10004"))//±»ºöÂÔ
    std::string EndOfDayMaker = step_decoder_->DecodeString((char*)place_str,20);

	if(FIND_TAG("10044"))
		data_array->l2_total_offer_qty = step_decoder_->DecodeInt64();
	if(FIND_TAG("10040"))
		int64_t weightedavgofferpx = step_decoder_->DecodeInt64();


	if(FIND_TAG("10069"))//offer
	{
		noofferpricelevel = step_decoder_->DecodeInt32();
		for(int i=0; i!=noofferpricelevel; ++i)
		{
			if(FIND_TAG("133"))
				data_array->ask_price[i] = step_decoder_->DecodeDouble();
			if(FIND_TAG("135"))
				data_array->ask_size[i] = step_decoder_->DecodeInt32();
			int orders_total = FIND_TAG("10067")!=0?step_decoder_->DecodeInt32():0;
			int orders_count = FIND_TAG("73")!=0?step_decoder_->DecodeInt32():0;
			if(orders_total!=0&&orders_count!=0)
			{
				DecodeOrderQueue(data_array,orders_total,orders_count,2);
			}
		}
	}

 
  if(FIND_TAG("10043"))
    data_array->l2_total_offer_qty = step_decoder_->DecodeInt64();
  if(FIND_TAG("10039"))
    int64_t weightedavgbidpx = step_decoder_->DecodeInt64();

	if(FIND_TAG("10068"))  //bid
	{
		nobidpricelevel = step_decoder_->DecodeInt32();
		for(int i=0; i!=nobidpricelevel; ++i)
		{
			if(FIND_TAG("132"))
				data_array->bid_price[i] = step_decoder_->DecodeDouble();
			if(FIND_TAG("134"))
				data_array->bid_size[i] = step_decoder_->DecodeInt32();
			int orders_total = FIND_TAG("10067")!=0?step_decoder_->DecodeInt32():0;
			int orders_count = FIND_TAG("73")!=0?step_decoder_->DecodeInt32():0;
			if(orders_total != 0 && orders_count != 0)
			{
				DecodeOrderQueue(data_array,orders_total,orders_count,1);
			}
		}
	}

	map<string,CompareStruct*>::iterator iter = compare_map_.find(data_array->wind_code);
	if(iter == compare_map_.end())
	{
		CompareStruct *curr_data = new CompareStruct();
		curr_data->trade_time = data_array->trade_time;
		curr_data->new_price = data_array->new_price;
		curr_data->volume = data_array->volume;
		curr_data->peratio1 = data_array->peratio1;
		curr_data->peratio2 = data_array->peratio2;
		compare_map_.insert(make_pair(data_array->wind_code,curr_data));
	}
	else
	{
		CompareStruct *last_data = iter->second;
		if(data_array->trade_time != last_data->trade_time
			|| (data_array->new_price == data_array->new_price && data_array->new_price != last_data->new_price)
			|| data_array->volume != last_data->volume
			|| (data_array->peratio1 == data_array->peratio1 && data_array->peratio1 != last_data->peratio1)
			|| (data_array->peratio2 == data_array->peratio2 && data_array->peratio2 != last_data->peratio2))
		{
			last_data->trade_time = data_array->trade_time;
			last_data->new_price = data_array->new_price;
			last_data->volume = data_array->volume;
			last_data->peratio1 = data_array->peratio1;
			last_data->peratio2 = data_array->peratio2;
		}
		else
		{
			delete data_array;
			return NULL;
		}
	}

	WindStepMessage *msg = new WindStepMessage();
	msg->MsgType = SZSE_MARKETDATA_103_TYPE;
	msg->Data = data_array;
	return msg;

}

void wmdf::SZSEStepMarketDataTemplate::DecodeOrderQueue( MarketData* market_data,int32_t orders_total,int32_t orders_count,int32_t side )
{
	int ret=0;
	OrderQueueImage* orderqueue_item = new OrderQueueImage();
	orderqueue_item->image_status=0;
	orderqueue_item->date_time_stamp = market_data->trade_time;
	memcpy(orderqueue_item->wind_code,market_data->wind_code,sizeof(market_data->wind_code));
	orderqueue_item->price_level_count=1;
	orderqueue_item->side[0] = side;
	orderqueue_item->order_queue_levels = new OrderQueuePriceLevel[orderqueue_item->price_level_count];
	OrderQueuePriceLevel* current_item = orderqueue_item->order_queue_levels;
	current_item->orders_count=orders_count;
	current_item->orders_total=orders_total;
	current_item->price = market_data->new_price;
	OrderItem* item = new OrderItem[orders_count];
	current_item->order_queue_items = item;
	for(int j=0;j<orders_count;j++)
	{
		item[j].order_qty=FIND_TAG("38")!=0?step_decoder_->DecodeInt64():0;
	}

	map<string,CompareStruct*>::iterator iter = compare_map_.find(market_data->wind_code);
	if(iter != compare_map_.end())
	{
		CompareStruct *last_data = iter->second;
		if(last_data->trade_time == market_data->trade_time)
		{
			ReleaseOrderQueue(orderqueue_item);
			return;
		}
	}

	WindStepMessage* msg = new WindStepMessage();
	msg->MsgType = SZSE_ORDERQUEUE_TYPE;
	msg->Data = orderqueue_item;
  if (side == 1)  // bid
  {
    bid_order_queue_msg_ = msg;
  }
  else
  {
	  ask_order_queue_msg_ = msg;
  }
}


