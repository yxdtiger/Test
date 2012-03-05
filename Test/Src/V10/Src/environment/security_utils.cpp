#include "security_utils.h"
#include <cstring>


void SecurityUtils::initialize_l1_market( L1MarketSnapshot *marketsnapshot, size_t n )
{
	size_t size_of_windcode = sizeof(marketsnapshot->wind_code);
	for(size_t i=0; i!=n; ++i)
	{
		memset(marketsnapshot[i].wind_code,0,size_of_windcode);
		marketsnapshot[i].data_time_stamp=WMDF_CONSTS_32BIT_NIL_VALUE;
		marketsnapshot[i].pre_close=WMDF_CONSTS_DOUBLE_NIL_VALUE;
		marketsnapshot[i].open_price=WMDF_CONSTS_DOUBLE_NIL_VALUE;
		marketsnapshot[i].high_price=WMDF_CONSTS_DOUBLE_NIL_VALUE;
		marketsnapshot[i].low_price=WMDF_CONSTS_DOUBLE_NIL_VALUE;
		marketsnapshot[i].new_price=WMDF_CONSTS_DOUBLE_NIL_VALUE;
		marketsnapshot[i].total_volume=WMDF_CONSTS_64BIT_NIL_VALUE;
		marketsnapshot[i].total_amount=WMDF_CONSTS_DOUBLE_NIL_VALUE;
		marketsnapshot[i].bid_price1=WMDF_CONSTS_DOUBLE_NIL_VALUE;
		marketsnapshot[i].bid_price2=WMDF_CONSTS_DOUBLE_NIL_VALUE;
		marketsnapshot[i].bid_price3=WMDF_CONSTS_DOUBLE_NIL_VALUE;
		marketsnapshot[i].bid_price4=WMDF_CONSTS_DOUBLE_NIL_VALUE;
		marketsnapshot[i].bid_price5=WMDF_CONSTS_DOUBLE_NIL_VALUE;
		marketsnapshot[i].ask_price1=WMDF_CONSTS_DOUBLE_NIL_VALUE;
		marketsnapshot[i].ask_price2=WMDF_CONSTS_DOUBLE_NIL_VALUE;
		marketsnapshot[i].ask_price3=WMDF_CONSTS_DOUBLE_NIL_VALUE;
		marketsnapshot[i].ask_price4=WMDF_CONSTS_DOUBLE_NIL_VALUE;
		marketsnapshot[i].ask_price5=WMDF_CONSTS_DOUBLE_NIL_VALUE;
		marketsnapshot[i].ask_size1=WMDF_CONSTS_32BIT_NIL_VALUE;
		marketsnapshot[i].ask_size2=WMDF_CONSTS_32BIT_NIL_VALUE;
		marketsnapshot[i].ask_size3=WMDF_CONSTS_32BIT_NIL_VALUE;
		marketsnapshot[i].ask_size4=WMDF_CONSTS_32BIT_NIL_VALUE;
		marketsnapshot[i].ask_size5=WMDF_CONSTS_32BIT_NIL_VALUE;
		marketsnapshot[i].bid_size1=WMDF_CONSTS_32BIT_NIL_VALUE;
		marketsnapshot[i].bid_size2=WMDF_CONSTS_32BIT_NIL_VALUE;
		marketsnapshot[i].bid_size3=WMDF_CONSTS_32BIT_NIL_VALUE;
		marketsnapshot[i].bid_size4=WMDF_CONSTS_32BIT_NIL_VALUE;
		marketsnapshot[i].bid_size5=WMDF_CONSTS_32BIT_NIL_VALUE;
	}
}

