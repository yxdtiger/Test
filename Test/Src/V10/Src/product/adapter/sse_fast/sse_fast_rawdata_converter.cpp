#include "sse_fast_rawdata_converter.h"
#include "../../../environment/security_utils.h"
#include "../../../include/date_time.h"
#include "../../../protocol/mdf_message.h"
#include "../../../session/message_publisher.h"



wmdf::SSEFastRawDataConverter::SSEFastRawDataConverter()
{
  market_data_3202_merger_ = new SSE_MarketData_3202_Merger();
}

wmdf::SSEFastRawDataConverter::~SSEFastRawDataConverter()
{
  if (NULL != market_data_3202_merger_)
  {
    delete market_data_3202_merger_;
    market_data_3202_merger_ = NULL;
  }
}

const char* wmdf::SSEFastRawDataConverter::sse_suffix = ".SH";


void wmdf::SSEFastRawDataConverter::convertMarketsnapshot( WindFastMessage* msg,uint32_t& pack_id )
{
	int32_t size = msg->Size;
	L2MarketSnapshot *marketdata = new L2MarketSnapshot[size];
	SecurityUtils::initialize_l2_market(marketdata,size);
	MarketData* temp = (MarketData*)msg->Data;
	for(int32_t i=0;i!=size;++i)
	{
		memcpy(marketdata[i].wind_code,temp[i].wind_code,sizeof(temp[i].wind_code));
		memcpy(&marketdata[i].wind_code[6],sse_suffix,4);
		marketdata[i].data_time_stamp = DateTime::hmsms2time(temp[i].trade_time);
		marketdata[i].new_price = temp[i].new_price;
		marketdata[i].pre_close = temp[i].pre_close;
		marketdata[i].open_price = temp[i].today_open;
		marketdata[i].high_price = temp[i].today_high;
		marketdata[i].low_price = temp[i].today_low;
		marketdata[i].total_volume = temp[i].volume;
		marketdata[i].total_amount = temp[i].total_amount;
		marketdata[i].bid_price1 = temp[i].bid_price[0];
		marketdata[i].bid_price2 = temp[i].bid_price[1];
		marketdata[i].bid_price3 = temp[i].bid_price[2];
		marketdata[i].bid_price4 = temp[i].bid_price[3];
		marketdata[i].bid_price5 = temp[i].bid_price[4];
		marketdata[i].bid_price6 = temp[i].bid_price[5];
		marketdata[i].bid_price7 = temp[i].bid_price[6];
		marketdata[i].bid_price8 = temp[i].bid_price[7];
		marketdata[i].bid_price9 = temp[i].bid_price[8];
		marketdata[i].bid_price10 = temp[i].bid_price[9];
		marketdata[i].ask_price1 = temp[i].ask_price[0];
		marketdata[i].ask_price2 = temp[i].ask_price[1];
		marketdata[i].ask_price3 = temp[i].ask_price[2];
		marketdata[i].ask_price4 = temp[i].ask_price[3];
		marketdata[i].ask_price5 = temp[i].ask_price[4];
		marketdata[i].ask_price6 = temp[i].ask_price[5];
		marketdata[i].ask_price7 = temp[i].ask_price[6];
		marketdata[i].ask_price8 = temp[i].ask_price[7];
		marketdata[i].ask_price9 = temp[i].ask_price[8];
		marketdata[i].ask_price10 = temp[i].ask_price[9];
		marketdata[i].bid_size1 = temp[i].bid_size[0];
		marketdata[i].bid_size2 = temp[i].bid_size[1];
		marketdata[i].bid_size3 = temp[i].bid_size[2];
		marketdata[i].bid_size4 = temp[i].bid_size[3];
		marketdata[i].bid_size5 = temp[i].bid_size[4];
		marketdata[i].bid_size6 = temp[i].bid_size[5];
		marketdata[i].bid_size7 = temp[i].bid_size[6];
		marketdata[i].bid_size8 = temp[i].bid_size[7];
		marketdata[i].bid_size9 = temp[i].bid_size[8];
		marketdata[i].bid_size10 = temp[i].bid_size[9];
		marketdata[i].ask_size1 = temp[i].ask_size[0];
		marketdata[i].ask_size2 = temp[i].ask_size[1];
		marketdata[i].ask_size3 = temp[i].ask_size[2];
		marketdata[i].ask_size4 = temp[i].ask_size[3];
		marketdata[i].ask_size5 = temp[i].ask_size[4];
		marketdata[i].ask_size6 = temp[i].ask_size[5];
		marketdata[i].ask_size7 = temp[i].ask_size[6];
		marketdata[i].ask_size8 = temp[i].ask_size[7];
		marketdata[i].ask_size9 = temp[i].ask_size[8];
		marketdata[i].ask_size10 = temp[i].ask_size[9];
		marketdata[i].transactions_count = temp[i].transactions_count;
		marketdata[i].weighted_average_bid_price = temp[i].weighted_average_best_bid;
		marketdata[i].weighted_average_ask_price = temp[i].weighted_average_best_ask;
		marketdata[i].total_bid_size = temp[i].l2_total_bid_qty;
		marketdata[i].total_ask_size = temp[i].l2_total_offer_qty;
		marketdata[i].peratio1 = temp[i].peratio1;
		marketdata[i].peratio2 = temp[i].peratio2;
	}

	MdfMessage* mdf_msg = new MdfMessage();
	mdf_msg->set_local_time(DateTime::get_current_time());
	mdf_msg->set_socket_id(0);
	mdf_msg->set_product_id(PRODUCT_SSE_LEVEL2_ID);
	mdf_msg->set_message_id(SSEL2_MARKETDATA_ID);
	mdf_msg->set_major_version(MAJOR_VERSION_SSE_LEVEL2_ID);
	mdf_msg->set_minor_version(MINOR_VERSION_SSE_LEVEL2_ID);
	mdf_msg->set_record_count(size);

	MarketMessageHeader* message_header = new MarketMessageHeader();
	message_header->place_holder = 0;
	mdf_msg->set_packet_num(++pack_id);
	mdf_msg->set_header(message_header);
	mdf_msg->set_body(marketdata);

	SharedMdfMessage message = SharedMdfMessage(mdf_msg);
	MessagePublisher::PublishOneMessage(message);
}

