#include "sse_fast_marketdata_3202_template.h"
#include "../common/rawdata_object.h"
// 
// 
// void ReleasePriceLevel(wmdf::PriceLevel_3202 *pricelevel ,int32_t count)
// {
// 	if(pricelevel != NULL)
// 	{
// 		for(int32_t i=0; i!=count; ++i)
// 		{
// 			if(pricelevel[i].no_order > 0)
// 				delete[] pricelevel[i].order_items;
// 		}
// 		delete[] pricelevel;
// 	}
// }
// 
// 
// void Reset(wmdf::MarketData_3202 &marketdata)
// {
// 	marketdata.alt_weighted_avg_bid_price = WMDF_CONSTS_DOUBLE_NIL_VALUE;
// 	marketdata.alt_weighted_avg_offer_price = WMDF_CONSTS_DOUBLE_NIL_VALUE;
// 	marketdata.bidlevels = NULL;
// 	marketdata.close_price = WMDF_CONSTS_DOUBLE_NIL_VALUE;
// 	marketdata.date_time_stamp = WMDF_CONSTS_32BIT_NIL_VALUE;
// 	marketdata.high_price = WMDF_CONSTS_DOUBLE_NIL_VALUE;
// 	marketdata.image_status = WMDF_CONSTS_32BIT_NIL_VALUE;
// 	memset(marketdata.instrument_status,0,sizeof(marketdata.instrument_status));
// 	marketdata.iopv = WMDF_CONSTS_DOUBLE_NIL_VALUE;
// 	marketdata.last_price = WMDF_CONSTS_DOUBLE_NIL_VALUE;
// 	marketdata.low_price = WMDF_CONSTS_DOUBLE_NIL_VALUE;
// 	marketdata.num_of_bid_level = WMDF_CONSTS_32BIT_NIL_VALUE;
// 	marketdata.num_of_offer_level = WMDF_CONSTS_32BIT_NIL_VALUE;
// 	marketdata.num_trades = WMDF_CONSTS_64BIT_NIL_VALUE;
// 	marketdata.offerlevels = NULL;
// 	marketdata.open_price = WMDF_CONSTS_DOUBLE_NIL_VALUE;
// 	marketdata.preclose_price = WMDF_CONSTS_DOUBLE_NIL_VALUE;
// 	marketdata.total_bid_qty = WMDF_CONSTS_64BIT_NIL_VALUE;
// 	marketdata.total_offer_qty = WMDF_CONSTS_64BIT_NIL_VALUE;
// 	marketdata.total_value_trade = WMDF_CONSTS_DOUBLE_NIL_VALUE;
// 	marketdata.total_volume_trade = WMDF_CONSTS_64BIT_NIL_VALUE;
// 	marketdata.total_warrant_exec_qty = WMDF_CONSTS_64BIT_NIL_VALUE;
// 	marketdata.warrant_lower_price = WMDF_CONSTS_DOUBLE_NIL_VALUE;
// 	marketdata.warrant_upper_price = WMDF_CONSTS_DOUBLE_NIL_VALUE;
// 	marketdata.weighted_avg_bid_price = WMDF_CONSTS_DOUBLE_NIL_VALUE;
// 	marketdata.weighted_avg_offer_price = WMDF_CONSTS_DOUBLE_NIL_VALUE;
// 	marketdata.yield_to_maturity = WMDF_CONSTS_DOUBLE_NIL_VALUE;
// 	memset(marketdata.windcode,0,sizeof(marketdata.windcode));
// }

wmdf::SSEFastMarketData3202Template::SSEFastMarketData3202Template()
:FastTemplate(item_count_)
{

}