void SecurityUtils::initialize_l2_market( L2MarketSnapshot *marketsnapshot, size_t n )
{
	size_t size_of_windcode = sizeof(marketsnapshot->wind_code);
	for(size_t i=0;i!=n;++i)
  {
		memset(marketsnapshot[i].wind_code,0,size_of_windcode);
    marketsnapshot[i].data_time_stamp=WMDF_CONSTS_32BIT_NIL_VALUE;
    marketsnapshot[i].pre_close=WMDF_CONSTS_DOUBLE_NIL_VALUE;
    marketsnapshot[i].open_price=WMDF_CONSTS_DOUBLE_NIL_VALUE;
    marketsnapshot[i].high_price=WMDF_CONSTS_DOUBLE_NIL_VALUE;
    marketsnapshot[i].low_price=WMDF_CONSTS_DOUBLE_NIL_VALUE;
    marketsnapshot[i].new_price=WMDF_CONSTS_DOUBLE_NIL_VALUE;
    marketsnapshot[i].total_volume=WMDF_CONSTS_64BIT_NIL_VALUE;
    marketsnapshot[i].total_amount=WMDF_CONSTS_DOUBLE_NIL_VALUE;
    marketsnapshot[i].bid_price1=WMDF_CONSTS_DOUBLE_NIL_VALUE;
    marketsnapshot[i].bid_price2=WMDF_CONSTS_DOUBLE_NIL_VALUE;
    marketsnapshot[i].bid_price3=WMDF_CONSTS_DOUBLE_NIL_VALUE;
    marketsnapshot[i].bid_price4=WMDF_CONSTS_DOUBLE_NIL_VALUE;
    marketsnapshot[i].bid_price5=WMDF_CONSTS_DOUBLE_NIL_VALUE;
    marketsnapshot[i].bid_price6=WMDF_CONSTS_DOUBLE_NIL_VALUE;
    marketsnapshot[i].bid_price7=WMDF_CONSTS_DOUBLE_NIL_VALUE;
    marketsnapshot[i].bid_price8=WMDF_CONSTS_DOUBLE_NIL_VALUE;
    marketsnapshot[i].bid_price9=WMDF_CONSTS_DOUBLE_NIL_VALUE;
    marketsnapshot[i].bid_price10=WMDF_CONSTS_DOUBLE_NIL_VALUE;
    marketsnapshot[i].ask_price1=WMDF_CONSTS_DOUBLE_NIL_VALUE;
    marketsnapshot[i].ask_price2=WMDF_CONSTS_DOUBLE_NIL_VALUE;
    marketsnapshot[i].ask_price3=WMDF_CONSTS_DOUBLE_NIL_VALUE;
    marketsnapshot[i].ask_price4=WMDF_CONSTS_DOUBLE_NIL_VALUE;
    marketsnapshot[i].ask_price5=WMDF_CONSTS_DOUBLE_NIL_VALUE;
    marketsnapshot[i].ask_price6=WMDF_CONSTS_DOUBLE_NIL_VALUE;
    marketsnapshot[i].ask_price7=WMDF_CONSTS_DOUBLE_NIL_VALUE;
    marketsnapshot[i].ask_price8=WMDF_CONSTS_DOUBLE_NIL_VALUE;
    marketsnapshot[i].ask_price9=WMDF_CONSTS_DOUBLE_NIL_VALUE;
    marketsnapshot[i].ask_price10=WMDF_CONSTS_DOUBLE_NIL_VALUE;
    marketsnapshot[i].ask_size1=WMDF_CONSTS_32BIT_NIL_VALUE;
    marketsnapshot[i].ask_size2=WMDF_CONSTS_32BIT_NIL_VALUE;
    marketsnapshot[i].ask_size3=WMDF_CONSTS_32BIT_NIL_VALUE;
    marketsnapshot[i].ask_size4=WMDF_CONSTS_32BIT_NIL_VALUE;
    marketsnapshot[i].ask_size5=WMDF_CONSTS_32BIT_NIL_VALUE;
    marketsnapshot[i].ask_size6=WMDF_CONSTS_32BIT_NIL_VALUE;
    marketsnapshot[i].ask_size7=WMDF_CONSTS_32BIT_NIL_VALUE;
    marketsnapshot[i].ask_size8=WMDF_CONSTS_32BIT_NIL_VALUE;
    marketsnapshot[i].ask_size9=WMDF_CONSTS_32BIT_NIL_VALUE;
    marketsnapshot[i].ask_size10=WMDF_CONSTS_32BIT_NIL_VALUE;
    marketsnapshot[i].bid_size1=WMDF_CONSTS_32BIT_NIL_VALUE;
    marketsnapshot[i].bid_size2=WMDF_CONSTS_32BIT_NIL_VALUE;
    marketsnapshot[i].bid_size3=WMDF_CONSTS_32BIT_NIL_VALUE;
    marketsnapshot[i].bid_size4=WMDF_CONSTS_32BIT_NIL_VALUE;
    marketsnapshot[i].bid_size5=WMDF_CONSTS_32BIT_NIL_VALUE;
    marketsnapshot[i].bid_size6=WMDF_CONSTS_32BIT_NIL_VALUE;
    marketsnapshot[i].bid_size7=WMDF_CONSTS_32BIT_NIL_VALUE;
    marketsnapshot[i].bid_size8=WMDF_CONSTS_32BIT_NIL_VALUE;
    marketsnapshot[i].bid_size9=WMDF_CONSTS_32BIT_NIL_VALUE;
    marketsnapshot[i].bid_size10=WMDF_CONSTS_32BIT_NIL_VALUE;
    marketsnapshot[i].transactions_count=WMDF_CONSTS_64BIT_NIL_VALUE;
    marketsnapshot[i].weighted_average_bid_price=WMDF_CONSTS_DOUBLE_NIL_VALUE;
    marketsnapshot[i].weighted_average_ask_price=WMDF_CONSTS_DOUBLE_NIL_VALUE;
    marketsnapshot[i].total_bid_size=WMDF_CONSTS_64BIT_NIL_VALUE;
    marketsnapshot[i].total_ask_size=WMDF_CONSTS_64BIT_NIL_VALUE;
		marketsnapshot[i].peratio1 = WMDF_CONSTS_DOUBLE_NIL_VALUE;
		marketsnapshot[i].peratio2 = WMDF_CONSTS_DOUBLE_NIL_VALUE;
		marketsnapshot[i].trade_status = WMDF_CONSTS_8BIT_NIL_VALUE;
  }
}

