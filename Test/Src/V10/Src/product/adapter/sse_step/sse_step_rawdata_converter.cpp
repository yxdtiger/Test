#include "sse_step_rawdata_converter.h"
#include "../../../environment/security_utils.h"
#include "../../../include/date_time.h"
#include "../common/rawdata_object.h"

const char* sse_suffix = ".SH";

wmdf::SSEStepRawDataConverter::SSEStepRawDataConverter()
{

}

wmdf::SSEStepRawDataConverter::~SSEStepRawDataConverter()
{

}

void wmdf::SSEStepRawDataConverter::ConvertData( std::vector<WindStepMessage*>& msgs)
{
	windstep_msgs_ = &msgs;
  reset();
  if(calculate())
  {
    allocateMemory();
    convert();
  }
}

void wmdf::SSEStepRawDataConverter::reset()
{
	num_of_indexdata_ = 0;
	num_of_marketsnapshot_ = 0;
	num_of_orderqueue_ = 0;
	num_of_transaction_ = 0;

	orderqueues_ = NULL;
	transactions_ = NULL;
	marketsnapshots_ = NULL;
	indexdata_ = NULL;
}

bool wmdf::SSEStepRawDataConverter::calculate()
{
	uint32_t size_of_data = windstep_msgs_->size();
	if(size_of_data == 0)
		return false;

	for(size_t i = 0; i!=size_of_data; ++i)
	{
		switch((*windstep_msgs_)[i]->MsgType)
		{
		case SSE_VIRTUALAUCTIONPRICE_1107_TYPE:
			++num_of_marketsnapshot_;
			break;
		case SSE_ORDERQUEUE_1111_TYPE:
			++num_of_orderqueue_;
			break;
		case SSE_MARKETDATA_1102_TYPE:
			++num_of_marketsnapshot_;
			break;
		case SSE_TRANSACTION_1101_TYPE:
			++num_of_transaction_;
			break;
		case SSE_INDEXDATA_1113_TYPE:
			++num_of_indexdata_;
			break;
		default:
			break;
		}
	}
	return true;
}

void wmdf::SSEStepRawDataConverter::allocateMemory()
{
	if(num_of_marketsnapshot_ != 0)
	{
		marketsnapshots_ = new L2MarketSnapshot[num_of_marketsnapshot_];
		SecurityUtils::initialize_l2_market(marketsnapshots_, num_of_marketsnapshot_);
	}

	if(num_of_transaction_ != 0)
	{
		transactions_ = new L2Transaction[num_of_transaction_];
		SecurityUtils::initialize_transaction(transactions_, num_of_transaction_);
	}

	if(num_of_orderqueue_ != 0)
	{
		orderqueues_ = new L2OrderQueue[num_of_orderqueue_];
		SecurityUtils::initialize_orderqueue(orderqueues_, num_of_orderqueue_);
	}

	if(num_of_indexdata_ != 0)
	{
		indexdata_ = new IndexSnapshot[num_of_indexdata_];
		SecurityUtils::initialize_indexdata(indexdata_, num_of_indexdata_);
	}
}

void wmdf::SSEStepRawDataConverter::convert()
{
	uint32_t size_of_step = windstep_msgs_->size();
	uint32_t pos_of_orderqueue = 0;
	uint32_t pos_of_marketsnapshot = 0;
	uint32_t pos_of_transaction = 0;
	uint32_t pos_of_marketoverview = 0;
	uint32_t pos_of_indexdata = 0;

	for(size_t i = 0; i!=size_of_step; ++i)
	{
		WindStepMessage* windstepmsg = (*windstep_msgs_)[i];
		switch(windstepmsg->MsgType)
		{
		case SSE_VIRTUALAUCTIONPRICE_1107_TYPE:
			convertVirtualauctionprice(pos_of_marketsnapshot, windstepmsg);
			++pos_of_marketsnapshot;
			break;
		case SSE_ORDERQUEUE_1111_TYPE:
			convertOrderqueue(pos_of_orderqueue, windstepmsg);
			++pos_of_orderqueue;
			break;
		case SSE_MARKETDATA_1102_TYPE:
			convertMarketsnapshot(pos_of_marketsnapshot, windstepmsg);
			++pos_of_marketsnapshot;
			break;
		case SSE_TRANSACTION_1101_TYPE:
			convertTransaction(pos_of_transaction,windstepmsg);
			++pos_of_transaction;
			break;
		case SSE_INDEXDATA_1113_TYPE:
			convertIndexData(pos_of_indexdata,windstepmsg);
			++pos_of_indexdata;
			break;
		default:
			break;
		}
	}
}

