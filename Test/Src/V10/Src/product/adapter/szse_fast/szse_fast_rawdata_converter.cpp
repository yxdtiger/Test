#include "szse_fast_rawdata_converter.h"
#include "../../../environment/security_utils.h"
#include "../../../include/date_time.h"
#include "../../../protocol/mdf_message.h"
#include "../../../session/message_publisher.h"
#include "fast_szse_template_object.h"

wmdf::SZSEFastRawDataConverter::SZSEFastRawDataConverter()
{

}

wmdf::SZSEFastRawDataConverter::~SZSEFastRawDataConverter()
{
}

const char* wmdf::SZSEFastRawDataConverter::szse_suffix = ".SZ";


void wmdf::SZSEFastRawDataConverter::convertMarketsnapshot( WindFastMessage* msg,uint32_t& pack_id )
{
	SZSEL2_Snapshot_103 *temp = (SZSEL2_Snapshot_103*)msg->Data;
	int32_t size = msg->Size;

	L2MarketSnapshot *marketdata = new L2MarketSnapshot[size];
	SecurityUtils::initialize_l2_market(marketdata,size);

	num_of_orderqueue_ = 0;

	for(int32_t i=0;i!=size;++i)
	{
		if(temp[i].nOfferPriceLevelNum>0 && temp[i].pOfferPriceLevel->nSequenceLength>0)
			++num_of_orderqueue_;
		if(temp[i].nBidPriceLevelNum>0 && temp[i].pBidPriceLevel->nSequenceLength>0)
			++num_of_orderqueue_;

		memcpy(marketdata[i].wind_code,temp[i].SecurityID,sizeof(marketdata->wind_code));
		memcpy(&marketdata[i].wind_code[6],szse_suffix,4);
// 		if(strcmp((char*)temp[i].SecurityID,"000656")==0 && temp[i].DataTimeStamp>92500000)
// 		{
// 			int a=0;
// 		}
		marketdata[i].data_time_stamp = DateTime::hmsms2time(temp[i].DataTimeStamp);
		marketdata[i].pre_close =	temp[i].PreClosePx;
		marketdata[i].open_price = temp[i].OpenPx;
		marketdata[i].high_price = temp[i].HighPx;
		marketdata[i].low_price = temp[i].LowPx;
		marketdata[i].new_price = temp[i].LastPx;
		marketdata[i].total_volume = temp[i].TotalVolumeTrade;
		marketdata[i].total_amount = temp[i].TotalValueTrade;
		marketdata[i].transactions_count = temp[i].NumTrades;
		marketdata[i].weighted_average_bid_price = temp[i].WeightedAvgBidPx;
		marketdata[i].weighted_average_ask_price = temp[i].WeightedAvgOfferPx;
		marketdata[i].total_bid_size = temp[i].TotalBidQty;
		marketdata[i].total_ask_size = temp[i].TotalOfferQty;
		marketdata[i].peratio1 = temp[i].PERatio1;
		marketdata[i].peratio2 = temp[i].PERatio2;

		std::string windcode = marketdata[i].wind_code;
		std::map<std::string,int8_t>::iterator iter = trade_status_.find(windcode);
		if(iter!=trade_status_.end())
			marketdata[i].trade_status = iter->second;

		double *price = &marketdata[i].bid_price1;
		int32_t *size = &marketdata[i].bid_size1;

		for(int32_t k=0;k!=temp[i].nBidPriceLevelNum;++k)
		{
			*price = temp[i].pBidPriceLevel[k].BidorOfferPx;
			*size = (int32_t)temp[i].pBidPriceLevel[k].BidorOfferSize;
			price++;
			size++;
		}
    //处理跌停板的情况
    for(int32_t index=temp[i].nBidPriceLevelNum;index < 10;index++)
    {
      *price = 0;
      *size = 0;
      price++;
      size++;
    }

		price = &marketdata[i].ask_price1;
		size = &marketdata[i].ask_size1;
		for(int32_t k=0;k!=temp[i].nOfferPriceLevelNum;++k)
		{
			*price = temp[i].pOfferPriceLevel[k].BidorOfferPx;
			*size = (int32_t)temp[i].pOfferPriceLevel[k].BidorOfferSize;
			price++;
			size++;
		}
    //处理涨停板的情况
    for(int32_t index=temp[i].nOfferPriceLevelNum;index < 10;index++)
    {
      *price = 0;
      *size = 0;
      price++;
      size++;
    }
	}


	MdfMessage* mdf_msg = new MdfMessage();
	mdf_msg->set_local_time(DateTime::get_current_time());
	mdf_msg->set_socket_id(0);
	mdf_msg->set_product_id(PRODUCT_SZSE_LEVEL2_ID);
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


void wmdf::SZSEFastRawDataConverter::convertOrderqueue( WindFastMessage *msg, uint32_t &pack_id )
{
	SZSEL2_Snapshot_103 *temp = (SZSEL2_Snapshot_103*)msg->Data;
	int32_t size = msg->Size;

	L2OrderQueue *orderqueue = new L2OrderQueue[num_of_orderqueue_];
	SecurityUtils::initialize_orderqueue(orderqueue,num_of_orderqueue_);

	for(int32_t i=0,pos=0;i!=size;++i)
	{
		if(temp[i].nOfferPriceLevelNum>0 && temp[i].pOfferPriceLevel->nSequenceLength>0)
		{
			orderqueue[pos].data_time_stamp = DateTime::hmsms2time(temp[i].DataTimeStamp*1000);
			orderqueue[pos].image_status = 1;
			orderqueue[pos].order_side = 2;
			orderqueue[pos].price_level_count = 1;
			orderqueue[pos].price_levels = new L2OrderQueuePriceLevel();
			orderqueue[pos].price_levels->operator_type = 0;
			orderqueue[pos].price_levels->order_price = temp[i].pOfferPriceLevel->BidorOfferPx;
			orderqueue[pos].price_levels->total_number = temp[i].pOfferPriceLevel->NumOrders;
			orderqueue[pos].price_levels->orders_number = temp[i].pOfferPriceLevel->nSequenceLength;
			orderqueue[pos].price_levels->order_items = new L2OrderQueueItem[orderqueue[pos].price_levels->orders_number];
			L2OrderQueueItem *orderitems = orderqueue[pos].price_levels->order_items;
			for(int32_t j=0;j!=orderqueue[pos].price_levels->orders_number;++j)
			{
				orderitems[j].size = temp[i].pOfferPriceLevel->pChildSequence[j].OrderQty;
				orderitems[j].entry_id = 0;
				orderitems[j].operator_type = 0;
			}
			++pos;
		}

		if(temp[i].nBidPriceLevelNum>0 && temp[i].pBidPriceLevel->nSequenceLength>0)
		{
			orderqueue[pos].data_time_stamp = DateTime::hmsms2time(temp[i].DataTimeStamp*1000);
			orderqueue[pos].image_status = 1;
			orderqueue[pos].order_side = 1;
			orderqueue[pos].price_level_count = 1;
			orderqueue[pos].price_levels = new L2OrderQueuePriceLevel();
			orderqueue[pos].price_levels->operator_type = 0;
			orderqueue[pos].price_levels->order_price = temp[i].pBidPriceLevel->BidorOfferPx;
			orderqueue[pos].price_levels->total_number = temp[i].pBidPriceLevel->NumOrders;
			orderqueue[pos].price_levels->orders_number = temp[i].pBidPriceLevel->nSequenceLength;
			orderqueue[pos].price_levels->order_items = new L2OrderQueueItem[orderqueue[pos].price_levels->orders_number];
			L2OrderQueueItem *orderitems = orderqueue[pos].price_levels->order_items;
			for(int32_t j=0;j!=orderqueue[pos].price_levels->orders_number;++j)
			{
				orderitems[j].size = temp[i].pBidPriceLevel->pChildSequence[j].OrderQty;
				orderitems[j].entry_id = 0;
				orderitems[j].operator_type = 0;
			}
			++pos;
		}
	}

	MdfMessage* mdf_msg = new MdfMessage();
	mdf_msg->set_local_time(DateTime::get_current_time());
	mdf_msg->set_socket_id(0);
	mdf_msg->set_product_id(PRODUCT_SZSE_LEVEL2_ID);
	mdf_msg->set_message_id(SSEL2_ORDERQUEUE_ID);
	mdf_msg->set_major_version(MAJOR_VERSION_SSE_LEVEL2_ID);
	mdf_msg->set_minor_version(MINOR_VERSION_SSE_LEVEL2_ID);
	mdf_msg->set_record_count(num_of_orderqueue_);

	MarketMessageHeader* message_header = new MarketMessageHeader();
	message_header->place_holder = 0;
	mdf_msg->set_packet_num(++pack_id);
	mdf_msg->set_header(message_header);
	mdf_msg->set_body(orderqueue);

	SharedMdfMessage message = SharedMdfMessage(mdf_msg);
	MessagePublisher::PublishOneMessage(message);
}




void wmdf::SZSEFastRawDataConverter::convertTransaction( WindFastMessage* msg, uint32_t& pack_id)
{
	int32_t size = msg->Size;
	L2Transaction *transaction = new L2Transaction[size];
	SecurityUtils::initialize_transaction(transaction,size);
	SZSEL2_Trade_202 *temp = (SZSEL2_Trade_202*)msg->Data;

	for(int32_t i=0;i!=size;++i)
	{
		memcpy(transaction[i].wind_code,temp[i].SecurityID,sizeof(transaction->wind_code));
		memcpy(&transaction[i].wind_code[6],szse_suffix,4);
		int32_t time = temp[i].TradeTime;
		transaction[i].trade_time = DateTime::hmsms2time(time);
		transaction[i].set_no = temp[i].SetNo;
		transaction[i].rec_no = temp[i].RecNo;
		transaction[i].buyOrderRecNo = temp[i].BuyOrderRecNo;
		transaction[i].sellOrderRecNo = temp[i].SellOrderRecNo;
		transaction[i].price = temp[i].Price;
		transaction[i].volume = temp[i].TradeQty;
		transaction[i].order_kind = temp[i].OrderKind[0];
		transaction[i].function_code = temp[i].FunctionCode[0];
	}

	MdfMessage* mdf_msg = new MdfMessage();
	mdf_msg->set_local_time(DateTime::get_current_time());
	mdf_msg->set_socket_id(0);
	mdf_msg->set_product_id(PRODUCT_SZSE_LEVEL2_ID);
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


void wmdf::SZSEFastRawDataConverter::convertIndexData( WindFastMessage* msg,uint32_t&pack_id )
{
	int32_t size = msg->Size;
	IndexSnapshot *indexdata = new IndexSnapshot[size];
	SecurityUtils::initialize_indexdata(indexdata,size);
	SZSEL2_Index_104 *temp = (SZSEL2_Index_104*)msg->Data;
	for(int32_t i=0;i!=size;++i)
	{
		memcpy(indexdata[i].wind_code,temp[i].SecurityID,sizeof(indexdata[i].wind_code));
		memcpy(&indexdata[i].wind_code[6],szse_suffix,4);
		indexdata[i].data_time_stamp = DateTime::hmsms2time(temp[i].DataTimeStamp);
		indexdata[i].pre_close = temp[i].CloseIndex;
		indexdata[i].high_price = temp[i].HighIndex;
		indexdata[i].low_price = temp[i].LowIndex;
		indexdata[i].new_price = temp[i].LastIndex;
		indexdata[i].open_price = temp[i].OpenIndex;
		indexdata[i].total_amount = temp[i].TurnOver;
		indexdata[i].total_volume = temp[i].TotalVolumeTraded;
	}

	MdfMessage* mdf_msg = new MdfMessage();
	mdf_msg->set_local_time(DateTime::get_current_time());
	mdf_msg->set_socket_id(0);
	mdf_msg->set_product_id(PRODUCT_SZSE_LEVEL2_ID);
	mdf_msg->set_message_id(SZSEL2_INDEXDATA_ID);
	mdf_msg->set_major_version(MAJOR_VERSION_SZSE_LEVEL2_ID);
	mdf_msg->set_minor_version(MINOR_VERSION_SZSE_LEVEL2_ID);
	mdf_msg->set_record_count(size);

	MarketMessageHeader* message_header = new MarketMessageHeader();
	message_header->place_holder = 0;
	mdf_msg->set_packet_num(++pack_id);
	mdf_msg->set_header(message_header);
	mdf_msg->set_body(indexdata);

	SharedMdfMessage message = SharedMdfMessage(mdf_msg);
	MessagePublisher::PublishOneMessage(message);
	
}

void wmdf::SZSEFastRawDataConverter::convertOrders( WindFastMessage *msg,uint32_t &pack_id )
{
	int32_t size = msg->Size;
	SZSEL2Order *ordersdata = new SZSEL2Order[size];
	SecurityUtils::initialize_orders(ordersdata,size);
	SZSEL2_Order_201 *temp = (SZSEL2_Order_201*)msg->Data;
	for(int32_t i=0;i!=size;++i)
	{
		memcpy(ordersdata[i].wind_code,temp[i].SecurityID,sizeof(ordersdata[i].wind_code));
		memcpy(&ordersdata[i].wind_code[6],szse_suffix,4);
		ordersdata[i].order_time = DateTime::hmsms2time(temp[i].OrderEntryTime);
		ordersdata[i].function_code = temp[i].FunctionCode[0];
		ordersdata[i].order_kind = temp[i].OrderKind[0];
		ordersdata[i].price = temp[i].Price;
		ordersdata[i].rec_no = temp[i].RecNo;
		ordersdata[i].set_no = temp[i].SetNo;
		ordersdata[i].size = temp[i].OrderQty;
	}

	MdfMessage* mdf_msg = new MdfMessage();
	mdf_msg->set_local_time(DateTime::get_current_time());
	mdf_msg->set_socket_id(0);
	mdf_msg->set_product_id(PRODUCT_SZSE_LEVEL2_ID);
	mdf_msg->set_message_id(SZSEL2_SINGLEORDER_ID);
	mdf_msg->set_major_version(MAJOR_VERSION_SZSE_LEVEL2_ID);
	mdf_msg->set_minor_version(MINOR_VERSION_SZSE_LEVEL2_ID);
	mdf_msg->set_record_count(size);

	MarketMessageHeader* message_header = new MarketMessageHeader();
	message_header->place_holder = 0;
	mdf_msg->set_packet_num(++pack_id);
	mdf_msg->set_header(message_header);
	mdf_msg->set_body(ordersdata);

	SharedMdfMessage message = SharedMdfMessage(mdf_msg);
	MessagePublisher::PublishOneMessage(message);
}

void wmdf::SZSEFastRawDataConverter::convertTradeStatus( WindFastMessage *msg )
{
	SZSEL2_StockInfo_101 *temp = (SZSEL2_StockInfo_101*)msg->Data;
	std::string windcode = "";
	windcode +=(char*)temp->SecurityID;
	windcode += ".SZ";

	uint32_t securitytradingstatus = temp->SecurityTradingStatus;
	int8_t trade_status;

	//2 = 停牌（Trading Halt）
	//3 = 恢复（Resume）
	//101 = 首日上市
	//102 = 增发新股
	//103 = 正常状态
	//104 = 上网定价发行
	//105 = 上网竞价发行
	//106 = 国债挂牌分销
	//107 = 长期停牌


	switch(securitytradingstatus)
	{
	case 2:
	case 107:
	case 104:
	case 105:
	case 106:
		trade_status = 2; //停牌
		break;
	case 3:
	case 101:
	case 102:
	case 103:
		trade_status = 1;  //正常交易
		break;
	default:
		trade_status = WMDF_CONSTS_8BIT_NIL_VALUE;
		break;
	}

	std::map<std::string,int8_t>::iterator iter = trade_status_.find(windcode);
	if(iter==trade_status_.end())
		trade_status_.insert(make_pair(windcode,trade_status));
	else
		iter->second = trade_status;
}