void SecurityUtils::initialize_transaction( L2Transaction *transaction, size_t n )
{
	size_t size_of_windcode = sizeof(transaction->wind_code);
	for(size_t i=0;i!=n;++i)
	{
		memset(transaction[i].wind_code,0,size_of_windcode);
		transaction[i].trade_time=WMDF_CONSTS_32BIT_NIL_VALUE;
		transaction[i].set_no=WMDF_CONSTS_32BIT_NIL_VALUE;
		transaction[i].rec_no=WMDF_CONSTS_32BIT_NIL_VALUE;
		transaction[i].buyOrderRecNo=WMDF_CONSTS_32BIT_NIL_VALUE;
		transaction[i].sellOrderRecNo=WMDF_CONSTS_32BIT_NIL_VALUE;
		transaction[i].price=WMDF_CONSTS_DOUBLE_NIL_VALUE;
		transaction[i].volume=WMDF_CONSTS_32BIT_NIL_VALUE;
		transaction[i].order_kind=WMDF_CONSTS_8BIT_NIL_VALUE;
		transaction[i].function_code=WMDF_CONSTS_8BIT_NIL_VALUE;
		transaction[i].amount = WMDF_CONSTS_DOUBLE_NIL_VALUE;
	}
}

void SecurityUtils::initialize_orderqueue( L2OrderQueue *orderqueue, size_t n )
{
	size_t size_of_windcode = sizeof(orderqueue->wind_code);
	for(size_t i = 0; i != n; ++i)
	{
		memset(orderqueue[i].wind_code, 0, size_of_windcode);
		orderqueue[i].data_time_stamp = WMDF_CONSTS_32BIT_NIL_VALUE;
		orderqueue[i].image_status = WMDF_CONSTS_32BIT_NIL_VALUE;
		orderqueue[i].order_side = WMDF_CONSTS_8BIT_NIL_VALUE;
		orderqueue[i].price_level_count = WMDF_CONSTS_32BIT_NIL_VALUE;
		orderqueue[i].price_levels = NULL;
	}
}

void SecurityUtils::initialize_indexdata( IndexSnapshot *indexdata, size_t n )
{
	size_t size_of_windcode = sizeof(indexdata->wind_code);
	for(size_t i=0; i!=n; ++i)
	{
		memset(indexdata[i].wind_code,0,size_of_windcode);
		indexdata[i].data_time_stamp = WMDF_CONSTS_32BIT_NIL_VALUE;
		indexdata[i].pre_close = WMDF_CONSTS_DOUBLE_NIL_VALUE;
		indexdata[i].high_price = WMDF_CONSTS_DOUBLE_NIL_VALUE;
		indexdata[i].low_price = WMDF_CONSTS_DOUBLE_NIL_VALUE;
		indexdata[i].new_price = WMDF_CONSTS_DOUBLE_NIL_VALUE;
		indexdata[i].open_price = WMDF_CONSTS_DOUBLE_NIL_VALUE;
		indexdata[i].total_amount = WMDF_CONSTS_DOUBLE_NIL_VALUE;
		indexdata[i].total_volume = WMDF_CONSTS_64BIT_NIL_VALUE;
	}
}

void SecurityUtils::initialize_orders( SZSEL2Order* orders, size_t n )
{
	size_t size_of_windcode = sizeof(orders->wind_code);
	for(size_t i=0;i!=n;++i)
	{
		memset(orders[i].wind_code,0,size_of_windcode);
		orders[i].function_code = WMDF_CONSTS_8BIT_NIL_VALUE;
		orders[i].order_kind = WMDF_CONSTS_8BIT_NIL_VALUE;
		orders[i].order_time = WMDF_CONSTS_32BIT_NIL_VALUE;
		orders[i].price = WMDF_CONSTS_DOUBLE_NIL_VALUE;
		orders[i].rec_no = WMDF_CONSTS_32BIT_NIL_VALUE;
		orders[i].set_no = WMDF_CONSTS_32BIT_NIL_VALUE;
		orders[i].size = WMDF_CONSTS_32BIT_NIL_VALUE;
	}
}