void wmdf::SSEFastRawDataConverter::convertOrderqueue( WindFastMessage* msg, uint32_t& pack_id )
{
	int32_t size = msg->Size;
	L2OrderQueue *orderqueue = new L2OrderQueue[size];
	SecurityUtils::initialize_orderqueue(orderqueue,size);
	OrderQueueImage* temp = (OrderQueueImage*)msg->Data;

	for(int32_t i=0;i!=size;++i)
	{
		memcpy(orderqueue[i].wind_code,temp[i].wind_code,sizeof(temp[i].wind_code));
		memcpy(&orderqueue[i].wind_code[6],sse_suffix,4);
		orderqueue[i].data_time_stamp = DateTime::hmsms2time(temp[i].date_time_stamp);
		orderqueue[i].image_status =	temp[i].image_status;
		orderqueue[i].price_level_count = temp[i].price_level_count;
		orderqueue[i].order_side = temp[i].side[0];
		if(temp[i].price_level_count>0)
		{
			orderqueue[i].price_levels = new L2OrderQueuePriceLevel[temp[i].price_level_count];
			for(int32_t j=0;j!=orderqueue[i].price_level_count;++j)
			{
				orderqueue[i].price_levels[j].operator_type = temp[i].order_queue_levels[j].price_level_operator;
				orderqueue[i].price_levels[j].order_price = temp[i].order_queue_levels[j].price;
				orderqueue[i].price_levels[j].total_number = temp[i].order_queue_levels[j].orders_total;
				orderqueue[i].price_levels[j].orders_number = temp[i].order_queue_levels[j].orders_count;
				if(temp[i].order_queue_levels[j].orders_count>0)
				{
					orderqueue[i].price_levels[j].order_items = new L2OrderQueueItem[temp[i].order_queue_levels[j].orders_count];
					for(int32_t k=0;k!=temp[i].order_queue_levels[j].orders_count;++k)
					{
						orderqueue[i].price_levels[j].order_items[k].entry_id = temp[i].order_queue_levels[j].order_queue_items[k].order_queue_operator_entry_id;
						orderqueue[i].price_levels[j].order_items[k].operator_type = temp[i].order_queue_levels[j].order_queue_items[k].order_queue_operator;
						orderqueue[i].price_levels[j].order_items[k].size = temp[i].order_queue_levels[j].order_queue_items[k].order_qty;
					}
				}
				else
				{
					temp[i].order_queue_levels[j].orders_count = 0;
					orderqueue[i].price_levels[j].order_items = NULL;
				}
			}
		}
		else
		{
			temp[i].price_level_count = 0;
			orderqueue[i].price_levels = NULL;
		}
	}

	MdfMessage* mdf_msg = new MdfMessage();
	mdf_msg->set_local_time(DateTime::get_current_time());
	mdf_msg->set_socket_id(0);
	mdf_msg->set_product_id(PRODUCT_SSE_LEVEL2_ID);
	mdf_msg->set_message_id(SSEL2_ORDERQUEUE_ID);
	mdf_msg->set_major_version(MAJOR_VERSION_SSE_LEVEL2_ID);
	mdf_msg->set_minor_version(MINOR_VERSION_SSE_LEVEL2_ID);
	mdf_msg->set_record_count(size);

	MarketMessageHeader* message_header = new MarketMessageHeader();
	message_header->place_holder = 0;
	mdf_msg->set_packet_num(++pack_id);
	mdf_msg->set_header(message_header);
	mdf_msg->set_body(orderqueue);

	SharedMdfMessage message = SharedMdfMessage(mdf_msg);
	MessagePublisher::PublishOneMessage(message);
}