wmdf::SSEFastMarketData3202Template::~SSEFastMarketData3202Template()
{

}
wmdf::WindFastMessage* wmdf::SSEFastMarketData3202Template::DecodeRaw(uint8_t* raw_data,uint32_t len)
{
  int data_status=0;
  int tid=0;
  fast_decoder_->ReSet((uint8_t*)raw_data,len);
  int capacity=64;
  int size=0;
  uint8_t priceLevelMap[56];
  uint8_t orderItemMap[56];
  uint8_t palce_str[20];
  MarketData_3202* data_array = new MarketData_3202[capacity];//做整体分配的方法
	for(int32_t i=0;i!=capacity;++i)
	{
		data_array[i].alt_weighted_avg_bid_price = WMDF_CONSTS_DOUBLE_NIL_VALUE;
		data_array[i].alt_weighted_avg_offer_price = WMDF_CONSTS_DOUBLE_NIL_VALUE;
		data_array[i].bidlevels = NULL;
		data_array[i].close_price = WMDF_CONSTS_DOUBLE_NIL_VALUE;
		data_array[i].date_time_stamp = WMDF_CONSTS_32BIT_NIL_VALUE;
		data_array[i].high_price = WMDF_CONSTS_DOUBLE_NIL_VALUE;
		data_array[i].image_status = WMDF_CONSTS_32BIT_NIL_VALUE;
		memset(data_array[i].instrument_status,0,sizeof(data_array->instrument_status));
		data_array[i].iopv = WMDF_CONSTS_DOUBLE_NIL_VALUE;
		data_array[i].last_price = WMDF_CONSTS_DOUBLE_NIL_VALUE;
		data_array[i].low_price = WMDF_CONSTS_DOUBLE_NIL_VALUE;
		data_array[i].num_of_bid_level = WMDF_CONSTS_32BIT_NIL_VALUE;
		data_array[i].num_of_offer_level = WMDF_CONSTS_32BIT_NIL_VALUE;
		data_array[i].num_trades = WMDF_CONSTS_64BIT_NIL_VALUE;
		data_array[i].offerlevels = NULL;
		data_array[i].open_price = WMDF_CONSTS_DOUBLE_NIL_VALUE;
		data_array[i].preclose_price = WMDF_CONSTS_DOUBLE_NIL_VALUE;
		data_array[i].total_bid_qty = WMDF_CONSTS_64BIT_NIL_VALUE;
		data_array[i].total_offer_qty = WMDF_CONSTS_64BIT_NIL_VALUE;
		data_array[i].total_value_trade = WMDF_CONSTS_DOUBLE_NIL_VALUE;
		data_array[i].total_volume_trade = WMDF_CONSTS_64BIT_NIL_VALUE;
		data_array[i].total_warrant_exec_qty = WMDF_CONSTS_64BIT_NIL_VALUE;
		data_array[i].warrant_lower_price = WMDF_CONSTS_DOUBLE_NIL_VALUE;
		data_array[i].warrant_upper_price = WMDF_CONSTS_DOUBLE_NIL_VALUE;
		data_array[i].weighted_avg_bid_price = WMDF_CONSTS_DOUBLE_NIL_VALUE;
		data_array[i].weighted_avg_offer_price = WMDF_CONSTS_DOUBLE_NIL_VALUE;
		data_array[i].yield_to_maturity = WMDF_CONSTS_DOUBLE_NIL_VALUE;
		memset(data_array[i].windcode,0,sizeof(data_array->windcode));
	}
  while(!fast_decoder_->Empty())
  {
    fast_decoder_->BeginNewMessage();
    tid = fast_decoder_->DecodeUInt32(0,DEFAULT);
    assert(tid==SSE_MARKETDATA_3202_TYPE);
    // constant 变量不需要
    if(size>=capacity)
    {
      MarketData_3202* temp = new MarketData_3202[capacity*2];
      memcpy(temp,data_array,sizeof(MarketData_3202)*capacity);
      capacity=capacity*2;
      delete[] data_array;
      data_array = temp;
			for(int32_t i=size;i!=capacity;++i)
			{
				data_array[i].alt_weighted_avg_bid_price = WMDF_CONSTS_DOUBLE_NIL_VALUE;
				data_array[i].alt_weighted_avg_offer_price = WMDF_CONSTS_DOUBLE_NIL_VALUE;
				data_array[i].bidlevels = NULL;
				data_array[i].close_price = WMDF_CONSTS_DOUBLE_NIL_VALUE;
				data_array[i].date_time_stamp = WMDF_CONSTS_32BIT_NIL_VALUE;
				data_array[i].high_price = WMDF_CONSTS_DOUBLE_NIL_VALUE;
				data_array[i].image_status = WMDF_CONSTS_32BIT_NIL_VALUE;
				memset(data_array[i].instrument_status,0,sizeof(data_array->instrument_status));
				data_array[i].iopv = WMDF_CONSTS_DOUBLE_NIL_VALUE;
				data_array[i].last_price = WMDF_CONSTS_DOUBLE_NIL_VALUE;
				data_array[i].low_price = WMDF_CONSTS_DOUBLE_NIL_VALUE;
				data_array[i].num_of_bid_level = WMDF_CONSTS_32BIT_NIL_VALUE;
				data_array[i].num_of_offer_level = WMDF_CONSTS_32BIT_NIL_VALUE;
				data_array[i].num_trades = WMDF_CONSTS_64BIT_NIL_VALUE;
				data_array[i].offerlevels = NULL;
				data_array[i].open_price = WMDF_CONSTS_DOUBLE_NIL_VALUE;
				data_array[i].preclose_price = WMDF_CONSTS_DOUBLE_NIL_VALUE;
				data_array[i].total_bid_qty = WMDF_CONSTS_64BIT_NIL_VALUE;
				data_array[i].total_offer_qty = WMDF_CONSTS_64BIT_NIL_VALUE;
				data_array[i].total_value_trade = WMDF_CONSTS_DOUBLE_NIL_VALUE;
				data_array[i].total_volume_trade = WMDF_CONSTS_64BIT_NIL_VALUE;
				data_array[i].total_warrant_exec_qty = WMDF_CONSTS_64BIT_NIL_VALUE;
				data_array[i].warrant_lower_price = WMDF_CONSTS_DOUBLE_NIL_VALUE;
				data_array[i].warrant_upper_price = WMDF_CONSTS_DOUBLE_NIL_VALUE;
				data_array[i].weighted_avg_bid_price = WMDF_CONSTS_DOUBLE_NIL_VALUE;
				data_array[i].weighted_avg_offer_price = WMDF_CONSTS_DOUBLE_NIL_VALUE;
				data_array[i].yield_to_maturity = WMDF_CONSTS_DOUBLE_NIL_VALUE;
				memset(data_array[i].windcode,0,sizeof(data_array->windcode));
			}
    }
    //读取数据
    
    int32_t date_time_stamp = fast_decoder_->DecodeInt32(1,COPY,PT_MANDATORY);
    data_status = fast_decoder_->DecodeInt32(2,DEFAULT);
    if (data_status < 0)
    {
      MarketData_3202& current_marketdata=data_array[size++];
      current_marketdata.date_time_stamp = date_time_stamp;
      fast_decoder_->DecodeString(3,NONE,(uint8_t*)current_marketdata.windcode,sizeof(current_marketdata.windcode));
      
			current_marketdata.image_status = fast_decoder_->DecodeInt32(4,NONE,PT_MANDATORY);

			int32_t pre_close = fast_decoder_->DecodeInt32(5,DEFAULT);
      current_marketdata.preclose_price= Utils::Divide(pre_close,WMDF_CONSTS_ONE_THOUSAND);

			int32_t open_price = fast_decoder_->DecodeInt32(6,DEFAULT);
			current_marketdata.open_price = Utils::Divide(open_price,WMDF_CONSTS_ONE_THOUSAND);

			int32_t high_price = fast_decoder_->DecodeInt32(7,DEFAULT);
			current_marketdata.high_price= Utils::Divide(high_price,WMDF_CONSTS_ONE_THOUSAND);

			int32_t low_price = fast_decoder_->DecodeInt32(8,DEFAULT);
			current_marketdata.low_price= Utils::Divide(low_price,WMDF_CONSTS_ONE_THOUSAND);

			int32_t last_price = fast_decoder_->DecodeInt32(9,DEFAULT);
			current_marketdata.last_price= Utils::Divide(last_price,WMDF_CONSTS_ONE_THOUSAND);

			int32_t close_price = fast_decoder_->DecodeInt32(10,DEFAULT);
			current_marketdata.close_price= Utils::Divide(close_price,WMDF_CONSTS_ONE_THOUSAND);

      fast_decoder_->DecodeString(11,DEFAULT,(uint8_t*)current_marketdata.instrument_status,sizeof(current_marketdata.instrument_status));
      
			int32_t num_trades = fast_decoder_->DecodeInt32(12,DEFAULT);
			if(num_trades!=WMDF_CONSTS_32BIT_NIL_VALUE)
				current_marketdata.num_trades = num_trades;

			int64_t total_volume_trade = fast_decoder_->DecodeInt64(13,DEFAULT);
			current_marketdata.total_volume_trade = Utils::DivideEx(total_volume_trade,WMDF_CONSTS_ONE_THOUSAND);


			int64_t total_value_trade = fast_decoder_->DecodeInt64(14,DEFAULT);
			current_marketdata.total_value_trade= Utils::Divide(total_value_trade,WMDF_CONSTS_HUNDRED_THOUSAND);

			int64_t total_bid_qty = fast_decoder_->DecodeInt64(15,DEFAULT);
			current_marketdata.total_bid_qty= Utils::DivideEx(total_bid_qty,WMDF_CONSTS_ONE_THOUSAND);

			int32_t weighted_avg_bid_price = fast_decoder_->DecodeInt32(16,DEFAULT);
				current_marketdata.weighted_avg_bid_price= Utils::Divide(weighted_avg_bid_price,WMDF_CONSTS_ONE_THOUSAND);

			int32_t alt_weighted_avg_bid_price = fast_decoder_->DecodeInt32(17,DEFAULT);
			current_marketdata.alt_weighted_avg_bid_price= Utils::Divide(alt_weighted_avg_bid_price,WMDF_CONSTS_ONE_THOUSAND);

			int64_t total_offer_qty = fast_decoder_->DecodeInt64(18,DEFAULT);
			current_marketdata.total_offer_qty= Utils::DivideEx(total_offer_qty,WMDF_CONSTS_ONE_THOUSAND);

			int32_t weighted_avg_offer_price = fast_decoder_->DecodeInt32(19,DEFAULT);
			current_marketdata.weighted_avg_offer_price= Utils::Divide(weighted_avg_offer_price,WMDF_CONSTS_ONE_THOUSAND);

			int32_t alt_weighted_avg_offer_price = fast_decoder_->DecodeInt32(20,DEFAULT);
			current_marketdata.alt_weighted_avg_offer_price= Utils::Divide(alt_weighted_avg_offer_price,WMDF_CONSTS_ONE_THOUSAND);

			int32_t iopv = fast_decoder_->DecodeInt32(21,DEFAULT);
			current_marketdata.iopv = Utils::Divide(iopv,WMDF_CONSTS_ONE_THOUSAND);

			int32_t yield_to_maturity = fast_decoder_->DecodeInt32(22,DEFAULT);
			current_marketdata.yield_to_maturity = Utils::Divide(yield_to_maturity,WMDF_CONSTS_TEN_THOUSAND);

			int64_t total_warrant_exec_qty = fast_decoder_->DecodeInt64(23,DEFAULT);
			current_marketdata.total_warrant_exec_qty = Utils::DivideEx(total_warrant_exec_qty,WMDF_CONSTS_ONE_THOUSAND);

			int64_t warrant_lower_price = fast_decoder_->DecodeInt64(24,DEFAULT);
			current_marketdata.warrant_lower_price = Utils::Divide(warrant_lower_price,WMDF_CONSTS_ONE_THOUSAND);

			int64_t warrant_upper_price = fast_decoder_->DecodeInt64(25,DEFAULT);
			current_marketdata.warrant_upper_price= Utils::Divide(warrant_upper_price,WMDF_CONSTS_ONE_THOUSAND);

      int no_bid_level = fast_decoder_->DecodeInt32(26,NONE);
      current_marketdata.num_of_bid_level = no_bid_level;
      if(no_bid_level>0)
      {
        current_marketdata.bidlevels = new PriceLevel_3202[no_bid_level];
        for(int i=0;i<no_bid_level;i++)
        {
          PriceLevel_3202* price_level_ptr = &current_marketdata.bidlevels[i];
          //使用pmap
          fast_decoder_->BeginSqueuce(priceLevelMap,sizeof(priceLevelMap));
          price_level_ptr->price_level_operator = fast_decoder_->DecodeInt32(0,DEFAULT);
					price_level_ptr->price = Utils::Divide(fast_decoder_->DecodeInt32(1,DEFAULT),WMDF_CONSTS_ONE_THOUSAND);
          price_level_ptr->order_qty = Utils::DivideEx2(fast_decoder_->DecodeInt64(2,DEFAULT),WMDF_CONSTS_ONE_THOUSAND);
          price_level_ptr->num_order = fast_decoder_->DecodeInt32(3,DEFAULT);
          price_level_ptr->no_order = fast_decoder_->DecodeInt32(4,NONE);
          fast_decoder_->EndSqueuce();
          if(price_level_ptr->no_order >0)
          {
            price_level_ptr->order_items = new OrderItem[price_level_ptr->no_order];
            for(int j=0;j<price_level_ptr->no_order ;j++)
            {
              OrderItem* order_item_ptr = &price_level_ptr->order_items[j];
              fast_decoder_->BeginSqueuce(orderItemMap,sizeof(orderItemMap));
              order_item_ptr->order_queue_operator = fast_decoder_->DecodeInt32(0,DEFAULT);
              order_item_ptr->order_queue_operator_entry_id = fast_decoder_->DecodeInt32(1,DEFAULT);
              int64_t order_qty = fast_decoder_->DecodeInt64(2,DEFAULT);
              order_item_ptr->order_qty = Utils::DivideEx2(order_qty,WMDF_CONSTS_ONE_THOUSAND);
              fast_decoder_->EndSqueuce();
            }
          }
          else
          {
            price_level_ptr->order_items = NULL;
          }
        }
      }
      else
      {
        current_marketdata.bidlevels = NULL;
      }

      int32_t no_offer_level = fast_decoder_->DecodeInt32(27,NONE);
      current_marketdata.num_of_offer_level = no_offer_level;
      if(no_offer_level>0)
      {
        current_marketdata.offerlevels = new PriceLevel_3202[no_offer_level];
        for(int32_t i=0;i<no_offer_level;i++)
        {
          PriceLevel_3202* price_level_ptr = &current_marketdata.offerlevels[i];
          fast_decoder_->BeginSqueuce(priceLevelMap,sizeof(priceLevelMap));
          price_level_ptr->price_level_operator = fast_decoder_->DecodeInt32(0,DEFAULT);
					price_level_ptr->price = Utils::Divide(fast_decoder_->DecodeInt32(1,DEFAULT),WMDF_CONSTS_ONE_THOUSAND);
          price_level_ptr->order_qty = Utils::DivideEx2(fast_decoder_->DecodeInt64(2,DEFAULT),WMDF_CONSTS_ONE_THOUSAND);
          price_level_ptr->num_order = fast_decoder_->DecodeInt32(3,DEFAULT);
          price_level_ptr->no_order = fast_decoder_->DecodeInt32(4,NONE);
          fast_decoder_->EndSqueuce();
          if(price_level_ptr->no_order >0)
          {
            price_level_ptr->order_items = new OrderItem[price_level_ptr->no_order];
            for(int32_t j=0;j<price_level_ptr->no_order ;j++)
            {
              OrderItem* order_item_ptr = &price_level_ptr->order_items[j];
              fast_decoder_->BeginSqueuce(orderItemMap,sizeof(orderItemMap));
              order_item_ptr->order_queue_operator = fast_decoder_->DecodeInt32(0,DEFAULT);
              order_item_ptr->order_queue_operator_entry_id = fast_decoder_->DecodeInt32(1,DEFAULT);
              order_item_ptr->order_qty = Utils::DivideEx2(fast_decoder_->DecodeInt64(2,DEFAULT),WMDF_CONSTS_ONE_THOUSAND);
              fast_decoder_->EndSqueuce();
            }
          }
          else
          {
            price_level_ptr->order_items = NULL;
          }
        }
      }
      else
      {
        current_marketdata.offerlevels = NULL;
      }

// 			if(current_marketdata.date_time_stamp > 112959 && current_marketdata.date_time_stamp <130000)
// 			{
// 				ReleasePriceLevel(current_marketdata.bidlevels,current_marketdata.num_of_bid_level);
// 				ReleasePriceLevel(current_marketdata.offerlevels,current_marketdata.num_of_offer_level);
// 				Reset(current_marketdata);
// 				size--;
// 			}

    }
    else  // 无效值
    {
      fast_decoder_->DecodeString(2,NONE,(uint8_t*)palce_str,sizeof(palce_str));
      fast_decoder_->DecodeInt32(3,NONE);
      fast_decoder_->DecodeInt32(4,DEFAULT);
      fast_decoder_->DecodeInt32(5,DEFAULT);
      fast_decoder_->DecodeInt32(6,DEFAULT);
      fast_decoder_->DecodeInt32(7,DEFAULT);
      fast_decoder_->DecodeInt32(8,DEFAULT);
      fast_decoder_->DecodeInt32(9,DEFAULT);
      fast_decoder_->DecodeString(10,DEFAULT,(uint8_t*)palce_str,sizeof(palce_str));
      fast_decoder_->DecodeInt32(11,DEFAULT);
      fast_decoder_->DecodeInt64(12,DEFAULT);
      fast_decoder_->DecodeInt64(13,DEFAULT);
      fast_decoder_->DecodeInt64(14,DEFAULT);
      fast_decoder_->DecodeInt32(15,DEFAULT);
      fast_decoder_->DecodeInt32(16,DEFAULT);

      fast_decoder_->DecodeInt64(17,DEFAULT);
      fast_decoder_->DecodeInt32(18,DEFAULT);
      fast_decoder_->DecodeInt32(19,DEFAULT);

      fast_decoder_->DecodeInt32(20,DEFAULT);
      fast_decoder_->DecodeInt32(21,DEFAULT);
      fast_decoder_->DecodeInt64(22,DEFAULT);
      fast_decoder_->DecodeInt64(23,DEFAULT);
      fast_decoder_->DecodeInt64(24,DEFAULT);

      int32_t no_bid_level = fast_decoder_->DecodeInt32(25,NONE);
      for(int32_t i=0;i<no_bid_level;i++)
      {

        fast_decoder_->BeginSqueuce(priceLevelMap,sizeof(priceLevelMap));
        fast_decoder_->DecodeInt32(0,DEFAULT);
        fast_decoder_->DecodeInt32(1,DEFAULT);
        fast_decoder_->DecodeInt64(2,DEFAULT);
        fast_decoder_->DecodeInt32(3,DEFAULT);
        int32_t no_orders = fast_decoder_->DecodeInt32(4,NONE);
        fast_decoder_->EndSqueuce();
        for(int32_t j=0;j<no_orders;j++)
        {

          fast_decoder_->BeginSqueuce(orderItemMap,sizeof(orderItemMap));
          fast_decoder_->DecodeInt32(0,DEFAULT);
          fast_decoder_->DecodeInt32(1,DEFAULT);
          fast_decoder_->DecodeInt64(2,DEFAULT);
          fast_decoder_->EndSqueuce();
        }
      }

      int32_t no_offer_level = fast_decoder_->DecodeInt32(26,NONE);
      for(int32_t i=0;i<no_offer_level;i++)
      {
        fast_decoder_->BeginSqueuce(priceLevelMap,sizeof(priceLevelMap));
        fast_decoder_->DecodeInt32(0,DEFAULT);
        fast_decoder_->DecodeInt32(1,DEFAULT);
        fast_decoder_->DecodeInt64(2,DEFAULT);
        fast_decoder_->DecodeInt32(3,DEFAULT);
        int32_t no_orders = fast_decoder_->DecodeInt32(4,NONE);
        fast_decoder_->EndSqueuce();
        for(int j=0;j<no_orders ;j++)
        {
          fast_decoder_->BeginSqueuce(orderItemMap,sizeof(orderItemMap));
          fast_decoder_->DecodeInt32(0,DEFAULT);
          fast_decoder_->DecodeInt32(1,DEFAULT);
          fast_decoder_->DecodeInt64(2,DEFAULT);
          fast_decoder_->EndSqueuce();
        }
      }
    }
  }
  if (size>0)
  {
    WindFastMessage* msg = new WindFastMessage();
    msg->MsgType = SSE_MARKETDATA_3202_TYPE;
    msg->Size=size;
    msg->Data = data_array;
    return msg;
  }
  else
  {
    delete data_array;
  }
  return NULL;
  
}
