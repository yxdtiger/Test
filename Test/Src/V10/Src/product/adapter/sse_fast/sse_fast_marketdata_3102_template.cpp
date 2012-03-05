#include "sse_fast_marketdata_3102_template.h"
#include "../common/rawdata_object.h"


wmdf::SSEFastMarketData3102Template::SSEFastMarketData3102Template()
:FastTemplate(item_count_)
{

}

wmdf::SSEFastMarketData3102Template::~SSEFastMarketData3102Template()
{

}

wmdf::WindFastMessage* wmdf::SSEFastMarketData3102Template::DecodeRaw(uint8_t* raw_data,uint32_t len)
{
	int data_status=0,data_time_stamp=0;
	int tid=0;
	fast_decoder_->ReSet(raw_data,len);
	int capacity=64;
	int size=0;
	uint8_t palce_str[20];
	uint64_t temp=0;
	MarketData* data_array = new MarketData[capacity];//做整体分配的方法	
	for(int32_t i=0;i!=capacity;++i)
	{
		for(int32_t j=0; j!=10; ++j)
		{
			data_array[i].ask_price[j] = 0;
			data_array[i].ask_size[j] = 0;
			data_array[i].bid_price[j] = 0;
			data_array[i].bid_size[j] = 0;
		}
		data_array[i].l2_total_bid_qty = WMDF_CONSTS_64BIT_NIL_VALUE;
		data_array[i].l2_total_offer_qty = WMDF_CONSTS_64BIT_NIL_VALUE;
		data_array[i].new_price = WMDF_CONSTS_DOUBLE_NIL_VALUE;
		data_array[i].pre_close = WMDF_CONSTS_DOUBLE_NIL_VALUE;
		data_array[i].today_high = WMDF_CONSTS_DOUBLE_NIL_VALUE;
		data_array[i].today_low = WMDF_CONSTS_DOUBLE_NIL_VALUE;
		data_array[i].today_open = WMDF_CONSTS_DOUBLE_NIL_VALUE;
		data_array[i].total_amount = WMDF_CONSTS_DOUBLE_NIL_VALUE;
		data_array[i].trade_date = WMDF_CONSTS_32BIT_NIL_VALUE;
		data_array[i].trade_time = WMDF_CONSTS_32BIT_NIL_VALUE;
		data_array[i].transactions_count = WMDF_CONSTS_64BIT_NIL_VALUE;
		data_array[i].volume = WMDF_CONSTS_64BIT_NIL_VALUE;
		data_array[i].weighted_average_best_ask = WMDF_CONSTS_DOUBLE_NIL_VALUE;
		data_array[i].weighted_average_best_bid = WMDF_CONSTS_DOUBLE_NIL_VALUE;
		data_array[i].peratio1 = WMDF_CONSTS_DOUBLE_NIL_VALUE;
		data_array[i].peratio2 = WMDF_CONSTS_DOUBLE_NIL_VALUE;
		data_array[i].trade_status = WMDF_CONSTS_8BIT_NIL_VALUE;
		memset(data_array[i].wind_code,0,sizeof(data_array->wind_code));
	}
	while(!fast_decoder_->Empty())
	{
		fast_decoder_->BeginNewMessage();
		tid = fast_decoder_->DecodeUInt32(0,DEFAULT);
		assert(tid==SSE_MARKETDATA_3102_TYPE);
		data_time_stamp = fast_decoder_->DecodeInt32(1,COPY,PT_MANDATORY);
		data_status=fast_decoder_->DecodeInt32(2,DEFAULT);
		if(data_status<=0)//数据有效
		{
			// constant 变量不需要
			if(size>=capacity)
			{
				MarketData* temp = new MarketData[capacity*2];
				memcpy(temp,data_array,sizeof(MarketData)*capacity);
				capacity=capacity*2;
				delete[] data_array;
				data_array = temp;
				for(int32_t i=size;i!=capacity;++i)
				{
					for(int32_t j=0; j!=10; ++j)
					{
						data_array[i].ask_price[j] = 0;
						data_array[i].ask_size[j] = 0;
						data_array[i].bid_price[j] = 0;
						data_array[i].bid_size[j] = 0;
					}
					data_array[i].l2_total_bid_qty = WMDF_CONSTS_64BIT_NIL_VALUE;
					data_array[i].l2_total_offer_qty = WMDF_CONSTS_64BIT_NIL_VALUE;
					data_array[i].new_price = WMDF_CONSTS_DOUBLE_NIL_VALUE;
					data_array[i].pre_close = WMDF_CONSTS_DOUBLE_NIL_VALUE;
					data_array[i].today_high = WMDF_CONSTS_DOUBLE_NIL_VALUE;
					data_array[i].today_low = WMDF_CONSTS_DOUBLE_NIL_VALUE;
					data_array[i].today_open = WMDF_CONSTS_DOUBLE_NIL_VALUE;
					data_array[i].total_amount = WMDF_CONSTS_DOUBLE_NIL_VALUE;
					data_array[i].trade_date = WMDF_CONSTS_32BIT_NIL_VALUE;
					data_array[i].trade_time = WMDF_CONSTS_32BIT_NIL_VALUE;
					data_array[i].transactions_count = WMDF_CONSTS_64BIT_NIL_VALUE;
					data_array[i].volume = WMDF_CONSTS_64BIT_NIL_VALUE;
					data_array[i].weighted_average_best_ask = WMDF_CONSTS_DOUBLE_NIL_VALUE;
					data_array[i].weighted_average_best_bid = WMDF_CONSTS_DOUBLE_NIL_VALUE;
					data_array[i].peratio1 = WMDF_CONSTS_DOUBLE_NIL_VALUE;
					data_array[i].peratio2 = WMDF_CONSTS_DOUBLE_NIL_VALUE;
					data_array[i].trade_status = WMDF_CONSTS_8BIT_NIL_VALUE;
					memset(data_array[i].wind_code,0,sizeof(data_array->wind_code));
				}
			}	
			//读取数据
			MarketData& curr_market_data = data_array[size++];
			//存放数据
			curr_market_data.trade_time= data_time_stamp*1000;
			fast_decoder_->DecodeString(3,NONE,(uint8_t*)curr_market_data.wind_code,sizeof(curr_market_data.wind_code));
			
			int32_t preclose = fast_decoder_->DecodeInt32(4,DEFAULT);
			curr_market_data.pre_close = Utils::Divide(preclose,WMDF_CONSTS_ONE_THOUSAND);

			int32_t todayopen = fast_decoder_->DecodeInt32(5,DEFAULT);
			curr_market_data.today_open=Utils::Divide(todayopen,WMDF_CONSTS_ONE_THOUSAND);

			int32_t todayhigh = fast_decoder_->DecodeInt32(6,DEFAULT);
			curr_market_data.today_high=Utils::Divide(todayhigh,WMDF_CONSTS_ONE_THOUSAND);

			int32_t todaylow = fast_decoder_->DecodeInt32(7,DEFAULT);
			curr_market_data.today_low = Utils::Divide(todaylow,WMDF_CONSTS_ONE_THOUSAND);

			int32_t newprice = fast_decoder_->DecodeInt32(8,DEFAULT);
			curr_market_data.new_price = Utils::Divide(newprice,WMDF_CONSTS_ONE_THOUSAND);

			uint8_t InstrumentStatus[8];
			fast_decoder_->DecodeString(9,DEFAULT,InstrumentStatus,sizeof(InstrumentStatus));

			int32_t BidPriceLevelCount;
			BidPriceLevelCount = fast_decoder_->DecodeInt32(10,NONE);

			if(BidPriceLevelCount>0)
			{
				for (int i=0;i<BidPriceLevelCount;i++)
				{
					curr_market_data.bid_size[i] = Utils::DivideEx2(fast_decoder_->DecodeInt64(10,NONE,PT_MANDATORY),WMDF_CONSTS_ONE_THOUSAND);
					curr_market_data.bid_price[i] = Utils::Divide(fast_decoder_->DecodeInt32(10,NONE,PT_MANDATORY),WMDF_CONSTS_ONE_THOUSAND);
				}
			}

			int32_t OfferPriceLevelsCount = fast_decoder_->DecodeInt32(10,NONE);

			if(OfferPriceLevelsCount>0)
			{
				for (int i=0;i<OfferPriceLevelsCount;i++)
				{
					curr_market_data.ask_size[i] = Utils::DivideEx2(fast_decoder_->DecodeInt64(10,NONE,PT_MANDATORY),WMDF_CONSTS_ONE_THOUSAND);
					curr_market_data.ask_price[i] = Utils::Divide(fast_decoder_->DecodeInt32(10,NONE,PT_MANDATORY),WMDF_CONSTS_ONE_THOUSAND);
				}
			}

      int32_t num_trades = fast_decoder_->DecodeInt32(12,DEFAULT);
      if(num_trades != WMDF_CONSTS_32BIT_NIL_VALUE)
        curr_market_data.transactions_count = num_trades;

			int64_t volume = fast_decoder_->DecodeInt64(13,DEFAULT);
			curr_market_data.volume = Utils::DivideEx(volume,WMDF_CONSTS_ONE_THOUSAND);

			int64_t total_amount = fast_decoder_->DecodeInt64(14,DEFAULT);
			curr_market_data.total_amount = Utils::Divide(total_amount,WMDF_CONSTS_HUNDRED_THOUSAND);

			int64_t l2_total_bid_qty = fast_decoder_->DecodeInt64(15,DEFAULT);
			curr_market_data.l2_total_bid_qty = Utils::DivideEx(l2_total_bid_qty,WMDF_CONSTS_ONE_THOUSAND);
			
			int32_t weighted_average_best_bid = fast_decoder_->DecodeInt32(16,DEFAULT);
			curr_market_data.weighted_average_best_bid = Utils::Divide(weighted_average_best_bid,WMDF_CONSTS_ONE_THOUSAND);

			double AltWeightedAvgBidPx=Utils::Divide(fast_decoder_->DecodeInt32(17,DEFAULT),WMDF_CONSTS_ONE_THOUSAND);

			int64_t l2_total_offer_qty = fast_decoder_->DecodeInt64(18,DEFAULT);
			curr_market_data.l2_total_offer_qty=Utils::DivideEx(l2_total_offer_qty,WMDF_CONSTS_ONE_THOUSAND);

			int32_t weighted_average_best_ask = fast_decoder_->DecodeInt32(19,DEFAULT);
			curr_market_data.weighted_average_best_ask = Utils::Divide(weighted_average_best_ask,WMDF_CONSTS_ONE_THOUSAND);

			double AltWeightedAvgOfferPx=Utils::Divide(fast_decoder_->DecodeInt32(20,DEFAULT),WMDF_CONSTS_ONE_THOUSAND);
			
			int64_t peratio2 = fast_decoder_->DecodeInt64(21,DEFAULT);
			if(peratio2>0)
      {
				curr_market_data.peratio2 = Utils::Divide(peratio2,WMDF_CONSTS_ONE_THOUSAND);
      }

			peratio2 = fast_decoder_->DecodeInt64(22,DEFAULT);
			if(peratio2>0)
				curr_market_data.peratio2 = Utils::Divide(peratio2,WMDF_CONSTS_TEN_THOUSAND);

			double TotalWarrantExecQty= Utils::Divide(fast_decoder_->DecodeInt64(23,DEFAULT),WMDF_CONSTS_ONE_THOUSAND);
			double WarLowerPx=Utils::Divide(fast_decoder_->DecodeInt64(24,DEFAULT),WMDF_CONSTS_ONE_THOUSAND);
			double WarUpperPx=Utils::Divide(fast_decoder_->DecodeInt64(25,DEFAULT),WMDF_CONSTS_ONE_THOUSAND);
		}else//数据无效
		{
			fast_decoder_->DecodeString(3,NONE,palce_str,sizeof(palce_str));
			fast_decoder_->DecodeInt32(4,DEFAULT) ;
			fast_decoder_->DecodeInt32(5,DEFAULT) ;
			fast_decoder_->DecodeInt32(6,DEFAULT) ;
			fast_decoder_->DecodeInt32(7,DEFAULT) ;
			fast_decoder_->DecodeInt32(8,DEFAULT) ;
			fast_decoder_->DecodeString(9,DEFAULT,palce_str,sizeof(palce_str));
			int count = fast_decoder_->DecodeInt32(10,NONE);		
			for (int i=1;i<count;i++)
			{
				fast_decoder_->DecodeInt64(10,NONE,PT_MANDATORY) ;
				fast_decoder_->DecodeInt32(10,NONE,PT_MANDATORY) ;
			}
			count = fast_decoder_->DecodeInt32(10,NONE);
			for (int i=1;i<count;i++)
			{
				fast_decoder_->DecodeInt64(10,NONE,PT_MANDATORY);
				fast_decoder_->DecodeInt32(10,NONE,PT_MANDATORY);
			}
			fast_decoder_->DecodeInt32(12,DEFAULT);
			fast_decoder_->DecodeInt64(13,DEFAULT);
			fast_decoder_->DecodeInt64(14,DEFAULT);
			fast_decoder_->DecodeInt64(15,DEFAULT);
			fast_decoder_->DecodeInt32(16,DEFAULT);
			fast_decoder_->DecodeInt32(17,DEFAULT);
			fast_decoder_->DecodeInt64(18,DEFAULT);
			fast_decoder_->DecodeInt32(19,DEFAULT);
			fast_decoder_->DecodeInt32(20,DEFAULT);
			fast_decoder_->DecodeInt64(21,DEFAULT);
			fast_decoder_->DecodeInt64(22,DEFAULT);
			fast_decoder_->DecodeInt64(23,DEFAULT);
			fast_decoder_->DecodeInt64(24,DEFAULT);
			fast_decoder_->DecodeInt64(25,DEFAULT);
		}

	}

  if (size > 0)
  {
    WindFastMessage* msg = new WindFastMessage();
    msg->MsgType = SSE_MARKETDATA_3102_TYPE;
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