void wmdf::SSEStepRawDataConverter::convertMarketsnapshot( uint32_t pos_of_marketsnapshot, WindStepMessage *wsm )
{
	MarketData* temp = (MarketData*)wsm->Data;
	memcpy(marketsnapshots_[pos_of_marketsnapshot].wind_code,temp->wind_code,sizeof(marketsnapshots_->wind_code));
	memcpy(&marketsnapshots_[pos_of_marketsnapshot].wind_code[6],sse_suffix,4);
	//marketsnapshots_[pos_of_marketsnapshot].trade_date = temp->trade_date;
	marketsnapshots_[pos_of_marketsnapshot].data_time_stamp = DateTime::hms2time(temp->trade_time)*1000;
	marketsnapshots_[pos_of_marketsnapshot].new_price = temp->new_price;
	marketsnapshots_[pos_of_marketsnapshot].pre_close = temp->pre_close;
	marketsnapshots_[pos_of_marketsnapshot].open_price = temp->today_open;
	marketsnapshots_[pos_of_marketsnapshot].high_price = temp->today_high;
	marketsnapshots_[pos_of_marketsnapshot].low_price = temp->today_low;
	marketsnapshots_[pos_of_marketsnapshot].total_volume = temp->volume;
	marketsnapshots_[pos_of_marketsnapshot].total_amount = temp->total_amount;
	marketsnapshots_[pos_of_marketsnapshot].bid_price1 = temp->bid_price[0];
	marketsnapshots_[pos_of_marketsnapshot].bid_price2 = temp->bid_price[1];
	marketsnapshots_[pos_of_marketsnapshot].bid_price3 = temp->bid_price[2];
	marketsnapshots_[pos_of_marketsnapshot].bid_price4 = temp->bid_price[3];
	marketsnapshots_[pos_of_marketsnapshot].bid_price5 = temp->bid_price[4];
	marketsnapshots_[pos_of_marketsnapshot].bid_price6 = temp->bid_price[5];
	marketsnapshots_[pos_of_marketsnapshot].bid_price7 = temp->bid_price[6];
	marketsnapshots_[pos_of_marketsnapshot].bid_price8 = temp->bid_price[7];
	marketsnapshots_[pos_of_marketsnapshot].bid_price9 = temp->bid_price[8];
	marketsnapshots_[pos_of_marketsnapshot].bid_price10 = temp->bid_price[9];
	marketsnapshots_[pos_of_marketsnapshot].ask_price1 = temp->ask_price[0];
	marketsnapshots_[pos_of_marketsnapshot].ask_price2 = temp->ask_price[1];
	marketsnapshots_[pos_of_marketsnapshot].ask_price3 = temp->ask_price[2];
	marketsnapshots_[pos_of_marketsnapshot].ask_price4 = temp->ask_price[3];
	marketsnapshots_[pos_of_marketsnapshot].ask_price5 = temp->ask_price[4];
	marketsnapshots_[pos_of_marketsnapshot].ask_price6 = temp->ask_price[5];
	marketsnapshots_[pos_of_marketsnapshot].ask_price7 = temp->ask_price[6];
	marketsnapshots_[pos_of_marketsnapshot].ask_price8 = temp->ask_price[7];
	marketsnapshots_[pos_of_marketsnapshot].ask_price9 = temp->ask_price[8];
	marketsnapshots_[pos_of_marketsnapshot].ask_price10 = temp->ask_price[9];
	marketsnapshots_[pos_of_marketsnapshot].bid_size1 = temp->bid_size[0];
	marketsnapshots_[pos_of_marketsnapshot].bid_size2 = temp->bid_size[1];
	marketsnapshots_[pos_of_marketsnapshot].bid_size3 = temp->bid_size[2];
	marketsnapshots_[pos_of_marketsnapshot].bid_size4 = temp->bid_size[3];
	marketsnapshots_[pos_of_marketsnapshot].bid_size5 = temp->bid_size[4];
	marketsnapshots_[pos_of_marketsnapshot].bid_size6 = temp->bid_size[5];
	marketsnapshots_[pos_of_marketsnapshot].bid_size7 = temp->bid_size[6];
	marketsnapshots_[pos_of_marketsnapshot].bid_size8 = temp->bid_size[7];
	marketsnapshots_[pos_of_marketsnapshot].bid_size9 = temp->bid_size[8];
	marketsnapshots_[pos_of_marketsnapshot].bid_size10 = temp->bid_size[9];
	marketsnapshots_[pos_of_marketsnapshot].ask_size1 = temp->ask_size[0];
	marketsnapshots_[pos_of_marketsnapshot].ask_size2 = temp->ask_size[1];
	marketsnapshots_[pos_of_marketsnapshot].ask_size3 = temp->ask_size[2];
	marketsnapshots_[pos_of_marketsnapshot].ask_size4 = temp->ask_size[3];
	marketsnapshots_[pos_of_marketsnapshot].ask_size5 = temp->ask_size[4];
	marketsnapshots_[pos_of_marketsnapshot].ask_size6 = temp->ask_size[5];
	marketsnapshots_[pos_of_marketsnapshot].ask_size7 = temp->ask_size[6];
	marketsnapshots_[pos_of_marketsnapshot].ask_size8 = temp->ask_size[7];
	marketsnapshots_[pos_of_marketsnapshot].ask_size9 = temp->ask_size[8];
	marketsnapshots_[pos_of_marketsnapshot].ask_size10 = temp->ask_size[9];
	marketsnapshots_[pos_of_marketsnapshot].transactions_count = temp->transactions_count;
	marketsnapshots_[pos_of_marketsnapshot].weighted_average_bid_price = temp->weighted_average_best_bid;
	marketsnapshots_[pos_of_marketsnapshot].weighted_average_ask_price = temp->weighted_average_best_ask;
	marketsnapshots_[pos_of_marketsnapshot].total_bid_size = temp->l2_total_bid_qty;
	marketsnapshots_[pos_of_marketsnapshot].total_ask_size = temp->l2_total_offer_qty;
	marketsnapshots_[pos_of_marketsnapshot].peratio1 = temp->peratio1;
	marketsnapshots_[pos_of_marketsnapshot].peratio2 = temp->peratio2;

}