void wmdf::SSEFastRawDataConverter::convertTransaction( WindFastMessage* msg, uint32_t& pack_id)
{
	int32_t size = msg->Size;
	L2Transaction *transaction = new L2Transaction[size];
	SecurityUtils::initialize_transaction(transaction,size);
	Transaction* temp = (Transaction*)msg->Data;

	for(int32_t i=0;i!=size;++i)
	{
		memcpy(transaction[i].wind_code,temp[i].wind_code,sizeof(temp[i].wind_code));
		memcpy(&transaction[i].wind_code[6],sse_suffix,4);
		int32_t time = temp[i].trade_time;
		if(msg->MsgType==SSE_TRANSACTION_3201_TYPE)
			transaction[i].trade_time = DateTime::hmsms2time(time*10);
		else
			transaction[i].trade_time = DateTime::hmsms2time(time);

		transaction[i].set_no = temp[i].set_no;
		transaction[i].rec_no = temp[i].rec_no;
		transaction[i].buyOrderRecNo = temp[i].buyOrderRecNo;
		transaction[i].sellOrderRecNo = temp[i].sellOrderRecNo;
		transaction[i].price = temp[i].price;
		transaction[i].volume = temp[i].trade_qty;
		transaction[i].order_kind = temp[i].order_kind;
		transaction[i].function_code = temp[i].function_code;
		transaction[i].amount = temp[i].trade_money;
	}

	MdfMessage* mdf_msg = new MdfMessage();
	mdf_msg->set_local_time(DateTime::get_current_time());
	mdf_msg->set_socket_id(0);
	mdf_msg->set_product_id(PRODUCT_SSE_LEVEL2_ID);
	mdf_msg->set_message_id(SSEL2_TRANSACTION_ID);
	mdf_msg->set_major_version(MAJOR_VERSION_SSE_LEVEL2_ID);
	mdf_msg->set_minor_version(MINOR_VERSION_SSE_LEVEL2_ID);
	mdf_msg->set_record_count(size);

	MarketMessageHeader* message_header = new MarketMessageHeader();
	message_header->place_holder = 0;
	mdf_msg->set_packet_num(++pack_id);
	mdf_msg->set_header(message_header);
	mdf_msg->set_body(transaction);

	SharedMdfMessage message = SharedMdfMessage(mdf_msg);
	MessagePublisher::PublishOneMessage(message);
}