void wmdf::SSEStepRawDataConverter::convertOrderqueue( uint32_t pos_of_orderqueue, WindStepMessage *wsm )
{
	OrderQueueImage* temp = (OrderQueueImage*)wsm->Data;
	memcpy(orderqueues_[pos_of_orderqueue].wind_code,temp->wind_code,sizeof(orderqueues_->wind_code));
	memcpy(&orderqueues_[pos_of_orderqueue].wind_code[6], sse_suffix, 4);
	orderqueues_[pos_of_orderqueue].data_time_stamp = DateTime::hms2time(temp->date_time_stamp)*1000;
	orderqueues_[pos_of_orderqueue].image_status = temp->image_status;
	orderqueues_[pos_of_orderqueue].price_level_count = temp->price_level_count;
	orderqueues_[pos_of_orderqueue].order_side = temp->side[0];
	if(temp->price_level_count>0)
	{
		orderqueues_[pos_of_orderqueue].price_levels = new L2OrderQueuePriceLevel[1];
		orderqueues_[pos_of_orderqueue].price_levels->operator_type = temp->order_queue_levels->price_level_operator;
		orderqueues_[pos_of_orderqueue].price_levels->order_price = temp->order_queue_levels->price;
		orderqueues_[pos_of_orderqueue].price_levels->total_number = temp->order_queue_levels->orders_total;
		orderqueues_[pos_of_orderqueue].price_levels->orders_number = temp->order_queue_levels->orders_count;
		orderqueues_[pos_of_orderqueue].price_levels->order_items = new L2OrderQueueItem[temp->order_queue_levels->orders_count];
		for(uint32_t i = 0; i!=temp->order_queue_levels->orders_count; ++i)
		{
			orderqueues_[pos_of_orderqueue].price_levels->order_items[i].size = temp->order_queue_levels->order_queue_items[i].order_qty;
			orderqueues_[pos_of_orderqueue].price_levels->order_items[i].operator_type = temp->order_queue_levels->order_queue_items[i].order_queue_operator;
			orderqueues_[pos_of_orderqueue].price_levels->order_items[i].entry_id = temp->order_queue_levels->order_queue_items[i].order_queue_operator_entry_id;
		}
	}
}