void wmdf::SSEFastRawDataConverter::convertVirtualauctionprice( WindFastMessage* msg, uint32_t& pack_id)
{
	int32_t size = msg->Size;
	VirtualAuctionPrice *temp = (VirtualAuctionPrice*)msg->Data;
	int count = 0;
	for(int i = 0;i!=size;++i)
	{
		if(temp[i].date_time_stamp<92500000)
			++count;
	}

	L2MarketSnapshot *marketdata = new L2MarketSnapshot[count];
	SecurityUtils::initialize_l2_market(marketdata,count);

	for(int32_t i=0;i!=size;++i)
	{
		if(temp[i].date_time_stamp<92500000)
		{
			memcpy(marketdata[i].wind_code,temp[i].windcode,sizeof(temp[i].windcode));
			memcpy(&marketdata[i].wind_code[6],sse_suffix,4);
			marketdata[i].data_time_stamp = DateTime::hmsms2time(temp[i].date_time_stamp);
			if(temp[i].bidleaveqty!=0)
			{
				marketdata[i].bid_price1 = temp[i].price;
				marketdata[i].bid_price2 = 0;
				marketdata[i].bid_size1 = (int32_t)temp[i].virtualauctionqty;
				marketdata[i].bid_size2 = (int32_t)temp[i].bidleaveqty;
				marketdata[i].ask_price1 = 0;
				marketdata[i].ask_price2 = 0;
				marketdata[i].ask_size1 = 0;
				marketdata[i].ask_size2 = 0;
			}
			else if(temp[i].askleaveqty!=0)
			{
				marketdata[i].ask_price1 = temp[i].price;
				marketdata[i].ask_price2 = 0;
				marketdata[i].ask_size1 = (int32_t)temp[i].virtualauctionqty;
				marketdata[i].ask_size2 = (int32_t)temp[i].askleaveqty;
				marketdata[i].bid_price1 = 0;
				marketdata[i].bid_price2 = 0;
				marketdata[i].bid_size1 = 0;
				marketdata[i].bid_size2 = 0;
			}
			else
			{
				marketdata[i].bid_price1 = temp[i].price;
				marketdata[i].bid_price2 = 0;
				marketdata[i].bid_size1 = (int32_t)temp[i].virtualauctionqty;
				marketdata[i].bid_size2 = 0;
				marketdata[i].ask_price1 = 0;
				marketdata[i].ask_price2 = 0;
				marketdata[i].ask_size1 = 0;
				marketdata[i].ask_size2 = 0;
			}
		}
	}
  if(count>0)
  {
    MdfMessage* mdf_msg = new MdfMessage();
    mdf_msg->set_local_time(DateTime::get_current_time());
    mdf_msg->set_socket_id(0);
    mdf_msg->set_product_id(PRODUCT_SSE_LEVEL2_ID);
    mdf_msg->set_message_id(SSEL2_MARKETDATA_ID);
    mdf_msg->set_major_version(MAJOR_VERSION_SSE_LEVEL2_ID);
    mdf_msg->set_minor_version(MINOR_VERSION_SSE_LEVEL2_ID);
    mdf_msg->set_record_count(count);

    MarketMessageHeader* message_header = new MarketMessageHeader();
    message_header->place_holder = 0;
    mdf_msg->set_packet_num(++pack_id);
    mdf_msg->set_header(message_header);
    mdf_msg->set_body(marketdata);

    SharedMdfMessage message = SharedMdfMessage(mdf_msg);
    MessagePublisher::PublishOneMessage(message);
  }
}

void wmdf::SSEFastRawDataConverter::convertIndexData( WindFastMessage* msg,uint32_t&pack_id )
{
	int32_t size = msg->Size;
	IndexSnapshot *indexdata = new IndexSnapshot[size];
	SecurityUtils::initialize_indexdata(indexdata,size);
	IndexData* temp = (IndexData*)msg->Data;
	for(int32_t i=0;i!=size;++i)
	{
		memcpy(indexdata[i].wind_code,temp[i].SecurityID,sizeof(temp[i].SecurityID));
		memcpy(&indexdata[i].wind_code[6],sse_suffix,4);
		indexdata[i].data_time_stamp = DateTime::hmsms2time(temp[i].DataTimeStamp*1000);
		indexdata[i].pre_close = temp[i].PreClosePx;
		indexdata[i].high_price = temp[i].HighPx;
		indexdata[i].low_price = temp[i].LowPx;
		indexdata[i].new_price = temp[i].LastPx;
		indexdata[i].open_price = temp[i].OpenPx;
		indexdata[i].total_amount = temp[i].total_amount;
		indexdata[i].total_volume = temp[i].volume;
	}

	MdfMessage* mdf_msg = new MdfMessage();
	mdf_msg->set_local_time(DateTime::get_current_time());
	mdf_msg->set_socket_id(0);
	mdf_msg->set_product_id(PRODUCT_SSE_LEVEL2_ID);
	mdf_msg->set_message_id(SSEL2_INDEXDATA_ID);
	mdf_msg->set_major_version(MAJOR_VERSION_SSE_LEVEL2_ID);
	mdf_msg->set_minor_version(MINOR_VERSION_SSE_LEVEL2_ID);
	mdf_msg->set_record_count(size);

	MarketMessageHeader* message_header = new MarketMessageHeader();
	message_header->place_holder = 0;
	mdf_msg->set_packet_num(++pack_id);
	mdf_msg->set_header(message_header);
	mdf_msg->set_body(indexdata);

	SharedMdfMessage message = SharedMdfMessage(mdf_msg);
	MessagePublisher::PublishOneMessage(message);
}

void wmdf::SSEFastRawDataConverter::convertMarketdata3202( WindFastMessage *msg, uint32_t &pack_id )
{
	MarketData_3202 *old_marketdata = (MarketData_3202*)msg->Data;
	int32_t size = msg->Size;
	L2MarketSnapshot *marketdata = new L2MarketSnapshot[size];
	SecurityUtils::initialize_l2_market(marketdata, size);
  market_data_3202_merger_->set_order_queue_count(0);

	for(int32_t i=0; i!=size; ++i)
	{
		memcpy(marketdata[i].wind_code,old_marketdata[i].windcode,sizeof(old_marketdata[i].windcode));
		memcpy(&marketdata[i].wind_code[6],sse_suffix,4);
		marketdata[i].data_time_stamp = DateTime::hmsms2time(old_marketdata[i].date_time_stamp*1000);
    market_data_3202_merger_->ProcessMarketSnapshot(&old_marketdata[i],&marketdata[i]);
	}

	MdfMessage* mdf_msg = new MdfMessage();
	mdf_msg->set_local_time(DateTime::get_current_time());
	mdf_msg->set_socket_id(0);
	mdf_msg->set_product_id(PRODUCT_SSE_LEVEL2_ID);
	mdf_msg->set_message_id(SSEL2_MARKETDATA_ID);
	mdf_msg->set_major_version(MAJOR_VERSION_SSE_LEVEL2_ID);
	mdf_msg->set_minor_version(MINOR_VERSION_SSE_LEVEL2_ID);
	mdf_msg->set_record_count(size);

	MarketMessageHeader* message_header = new MarketMessageHeader();
	message_header->place_holder = 0;
	mdf_msg->set_packet_num(++pack_id);
	mdf_msg->set_header(message_header);
	mdf_msg->set_body(marketdata);

	SharedMdfMessage message = SharedMdfMessage(mdf_msg);
	MessagePublisher::PublishOneMessage(message);
}