void wmdf::SSEStepRawDataConverter::convertTransaction( uint32_t pos_of_transaction, WindStepMessage* wsm )
{
	Transaction* temp = (Transaction*)wsm->Data;
	memcpy(transactions_[pos_of_transaction].wind_code,temp->wind_code,sizeof(transactions_->wind_code));
	memcpy(&transactions_[pos_of_transaction].wind_code[6], sse_suffix,4);
	transactions_[pos_of_transaction].trade_time = DateTime::hms2time(temp->trade_time)*1000;
	transactions_[pos_of_transaction].set_no = temp->set_no;
	transactions_[pos_of_transaction].rec_no = temp->rec_no;
	transactions_[pos_of_transaction].buyOrderRecNo = temp->buyOrderRecNo;
	transactions_[pos_of_transaction].sellOrderRecNo = temp->sellOrderRecNo;
	transactions_[pos_of_transaction].price = temp->price;
	transactions_[pos_of_transaction].volume = temp->trade_qty;
	transactions_[pos_of_transaction].order_kind = temp->order_kind;
	transactions_[pos_of_transaction].function_code = temp->function_code;
	transactions_[pos_of_transaction].amount = temp->trade_money;
}

void wmdf::SSEStepRawDataConverter::convertVirtualauctionprice( uint32_t pos_of_marketsnapshot, WindStepMessage* wsm )
{
	VirtualAuctionPrice* temp = (VirtualAuctionPrice*)wsm->Data;
	memcpy(marketsnapshots_[pos_of_marketsnapshot].wind_code,temp->windcode,sizeof(temp->windcode));
	memcpy(&marketsnapshots_[pos_of_marketsnapshot].wind_code[6],sse_suffix,4);
	if(temp->bidleaveqty!=0)
	{
		marketsnapshots_[pos_of_marketsnapshot].bid_price1 = temp->price;
		marketsnapshots_[pos_of_marketsnapshot].bid_price2 = 0;
		marketsnapshots_[pos_of_marketsnapshot].bid_size1 = temp->virtualauctionqty;
		marketsnapshots_[pos_of_marketsnapshot].bid_size2 = temp->bidleaveqty;
    marketsnapshots_[pos_of_marketsnapshot].ask_price1 = 0;
    marketsnapshots_[pos_of_marketsnapshot].ask_price2 = 0;
    marketsnapshots_[pos_of_marketsnapshot].ask_size1 = 0;
    marketsnapshots_[pos_of_marketsnapshot].ask_size2 = 0;
	}
	else if(temp->askleaveqty!=0)
	{
		marketsnapshots_[pos_of_marketsnapshot].ask_price1 = temp->price;
		marketsnapshots_[pos_of_marketsnapshot].ask_price2 = 0;
		marketsnapshots_[pos_of_marketsnapshot].ask_size1 = temp->virtualauctionqty;
		marketsnapshots_[pos_of_marketsnapshot].ask_size2 = temp->askleaveqty;
    marketsnapshots_[pos_of_marketsnapshot].bid_price1 = 0;
    marketsnapshots_[pos_of_marketsnapshot].bid_price2 = 0;
    marketsnapshots_[pos_of_marketsnapshot].bid_size1 = 0;
    marketsnapshots_[pos_of_marketsnapshot].bid_size2 = 0;
	}

}

void wmdf::SSEStepRawDataConverter::convertIndexData( uint32_t pos_of_indexdata, WindStepMessage* wsm )
{
	IndexData *temp = (IndexData*)wsm->Data;
	memcpy(indexdata_[pos_of_indexdata].wind_code,temp->SecurityID,sizeof(temp->SecurityID));
	memcpy(&indexdata_[pos_of_indexdata].wind_code[6],sse_suffix,4);
	indexdata_[pos_of_indexdata].data_time_stamp = DateTime::hms2time(temp->DataTimeStamp)*1000;
	indexdata_[pos_of_indexdata].pre_close = temp->PreClosePx;
	indexdata_[pos_of_indexdata].high_price = temp->HighPx;
	indexdata_[pos_of_indexdata].low_price = temp->LowPx;
	indexdata_[pos_of_indexdata].new_price = temp->LastPx;
	indexdata_[pos_of_indexdata].open_price = temp->OpenPx;
	indexdata_[pos_of_indexdata].total_amount = temp->total_amount;
	indexdata_[pos_of_indexdata].total_volume = temp->volume;
}