void wmdf::SSEFastRawDataConverter::convertOrderqueue3202( WindFastMessage *msg, uint32_t &pack_id)
{
  if (market_data_3202_merger_->order_queue_count() <= 0)
  {
    return;
  }

	MarketData_3202 *src = (MarketData_3202*)msg->Data;
	L2OrderQueue *orderqueues = new L2OrderQueue[market_data_3202_merger_->order_queue_count()];
	SecurityUtils::initialize_orderqueue(orderqueues, market_data_3202_merger_->order_queue_count());

  //处理委买
  int32_t dst_index = 0;
	for(int32_t src_index=0;src_index<msg->Size;src_index++)
	{
    int32_t price_level_count = GetOrderQueuePriceLevelCount(src[src_index].bidlevels,src[src_index].num_of_bid_level);
    if (price_level_count>0)
    {
      memcpy(orderqueues[dst_index].wind_code,src[src_index].windcode,sizeof(src[src_index].windcode));
      memcpy(&orderqueues[dst_index].wind_code[6],sse_suffix,4);
			orderqueues[dst_index].data_time_stamp = DateTime::hmsms2time(src[src_index].date_time_stamp*1000);
			orderqueues[dst_index].image_status = src[src_index].image_status;
			orderqueues[dst_index].order_side = 1;
			orderqueues[dst_index].price_level_count = price_level_count;
			orderqueues[dst_index].price_levels = new L2OrderQueuePriceLevel[price_level_count];
      ConvertOrderQueue3202(src[src_index].bidlevels,src[src_index].num_of_bid_level,orderqueues[dst_index]);
      dst_index++;
    }
    //处理委卖
		price_level_count = GetOrderQueuePriceLevelCount(src[src_index].offerlevels,src[src_index].num_of_offer_level);
		if(price_level_count > 0)
		{
      memcpy(orderqueues[dst_index].wind_code,src[src_index].windcode,sizeof(src[src_index].windcode));
      memcpy(&orderqueues[dst_index].wind_code[6],sse_suffix,4);
			orderqueues[dst_index].data_time_stamp = DateTime::hmsms2time(src[src_index].date_time_stamp*1000);
			orderqueues[dst_index].image_status = src[src_index].image_status;
			orderqueues[dst_index].order_side = 2;
			orderqueues[dst_index].price_level_count = price_level_count;
			memcpy(orderqueues[dst_index].wind_code,src[src_index].windcode,sizeof(src[src_index].windcode));
			memcpy(&orderqueues[dst_index].wind_code[6],sse_suffix,4);
			orderqueues[dst_index].price_levels = new L2OrderQueuePriceLevel[price_level_count];
      ConvertOrderQueue3202(src[src_index].offerlevels,src[src_index].num_of_offer_level,orderqueues[dst_index]);
      dst_index++;
		}
	}
  assert(dst_index == market_data_3202_merger_->order_queue_count());

	MdfMessage* mdf_msg = new MdfMessage();
	mdf_msg->set_local_time(DateTime::get_current_time());
	mdf_msg->set_socket_id(0);
	mdf_msg->set_product_id(PRODUCT_SSE_LEVEL2_ID);
	mdf_msg->set_message_id(SSEL2_ORDERQUEUE_ID);
	mdf_msg->set_major_version(MAJOR_VERSION_SSE_LEVEL2_ID);
	mdf_msg->set_minor_version(MINOR_VERSION_SSE_LEVEL2_ID);
	mdf_msg->set_record_count(market_data_3202_merger_->order_queue_count());

	MarketMessageHeader* message_header = new MarketMessageHeader();
	message_header->place_holder = 0;
	mdf_msg->set_packet_num(++pack_id);
	mdf_msg->set_header(message_header);
	mdf_msg->set_body(orderqueues);

	SharedMdfMessage message = SharedMdfMessage(mdf_msg);
	MessagePublisher::PublishOneMessage(message);
}


int32_t wmdf::SSEFastRawDataConverter::GetOrderQueuePriceLevelCount( PriceLevel_3202* price_levels,int32_t count )
{
  int32_t result = 0;
  for (int32_t index = 0; index< count;index++)
  {
    if (price_levels[index].no_order > 0)
    {
      result++;
    }
  }
  return result;
}

void wmdf::SSEFastRawDataConverter::ConvertOrderQueue3202( PriceLevel_3202* src,int32_t src_count, L2OrderQueue& dst )
{
  int32_t dstindex = 0;
  for(int32_t srcindex=0;srcindex!=src_count;srcindex++)
  {
    if (src[srcindex].no_order <=0) { continue; }
    L2OrderQueuePriceLevel* dst_pricelevel = &dst.price_levels[dstindex];
    dst_pricelevel->operator_type = src[srcindex].price_level_operator;
    dst_pricelevel->order_price = src[srcindex].price;
    dst_pricelevel->total_number = src[srcindex].num_order;
    dst_pricelevel->orders_number = src[srcindex].no_order;
    dst_pricelevel->order_items = new L2OrderQueueItem[src[srcindex].no_order];
    for(int32_t index=0;index!=src[srcindex].no_order;index++)
    {
      dst_pricelevel->order_items[index].entry_id = src[srcindex].order_items[index].order_queue_operator_entry_id;
      dst_pricelevel->order_items[index].operator_type = src[srcindex].order_items[index].order_queue_operator;
      dst_pricelevel->order_items[index].size = src[srcindex].order_items[index].order_qty;
    }
    dstindex++;
  }
}

void wmdf::SSEFastRawDataConverter::Reset()
{
  if (NULL != market_data_3202_merger_)
  {
    market_data_3202_merger_->Clear();
  }
}
