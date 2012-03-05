#include "gava_feed.h"
#include <iostream>
#include "../../include/date_time.h"
#include "../../include/wmdf_api.h"
#include "../../include/wmdf_structs.h"
#include "../../monitor/logger.h"
#include "feed_info.h"

#ifdef WIN32
#include <windows.h>
#else
#include <pthread.h>
#include <unistd.h>
#endif

using namespace wmdf;


void CallBackFunc(RecordArgs* recordArgs)
{
  if(NULL!=recordArgs)
  {
    GavaFeed* gf = (GavaFeed*)recordArgs->state;
    if(NULL != gf)
      gf->Process(recordArgs->record);
    wmdf_release_record(recordArgs);
  }
}

wmdf::GavaFeed::GavaFeed(FeedInfo* feed_info)
{
  market_data_container_ = new MarketSnapshotContainer();
  feed_info_ = feed_info;
	sse_trade_index_ = 0;
	szse_trade_index_ = 0;
	cffel1_trade_index_ = 0;
	dcel1_trade_index_ = 0;
	shfel1_trade_index_ = 0;
	czcel1_trade_index_ = 0;
	cffel2_trade_index_ = 0;
  isworking_ = false;
  //clear_cache_time_ = 233000;
  //next_date_ = DateTime::today();
}

wmdf::GavaFeed::~GavaFeed(void)
{
  if (NULL != market_data_container_)
  {
    delete market_data_container_;
    market_data_container_ = NULL;
  }
  feed_info_ = NULL;
}

void wmdf::GavaFeed::Process(MdfRecord* record )
{
  if(record != NULL)
  {
    if (IsProductExistedInIdMap(record->product_id))
    {
      if (packet_id_map_[record->product_id]+1!=record->packet_num)
      {
        Logger::instance()->WriteFormatLog(LL_INFO,"%s  packet lost. product id: %d, pre_packet_num：%d, curr_packet_num: %d ",DateTime::now2str().c_str(),record->product_id,packet_id_map_[record->product_id],record->packet_num);
      }
      packet_id_map_[record->product_id] = record->packet_num;
    }
    else
    {
      packet_id_map_.insert(make_pair(record->product_id,record->packet_num));
    }
    if(record->product_id!=PRODEUCT_SYSTEM_ID)
    {
      if(GetGavaHandle(record->product_id)==WMDF_CONSTS_32BIT_NIL_VALUE)
        return;
    }

    //上交所level1
    if(record->product_id==PRODUCT_SSE_LEVEL1_ID)
    {
      if(record->message_id==SSEL1_MARKETDATA_ID)
      {
				ProcessSHSZL1MarketData(record);
				ProcessSHSZL1Transaction(record,sse_trade_index_);
      }
    }
    //深交所level1
    else if(record->product_id==PRODUCT_SZSE_LEVEL1_ID)
    {
      if(record->message_id==SZSEL1_MARKETDATA_ID)
      {
        ProcessSHSZL1MarketData(record);
				ProcessSHSZL1Transaction(record,szse_trade_index_);
      }
    }
    //上交所level2
    else if(record->product_id==PRODUCT_SSE_LEVEL2_ID)
    {
      if(record->message_id==SSEL2_MARKETDATA_ID)
      {
        ProcessSSEL2MarketData(record);
      }
      else if(record->message_id==SSEL2_TRANSACTION_ID)
      {
        ProcessSSEL2Transaction(record);
      }
      else if (record->message_id==SSEL2_INDEXDATA_ID)
      {
        ProcessIndexSnapshot(record,"index");
      }
//       else if(record->message_id == SSEL2_ORDERQUEUE_ID)
//       {
//         ProcessL2OrderQueues(record);
//       }
    }
    //深交所L2
    else if(record->product_id==PRODUCT_SZSE_LEVEL2_ID)
    {
      if(record->message_id==SZSEL2_MARKETDATA_ID)
      {
        ProcessSZSEL2MarketData(record);
      }
      else if(record->message_id==SZSEL2_TRANSACTION_ID)
      {
        ProcessSZSEL2Transaction(record);
      }
      else if (record->message_id==SZSEL2_INDEXDATA_ID)
      {
        ProcessIndexSnapshot(record,"index");
      }
//       else if(record->message_id == SZSEL2_ORDERQUEUE_ID)
//       {
//         ProcessL2OrderQueues(record);
//       }
      else if(record->message_id == SZSEL2_SINGLEORDER_ID)
      {
        ProcessSZSEL2Order(record);
      }
    }
    //新华富时L1
    else if(record->product_id == PRODUCT_FTSE_LEVEL1_ID)
    {
      ProcessIndexSnapshot(record,"xi_index");
    }
    //中金所L1
    else if(record->product_id==PRODUCT_CFFE_LEVEL1_ID)
    {
      ProcessCFFEL1MarketData(record);
			ProcessFutureL1TransactionData(record, cffel1_trade_index_,"cffel1_tbt");
    }
    //中金所L2
    else if(record->product_id==PRODUCT_CFFE_LEVEL2_ID)
    {
      ProcessCFFEL2MarketData(record);
			ProcessCFFEL2TransactionData(record,cffel2_trade_index_,"cffel2_tbt");
    }
    //大商所L2
    else if(record->product_id==PRODUCT_DCE_LEVEL1_ID)
    {
      ProcessDCEMarketData(record);
			ProcessFutureL1TransactionData(record, dcel1_trade_index_, "dce_tbt");
    }
    //上期所L1
    else if(record->product_id==PRODUCT_SHFE_LEVEL1_ID)
    {
      ProcessSHFEMarketData(record);
			ProcessFutureL1TransactionData(record, shfel1_trade_index_, "shfe_tbt");
    }
    //郑商所L1
    else if(record->product_id==PRODUCT_CZCE_LEVEL1_ID)
    {
      ProcessCZCEMarketData(record);
			ProcessFutureL1TransactionData(record, czcel1_trade_index_, "czce_tbt");
    }
		else if(record->product_id == PRODUCT_CIQINDEX_LEVEL1_ID)
		{
			ProcessIndexSnapshot(record,"ci_index");
		}
		else if(record->product_id == PRODUCT_CSINDEX_LEVEL1_ID)
		{
			ProcessIndexSnapshot(record,"csi_index");
		}
		else if(record->product_id == PRODUCT_WINDINDEX_LEVEL1_ID)
		{
			ProcessIndexSnapshot(record,"wi_index");
		}
    else if(record->product_id == PRODEUCT_SYSTEM_ID)
    {
      if(record->message_id==SYSTEM_MARKET_DATE_SIGNAL_ID)
      {
        ProcessMarketDateSignal(record);
      }
    }
  }
}

void wmdf::GavaFeed::ProcessSHSZL1MarketData( MdfRecord* record )
{
  GValue time=gvector(LUA_TTIME,record->record_count);
  GValue local_time=gvector(LUA_TTIME,record->record_count);
  GValue sym=gvector(LUA_TSTRING,record->record_count);

  GValue pre_close=gvector(LUA_TNUMBER,record->record_count);
  GValue open=gvector(LUA_TNUMBER,record->record_count);
  GValue high=gvector(LUA_TNUMBER,record->record_count);
  GValue low=gvector(LUA_TNUMBER,record->record_count);
  GValue new_price=gvector(LUA_TNUMBER,record->record_count);

  GValue volume=gvector(LUA_TLONG,record->record_count);
  GValue amount=gvector(LUA_TNUMBER,record->record_count);


  GValue bid_size1=gvector(LUA_TINT,record->record_count);
  GValue bid_size2=gvector(LUA_TINT,record->record_count);
  GValue bid_size3=gvector(LUA_TINT,record->record_count);
  GValue bid_size4=gvector(LUA_TINT,record->record_count);
  GValue bid_size5=gvector(LUA_TINT,record->record_count);


  GValue bid_price1=gvector(LUA_TNUMBER,record->record_count);
  GValue bid_price2=gvector(LUA_TNUMBER,record->record_count);
  GValue bid_price3=gvector(LUA_TNUMBER,record->record_count);
  GValue bid_price4=gvector(LUA_TNUMBER,record->record_count);
  GValue bid_price5=gvector(LUA_TNUMBER,record->record_count);

  GValue ask_size1=gvector(LUA_TINT,record->record_count);
  GValue ask_size2=gvector(LUA_TINT,record->record_count);
  GValue ask_size3=gvector(LUA_TINT,record->record_count);
  GValue ask_size4=gvector(LUA_TINT,record->record_count);
  GValue ask_size5=gvector(LUA_TINT,record->record_count);

  GValue ask_price1=gvector(LUA_TNUMBER,record->record_count);
  GValue ask_price2=gvector(LUA_TNUMBER,record->record_count);
  GValue ask_price3=gvector(LUA_TNUMBER,record->record_count);
  GValue ask_price4=gvector(LUA_TNUMBER,record->record_count);
  GValue ask_price5=gvector(LUA_TNUMBER,record->record_count);

  L1MarketSnapshot *marketDatas = (L1MarketSnapshot*)record->body;

  for(int i=0;i<record->record_count;i++)
  {
		L1MarketSnapshot *market_snapshot = &marketDatas[i];
		market_data_container_->MergeL1MarketSnapshot(market_snapshot);
    gI(time)[i] = market_snapshot->data_time_stamp;
    gI(local_time)[i] = record->local_time;
    gS(sym)[i]=ss(market_snapshot->wind_code);
    gE(pre_close)[i] = market_snapshot->pre_close;
    gE(open)[i] = market_snapshot->open_price;
    gE(high)[i] = market_snapshot->high_price;
    gE(low)[i] = market_snapshot->low_price;
    gE(new_price)[i] = market_snapshot->new_price;
    gJ(volume)[i] = market_snapshot->total_volume;
    gE(amount)[i] = market_snapshot->total_amount;

    gE(bid_price1)[i] = market_snapshot->bid_price1;
    gE(bid_price2)[i] = market_snapshot->bid_price2;
    gE(bid_price3)[i] = market_snapshot->bid_price3;
    gE(bid_price4)[i] = market_snapshot->bid_price4;
    gE(bid_price5)[i] = market_snapshot->bid_price5;

    gI(bid_size1)[i] = market_snapshot->bid_size1;
    gI(bid_size2)[i] = market_snapshot->bid_size2;
    gI(bid_size3)[i] = market_snapshot->bid_size3;
    gI(bid_size4)[i] = market_snapshot->bid_size4;
    gI(bid_size5)[i] = market_snapshot->bid_size5;

    gE(ask_price1)[i] = market_snapshot->ask_price1;
    gE(ask_price2)[i] = market_snapshot->ask_price2;
    gE(ask_price3)[i] = market_snapshot->ask_price3;
    gE(ask_price4)[i] = market_snapshot->ask_price4;
    gE(ask_price5)[i] = market_snapshot->ask_price5;

    gI(ask_size1)[i] = market_snapshot->ask_size1;
    gI(ask_size2)[i] = market_snapshot->ask_size2;
    gI(ask_size3)[i] = market_snapshot->ask_size3;
    gI(ask_size4)[i] = market_snapshot->ask_size4;
    gI(ask_size5)[i] = market_snapshot->ask_size5;
  }
  GValue shszl1_mkt =glist(30,time,local_time,sym,pre_close,open,high,low,new_price,volume,amount,
    bid_price1,bid_price2,bid_price3,bid_price4,bid_price5,
    bid_size1,bid_size2,bid_size3,bid_size4,bid_size5,
    ask_price1,ask_price2,ask_price3,ask_price4,ask_price5,
    ask_size1,ask_size2,ask_size3,ask_size4,ask_size5);
  ExecGavaScript(record->product_id,"upd(x,y)",gstring("mkt"),shszl1_mkt);
}

void wmdf::GavaFeed::ProcessSHSZL1Transaction( MdfRecord *record,int32_t &trade_index)
{	
	int32_t num_of_l1transaction = market_data_container_->L1TransactionCount();

	if(num_of_l1transaction>0)
	{
		GValue time       = gvector(LUA_TTIME,num_of_l1transaction);
		GValue local_time = gvector(LUA_TTIME,num_of_l1transaction);
		GValue sym        = gvector(LUA_TSTRING,num_of_l1transaction);
		GValue index      = gvector(LUA_TINT,num_of_l1transaction);
		GValue price      = gvector(LUA_TNUMBER,num_of_l1transaction);
		GValue volume     = gvector(LUA_TINT,num_of_l1transaction);
		GValue amount     = gvector(LUA_TNUMBER,num_of_l1transaction);

		for(int i=0;i!=num_of_l1transaction;++i)
		{
			L1Transaction *marketsnapshot = market_data_container_->GetL1Transaction(i);
			gI(time)[i]       = marketsnapshot->time;
			gI(local_time)[i] = record->local_time;
			gS(sym)[i]				=	ss(marketsnapshot->wind_code);
			gI(index)[i]			= ++trade_index;
			gE(price)[i]      = marketsnapshot->new_price;
			gI(volume)[i]     =	(int32_t)marketsnapshot->delt_volume;
			gE(amount)[i]     = marketsnapshot->delt_amount;
		}

		market_data_container_->ClearL1Transaction();

		GValue data =glist(7,time,local_time,sym,index,price,volume,amount);
		ExecGavaScript(record->product_id,"upd(x,y)",gstring("trades"),data);
	}
}

void wmdf::GavaFeed::ProcessSSEL2MarketData(MdfRecord* record )
{
  GValue time        = gvector(LUA_TTIME,record->record_count);
  GValue local_time  = gvector(LUA_TTIME,record->record_count);
  GValue sym         = gvector(LUA_TSTRING,record->record_count);
  GValue pre_close   = gvector(LUA_TNUMBER,record->record_count);
  GValue open        = gvector(LUA_TNUMBER,record->record_count);
  GValue high        = gvector(LUA_TNUMBER,record->record_count);
  GValue low         = gvector(LUA_TNUMBER,record->record_count);
  GValue new_price   = gvector(LUA_TNUMBER,record->record_count);

  GValue volume      = gvector(LUA_TLONG,record->record_count);
  GValue amount      = gvector(LUA_TNUMBER,record->record_count);
  GValue trans_count = gvector(LUA_TLONG,record->record_count);
  GValue peratio     = gvector(LUA_TNUMBER,record->record_count);

  GValue bid_size1 = gvector(LUA_TINT,record->record_count);
  GValue bid_size2 = gvector(LUA_TINT,record->record_count);
  GValue bid_size3 = gvector(LUA_TINT,record->record_count);
  GValue bid_size4 = gvector(LUA_TINT,record->record_count);
  GValue bid_size5 = gvector(LUA_TINT,record->record_count);
  GValue bid_size6 = gvector(LUA_TINT,record->record_count);
  GValue bid_size7 = gvector(LUA_TINT,record->record_count);
  GValue bid_size8 = gvector(LUA_TINT,record->record_count);
  GValue bid_size9 = gvector(LUA_TINT,record->record_count);
  GValue bid_size10= gvector(LUA_TINT,record->record_count);

  GValue bid_price1  = gvector(LUA_TNUMBER,record->record_count);
  GValue bid_price2  = gvector(LUA_TNUMBER,record->record_count);
  GValue bid_price3  = gvector(LUA_TNUMBER,record->record_count);
  GValue bid_price4  = gvector(LUA_TNUMBER,record->record_count);
  GValue bid_price5  = gvector(LUA_TNUMBER,record->record_count);
  GValue bid_price6  = gvector(LUA_TNUMBER,record->record_count);
  GValue bid_price7  = gvector(LUA_TNUMBER,record->record_count);
  GValue bid_price8  = gvector(LUA_TNUMBER,record->record_count);
  GValue bid_price9  = gvector(LUA_TNUMBER,record->record_count);
  GValue bid_price10 = gvector(LUA_TNUMBER,record->record_count);

  GValue ask_size1=gvector(LUA_TINT,record->record_count);
  GValue ask_size2=gvector(LUA_TINT,record->record_count);
  GValue ask_size3=gvector(LUA_TINT,record->record_count);
  GValue ask_size4=gvector(LUA_TINT,record->record_count);
  GValue ask_size5=gvector(LUA_TINT,record->record_count);
  GValue ask_size6=gvector(LUA_TINT,record->record_count);
  GValue ask_size7=gvector(LUA_TINT,record->record_count);
  GValue ask_size8=gvector(LUA_TINT,record->record_count);
  GValue ask_size9=gvector(LUA_TINT,record->record_count);
  GValue ask_size10=gvector(LUA_TINT,record->record_count);

  GValue ask_price1=gvector(LUA_TNUMBER,record->record_count);
  GValue ask_price2=gvector(LUA_TNUMBER,record->record_count);
  GValue ask_price3=gvector(LUA_TNUMBER,record->record_count);
  GValue ask_price4=gvector(LUA_TNUMBER,record->record_count);
  GValue ask_price5=gvector(LUA_TNUMBER,record->record_count);
  GValue ask_price6=gvector(LUA_TNUMBER,record->record_count);
  GValue ask_price7=gvector(LUA_TNUMBER,record->record_count);
  GValue ask_price8=gvector(LUA_TNUMBER,record->record_count);
  GValue ask_price9=gvector(LUA_TNUMBER,record->record_count);
  GValue ask_price10=gvector(LUA_TNUMBER,record->record_count);

  L2MarketSnapshot* marketDatas = (L2MarketSnapshot*)record->body;

  for(int i=0;i<record->record_count;i++)
  {
    L2MarketSnapshot* market_snapshot = &marketDatas[i];
    market_data_container_->MergeL2MarketSnapshot(market_snapshot);
    gI(time)[i]=market_snapshot->data_time_stamp;
    gI(local_time)[i]=record->local_time;
    gS(sym)[i]=ss(market_snapshot->wind_code);
    gE(pre_close)[i]=market_snapshot->pre_close;
    gE(open)[i]=market_snapshot->open_price;
    gE(high)[i]=market_snapshot->high_price;
    gE(low)[i]=market_snapshot->low_price;
    gE(new_price)[i]=market_snapshot->new_price;
    gJ(volume)[i]=market_snapshot->total_volume;
    gE(amount)[i]=market_snapshot->total_amount;
    gJ(trans_count)[i]=market_snapshot->transactions_count;
    gE(peratio)[i] = market_snapshot->peratio2;
    gE(bid_price1)[i]=market_snapshot->bid_price1;
    gE(bid_price2)[i]=market_snapshot->bid_price2;
    gE(bid_price3)[i]=market_snapshot->bid_price3;
    gE(bid_price4)[i]=market_snapshot->bid_price4;
    gE(bid_price5)[i]=market_snapshot->bid_price5;
    gE(bid_price6)[i]=market_snapshot->bid_price6;
    gE(bid_price7)[i]=market_snapshot->bid_price7;
    gE(bid_price8)[i]=market_snapshot->bid_price8;
    gE(bid_price9)[i]=market_snapshot->bid_price9;
    gE(bid_price10)[i]=market_snapshot->bid_price10;

    gI(bid_size1)[i]=market_snapshot->bid_size1;
    gI(bid_size2)[i]=market_snapshot->bid_size2;
    gI(bid_size3)[i]=market_snapshot->bid_size3;
    gI(bid_size4)[i]=market_snapshot->bid_size4;
    gI(bid_size5)[i]=market_snapshot->bid_size5;
    gI(bid_size6)[i]=market_snapshot->bid_size6;
    gI(bid_size7)[i]=market_snapshot->bid_size7;
    gI(bid_size8)[i]=market_snapshot->bid_size8;
    gI(bid_size9)[i]=market_snapshot->bid_size9;
    gI(bid_size10)[i]=market_snapshot->bid_size10;

    gE(ask_price1)[i]=market_snapshot->ask_price1;
    gE(ask_price2)[i]=market_snapshot->ask_price2;
    gE(ask_price3)[i]=market_snapshot->ask_price3;
    gE(ask_price4)[i]=market_snapshot->ask_price4;
    gE(ask_price5)[i]=market_snapshot->ask_price5;
    gE(ask_price6)[i]=market_snapshot->ask_price6;
    gE(ask_price7)[i]=market_snapshot->ask_price7;
    gE(ask_price8)[i]=market_snapshot->ask_price8;
    gE(ask_price9)[i]=market_snapshot->ask_price9;
    gE(ask_price10)[i]=market_snapshot->ask_price10;

    gI(ask_size1)[i]=market_snapshot->ask_size1;
    gI(ask_size2)[i]=market_snapshot->ask_size2;
    gI(ask_size3)[i]=market_snapshot->ask_size3;
    gI(ask_size4)[i]=market_snapshot->ask_size4;
    gI(ask_size5)[i]=market_snapshot->ask_size5;
    gI(ask_size6)[i]=market_snapshot->ask_size6;
    gI(ask_size7)[i]=market_snapshot->ask_size7;
    gI(ask_size8)[i]=market_snapshot->ask_size8;
    gI(ask_size9)[i]=market_snapshot->ask_size9;
    gI(ask_size10)[i]=market_snapshot->ask_size10;
  }
  GValue mkt =glist(52,time,local_time,sym,pre_close,open,high,low,new_price,volume,amount,trans_count,peratio,
    bid_price1,bid_price2,bid_price3,bid_price4,bid_price5,bid_price6,bid_price7,bid_price8,bid_price9,bid_price10,
    bid_size1,bid_size2,bid_size3,bid_size4,bid_size5,bid_size6,bid_size7,bid_size8,bid_size9,bid_size10,
    ask_price1,ask_price2,ask_price3,ask_price4,ask_price5,ask_price6,ask_price7,ask_price8,ask_price9,ask_price10,
    ask_size1,ask_size2,ask_size3,ask_size4,ask_size5,ask_size6,ask_size7,ask_size8,ask_size9,ask_size10);
  ExecGavaScript(record->product_id,"upd(x,y)",gstring("mkt"),mkt);
}

void wmdf::GavaFeed::ProcessSSEL2Transaction(MdfRecord* record )
{
  GValue time       = gvector(LUA_TTIME,record->record_count);
  GValue local_time = gvector(LUA_TTIME,record->record_count);
  GValue sym        = gvector(LUA_TSTRING,record->record_count);
  GValue channel    = gvector(LUA_TINT,record->record_count);
  GValue index      = gvector(LUA_TINT,record->record_count);
  GValue price      = gvector(LUA_TNUMBER,record->record_count);
  GValue volume     = gvector(LUA_TINT,record->record_count);
  GValue amount     = gvector(LUA_TNUMBER,record->record_count);

  L2Transaction* trans = (L2Transaction*)record->body;

  for(int i=0;i<record->record_count;i++)
  {
    gI(time)[i]       = trans[i].trade_time;
    gI(local_time)[i] = record->local_time;
    gS(sym)[i]        = ss(trans[i].wind_code);
    gI(channel)[i]    = trans[i].set_no;
    gI(index)[i]      = trans[i].rec_no;
    gE(price)[i]      = trans[i].price;
    gI(volume)[i]     = trans[i].volume;
    gE(amount)[i]     = trans[i].amount;
  }

  GValue data =glist(8,time,local_time,sym,channel,index,price,volume,amount);
  ExecGavaScript(record->product_id,"upd(x,y)",gstring("trades"),data);
}
void wmdf::GavaFeed::ProcessIndexSnapshot( MdfRecord* record,std::string table )
{
  GValue time       = gvector(LUA_TTIME,record->record_count);
  GValue local_time = gvector(LUA_TTIME,record->record_count);
  GValue sym        = gvector(LUA_TSTRING,record->record_count);
  GValue preclose   = gvector(LUA_TNUMBER,record->record_count);
  GValue open       = gvector(LUA_TNUMBER,record->record_count);
  GValue high       = gvector(LUA_TNUMBER,record->record_count);
  GValue low        = gvector(LUA_TNUMBER,record->record_count);
  GValue new_price  = gvector(LUA_TNUMBER,record->record_count);
  GValue volume     = gvector(LUA_TLONG,record->record_count);
  GValue amount     = gvector(LUA_TNUMBER,record->record_count);


  IndexSnapshot* index_snapshots = (IndexSnapshot*)record->body;

  for(int i=0;i<record->record_count;i++)
  {
    IndexSnapshot* index_snapshot = &index_snapshots[i];
    market_data_container_->MergeIndexSnapshot(index_snapshot);
    gI(time)[i]       = index_snapshot->data_time_stamp;
    gI(local_time)[i] = record->local_time;
    gS(sym)[i]        = ss(index_snapshot->wind_code);
    gE(preclose)[i]   = index_snapshot->pre_close;
    gE(open)[i]       = index_snapshot->open_price;
    gE(high)[i]       = index_snapshot->high_price;
    gE(low)[i]        = index_snapshot->low_price;
    gE(new_price)[i]  = index_snapshot->new_price;
    gJ(volume)[i]     = index_snapshot->total_volume;
    gE(amount)[i]     = index_snapshot->total_amount;
  }
  GValue data =glist(10,time,local_time,sym,preclose,open,high,low,new_price,volume,amount);
  ExecGavaScript(record->product_id,"upd(x,y)",gstring(table.c_str()),data);
}
void wmdf::GavaFeed::ProcessSZSEL2MarketData( MdfRecord* record )
{
  GValue time=gvector(LUA_TTIME,record->record_count);
  GValue local_time=gvector(LUA_TTIME,record->record_count);
  GValue sym=gvector(LUA_TSTRING,record->record_count);
  GValue pre_close=gvector(LUA_TNUMBER,record->record_count);
  GValue open=gvector(LUA_TNUMBER,record->record_count);
  GValue high=gvector(LUA_TNUMBER,record->record_count);
  GValue low=gvector(LUA_TNUMBER,record->record_count);
  GValue new_price=gvector(LUA_TNUMBER,record->record_count);

  GValue volume=gvector(LUA_TLONG,record->record_count);
  GValue amount=gvector(LUA_TNUMBER,record->record_count);
  GValue trans_count=gvector(LUA_TLONG,record->record_count);
  GValue peratio1=gvector(LUA_TNUMBER,record->record_count);
  GValue peratio2=gvector(LUA_TNUMBER,record->record_count);


  GValue bid_size1=gvector(LUA_TINT,record->record_count);
  GValue bid_size2=gvector(LUA_TINT,record->record_count);
  GValue bid_size3=gvector(LUA_TINT,record->record_count);
  GValue bid_size4=gvector(LUA_TINT,record->record_count);
  GValue bid_size5=gvector(LUA_TINT,record->record_count);
  GValue bid_size6=gvector(LUA_TINT,record->record_count);
  GValue bid_size7=gvector(LUA_TINT,record->record_count);
  GValue bid_size8=gvector(LUA_TINT,record->record_count);
  GValue bid_size9=gvector(LUA_TINT,record->record_count);
  GValue bid_size10=gvector(LUA_TINT,record->record_count);

  GValue bid_price1=gvector(LUA_TNUMBER,record->record_count);
  GValue bid_price2=gvector(LUA_TNUMBER,record->record_count);
  GValue bid_price3=gvector(LUA_TNUMBER,record->record_count);
  GValue bid_price4=gvector(LUA_TNUMBER,record->record_count);
  GValue bid_price5=gvector(LUA_TNUMBER,record->record_count);
  GValue bid_price6=gvector(LUA_TNUMBER,record->record_count);
  GValue bid_price7=gvector(LUA_TNUMBER,record->record_count);
  GValue bid_price8=gvector(LUA_TNUMBER,record->record_count);
  GValue bid_price9=gvector(LUA_TNUMBER,record->record_count);
  GValue bid_price10=gvector(LUA_TNUMBER,record->record_count);

  GValue ask_size1=gvector(LUA_TINT,record->record_count);
  GValue ask_size2=gvector(LUA_TINT,record->record_count);
  GValue ask_size3=gvector(LUA_TINT,record->record_count);
  GValue ask_size4=gvector(LUA_TINT,record->record_count);
  GValue ask_size5=gvector(LUA_TINT,record->record_count);
  GValue ask_size6=gvector(LUA_TINT,record->record_count);
  GValue ask_size7=gvector(LUA_TINT,record->record_count);
  GValue ask_size8=gvector(LUA_TINT,record->record_count);
  GValue ask_size9=gvector(LUA_TINT,record->record_count);
  GValue ask_size10=gvector(LUA_TINT,record->record_count);

  GValue ask_price1=gvector(LUA_TNUMBER,record->record_count);
  GValue ask_price2=gvector(LUA_TNUMBER,record->record_count);
  GValue ask_price3=gvector(LUA_TNUMBER,record->record_count);
  GValue ask_price4=gvector(LUA_TNUMBER,record->record_count);
  GValue ask_price5=gvector(LUA_TNUMBER,record->record_count);
  GValue ask_price6=gvector(LUA_TNUMBER,record->record_count);
  GValue ask_price7=gvector(LUA_TNUMBER,record->record_count);
  GValue ask_price8=gvector(LUA_TNUMBER,record->record_count);
  GValue ask_price9=gvector(LUA_TNUMBER,record->record_count);
  GValue ask_price10=gvector(LUA_TNUMBER,record->record_count);


  L2MarketSnapshot* marketDatas = (L2MarketSnapshot*)record->body;

  for(int i=0;i<record->record_count;i++)
  {
    L2MarketSnapshot* market_snapshot = &marketDatas[i];
    market_data_container_->MergeL2MarketSnapshot(market_snapshot);

    gI(time)[i]=market_snapshot->data_time_stamp;
    gI(local_time)[i]=record->local_time;
    gS(sym)[i]=ss(market_snapshot->wind_code);
    gE(pre_close)[i]=market_snapshot->pre_close;
    gE(open)[i]=market_snapshot->open_price;
    gE(high)[i]=market_snapshot->high_price;
    gE(low)[i]=market_snapshot->low_price;
    gE(new_price)[i]=market_snapshot->new_price;
    gJ(volume)[i]=market_snapshot->total_volume;
    gE(amount)[i]=market_snapshot->total_amount;
    gJ(trans_count)[i]=market_snapshot->transactions_count;
    gE(peratio1)[i] = market_snapshot->peratio1;
    gE(peratio2)[i] = market_snapshot->peratio2;

    gE(bid_price1)[i]=market_snapshot->bid_price1;
    gE(bid_price2)[i]=market_snapshot->bid_price2;
    gE(bid_price3)[i]=market_snapshot->bid_price3;
    gE(bid_price4)[i]=market_snapshot->bid_price4;
    gE(bid_price5)[i]=market_snapshot->bid_price5;
    gE(bid_price6)[i]=market_snapshot->bid_price6;
    gE(bid_price7)[i]=market_snapshot->bid_price7;
    gE(bid_price8)[i]=market_snapshot->bid_price8;
    gE(bid_price9)[i]=market_snapshot->bid_price9;
    gE(bid_price10)[i]=market_snapshot->bid_price10;

    gI(bid_size1)[i]=market_snapshot->bid_size1;
    gI(bid_size2)[i]=market_snapshot->bid_size2;
    gI(bid_size3)[i]=market_snapshot->bid_size3;
    gI(bid_size4)[i]=market_snapshot->bid_size4;
    gI(bid_size5)[i]=market_snapshot->bid_size5;
    gI(bid_size6)[i]=market_snapshot->bid_size6;
    gI(bid_size7)[i]=market_snapshot->bid_size7;
    gI(bid_size8)[i]=market_snapshot->bid_size8;
    gI(bid_size9)[i]=market_snapshot->bid_size9;
    gI(bid_size10)[i]=market_snapshot->bid_size10;

    gE(ask_price1)[i]=market_snapshot->ask_price1;
    gE(ask_price2)[i]=market_snapshot->ask_price2;
    gE(ask_price3)[i]=market_snapshot->ask_price3;
    gE(ask_price4)[i]=market_snapshot->ask_price4;
    gE(ask_price5)[i]=market_snapshot->ask_price5;
    gE(ask_price6)[i]=market_snapshot->ask_price6;
    gE(ask_price7)[i]=market_snapshot->ask_price7;
    gE(ask_price8)[i]=market_snapshot->ask_price8;
    gE(ask_price9)[i]=market_snapshot->ask_price9;
    gE(ask_price10)[i]=market_snapshot->ask_price10;

    gI(ask_size1)[i]=market_snapshot->ask_size1;
    gI(ask_size2)[i]=market_snapshot->ask_size2;
    gI(ask_size3)[i]=market_snapshot->ask_size3;
    gI(ask_size4)[i]=market_snapshot->ask_size4;
    gI(ask_size5)[i]=market_snapshot->ask_size5;
    gI(ask_size6)[i]=market_snapshot->ask_size6;
    gI(ask_size7)[i]=market_snapshot->ask_size7;
    gI(ask_size8)[i]=market_snapshot->ask_size8;
    gI(ask_size9)[i]=market_snapshot->ask_size9;
    gI(ask_size10)[i]=market_snapshot->ask_size10;
  }
  GValue mkt =glist(53,time,local_time,sym,pre_close,open,high,low,new_price,volume,amount,trans_count,peratio1,peratio2,
    bid_price1,bid_price2,bid_price3,bid_price4,bid_price5,bid_price6,bid_price7,bid_price8,bid_price9,bid_price10,
    bid_size1,bid_size2,bid_size3,bid_size4,bid_size5,bid_size6,bid_size7,bid_size8,bid_size9,bid_size10,
    ask_price1,ask_price2,ask_price3,ask_price4,ask_price5,ask_price6,ask_price7,ask_price8,ask_price9,ask_price10,
    ask_size1,ask_size2,ask_size3,ask_size4,ask_size5,ask_size6,ask_size7,ask_size8,ask_size9,ask_size10);
  ExecGavaScript(record->product_id,"upd(x,y)",gstring("mkt"),mkt);
}

void wmdf::GavaFeed::ProcessSZSEL2Transaction( MdfRecord* record )
{
  GValue time       = gvector(LUA_TTIME,record->record_count);
  GValue local_time = gvector(LUA_TTIME,record->record_count);
  GValue sym        = gvector(LUA_TSTRING,record->record_count);
  GValue price      = gvector(LUA_TNUMBER,record->record_count);
  GValue volume     = gvector(LUA_TINT,record->record_count);
  GValue channel    = gvector(LUA_TINT,record->record_count);
  GValue index      = gvector(LUA_TINT,record->record_count);
  GValue buy_order_no = gvector(LUA_TINT,record->record_count);
  GValue sell_order_no = gvector(LUA_TINT,record->record_count);
  GValue order_kind    = gvector(LUA_TINT,record->record_count);
  GValue function_code = gvector(LUA_TINT,record->record_count);

  L2Transaction* trans = (L2Transaction*)record->body;

  for(int i=0;i<record->record_count;i++)
  {
    gI(time)[i]       = trans[i].trade_time;
    gI(local_time)[i] = record->local_time;
    gS(sym)[i]        = ss(trans[i].wind_code);
    gI(channel)[i]    = trans[i].set_no;
    gI(index)[i]      = trans[i].rec_no;
    gE(price)[i]      = trans[i].price;
    gI(volume)[i]     = trans[i].volume;
    gI(buy_order_no)[i] = trans[i].buyOrderRecNo;
    gI(sell_order_no)[i] = trans[i].sellOrderRecNo;
    gI(order_kind)[i] = trans[i].order_kind;
    gI(function_code)[i] = trans[i].function_code;
  }
  GValue data =glist(11,time,local_time,sym,channel,index,price,volume,buy_order_no,sell_order_no,order_kind,function_code);
  ExecGavaScript(record->product_id,"upd(x,y)",gstring("trades"),data);
}
void wmdf::GavaFeed::ProcessSZSEL2Order( MdfRecord* record )
{
  GValue time=gvector(LUA_TTIME,record->record_count);
  GValue local_time=gvector(LUA_TTIME,record->record_count);
  GValue sym=gvector(LUA_TSTRING,record->record_count);
  GValue price=gvector(LUA_TNUMBER,record->record_count);
  GValue volume=gvector(LUA_TINT,record->record_count);
  GValue set_no=gvector(LUA_TINT,record->record_count);
  GValue rec_no=gvector(LUA_TINT,record->record_count);
  GValue order_kind=gvector(LUA_TINT,record->record_count);
  GValue func_code=gvector(LUA_TINT,record->record_count);


  //single_order=$([],time=[time], sym=[string],price=[number],volume=[int],
  //set_no=[int],rec_no=[int],order_kind=[int],func_code=[int])
  SZSEL2Order* single_orders = (SZSEL2Order*)record->body;

  for(int i=0;i<record->record_count;i++)
  {
    gI(time)[i]=single_orders[i].order_time;
    gI(local_time)[i]=record->local_time;
    gS(sym)[i]=ss(single_orders[i].wind_code);
    gE(price)[i]=single_orders[i].price;
    gI(volume)[i]=single_orders[i].size;
    gI(set_no)[i]=single_orders[i].set_no;
    gI(rec_no)[i]=single_orders[i].rec_no;
    gI(order_kind)[i]=single_orders[i].order_kind;
    gI(func_code)[i]=single_orders[i].function_code;
  }

  GValue data =glist(9,time,local_time,sym,set_no,rec_no,price,volume,order_kind,func_code);
  ExecGavaScript(record->product_id,"upd(x,y)",gstring("orders"),data);
}

void wmdf::GavaFeed::ProcessCFFEL1MarketData(MdfRecord* record)
{
  GValue time=gvector(LUA_TTIME,record->record_count);
  GValue local_time=gvector(LUA_TTIME,record->record_count);
  GValue sym=gvector(LUA_TSTRING,record->record_count);

  GValue pre_close=gvector(LUA_TNUMBER,record->record_count);
  GValue open=gvector(LUA_TNUMBER,record->record_count);
  GValue high=gvector(LUA_TNUMBER,record->record_count);
  GValue low=gvector(LUA_TNUMBER,record->record_count);
  GValue new_price=gvector(LUA_TNUMBER,record->record_count);

  GValue ask_price1=gvector(LUA_TNUMBER,record->record_count);
  GValue ask_size1=gvector(LUA_TINT,record->record_count);
  GValue bid_price1=gvector(LUA_TNUMBER,record->record_count);
  GValue bid_size1=gvector(LUA_TINT,record->record_count);
  GValue settle=gvector(LUA_TNUMBER,record->record_count);
  GValue pre_settle=gvector(LUA_TNUMBER,record->record_count);
  GValue pre_position=gvector(LUA_TLONG,record->record_count);
  GValue position=gvector(LUA_TLONG,record->record_count);

  GValue volume=gvector(LUA_TLONG,record->record_count);
  GValue amount=gvector(LUA_TNUMBER,record->record_count);
  GValue high_limit=gvector(LUA_TNUMBER,record->record_count);
  GValue low_limit=gvector(LUA_TNUMBER,record->record_count);


  FuturesMarketSnapshot* marketDatas = (FuturesMarketSnapshot*)record->body;
  for(int i=0;i<record->record_count;i++)
  {
    gI(time)[i]=marketDatas[i].data_time_stamp;
    gI(local_time)[i]=record->local_time;
    gS(sym)[i]=ss(marketDatas[i].wind_code);

    gE(pre_close)[i]=marketDatas[i].pre_close;
    gE(open)[i]=marketDatas[i].open_price;
    gE(high)[i]=marketDatas[i].high_price;
    gE(low)[i]=marketDatas[i].low_price;
    gE(new_price)[i]=marketDatas[i].new_price;

    gE(bid_price1)[i]=marketDatas[i].bid_price1;
    gI(bid_size1)[i]=marketDatas[i].bid_size1;
    gE(ask_price1)[i]=marketDatas[i].ask_price1;
    gI(ask_size1)[i]=marketDatas[i].ask_size1;

    gJ(volume)[i]=marketDatas[i].total_volume;
    gE(amount)[i]=marketDatas[i].total_amount;
    gE(settle)[i]=marketDatas[i].settle;
    gE(pre_settle)[i]=marketDatas[i].pre_settle;
    gJ(position)[i]=marketDatas[i].position;
    gJ(pre_position)[i]=marketDatas[i].pre_position;
    gE(high_limit)[i]=marketDatas[i].high_limit;
    gE(low_limit)[i]=marketDatas[i].low_limit;

  }
  GValue data =glist(20,time,local_time,sym,pre_close,open,high,low,new_price,
    ask_price1,ask_size1,bid_price1,bid_size1,
    settle,pre_settle,pre_position,position,volume,amount,
    high_limit,low_limit);
  ExecGavaScript(record->product_id,"upd(x,y)",gstring("cffel1_mkt"),data);
}




void wmdf::GavaFeed::ProcessFutureL1TransactionData( MdfRecord *record, int32_t &trade_index, string table)
{
	FuturesMarketSnapshot *marketDatas = (FuturesMarketSnapshot*)record->body;

	FutureTransaction *future_data = new FutureTransaction[record->record_count];

	int32_t num_of_futuretransaction = market_data_container_->ConvertFutureL1Transaction(record->body,record->record_count,future_data);

	GValue time				=	gvector(LUA_TTIME,num_of_futuretransaction);
	GValue local_time	=	gvector(LUA_TTIME,num_of_futuretransaction);
	GValue sym				=	gvector(LUA_TSTRING,num_of_futuretransaction);
	GValue index      = gvector(LUA_TINT,num_of_futuretransaction);
	GValue price			= gvector(LUA_TNUMBER,num_of_futuretransaction);
	GValue volume     = gvector(LUA_TINT,num_of_futuretransaction);
	GValue position		= gvector(LUA_TINT,num_of_futuretransaction);
	GValue status			=	gvector(LUA_TINT,num_of_futuretransaction);


	for(int32_t i=0;i!=num_of_futuretransaction;++i)
	{
		FutureTransaction *data = &future_data[i];
		gI(time)[i] = data->time;
		gI(local_time)[i] = record->local_time;
		gS(sym)[i]		= ss(data->wind_code);
		gI(index)[i] = ++trade_index;
		gE(price)[i]		=	data->new_price;
		gI(volume)[i] = data->delt_volume;
		gI(position)[i] = data->delt_position;
		gI(status)[i] = data->futureoptionstate;
	}

	delete [] future_data;

	GValue data =glist(8,time,local_time,sym,index,price,volume,position,status);
	ExecGavaScript(record->product_id,"upd(x,y)",gstring(table.c_str()),data);
}
void wmdf::GavaFeed::ProcessCFFEL2MarketData(MdfRecord* record)
{
  GValue time=gvector(LUA_TTIME,record->record_count);
  GValue local_time=gvector(LUA_TTIME,record->record_count);
  GValue sym=gvector(LUA_TSTRING,record->record_count);

  GValue pre_close=gvector(LUA_TNUMBER,record->record_count);
  GValue open=gvector(LUA_TNUMBER,record->record_count);
  GValue high=gvector(LUA_TNUMBER,record->record_count);
  GValue low=gvector(LUA_TNUMBER,record->record_count);
  GValue new_price=gvector(LUA_TNUMBER,record->record_count);

  GValue ask_price1=gvector(LUA_TNUMBER,record->record_count);
  GValue ask_price2=gvector(LUA_TNUMBER,record->record_count);
  GValue ask_price3=gvector(LUA_TNUMBER,record->record_count);
  GValue ask_price4=gvector(LUA_TNUMBER,record->record_count);
  GValue ask_price5=gvector(LUA_TNUMBER,record->record_count);

  GValue ask_size1=gvector(LUA_TINT,record->record_count);
  GValue ask_size2=gvector(LUA_TINT,record->record_count);
  GValue ask_size3=gvector(LUA_TINT,record->record_count);
  GValue ask_size4=gvector(LUA_TINT,record->record_count);
  GValue ask_size5=gvector(LUA_TINT,record->record_count);

  GValue bid_price1=gvector(LUA_TNUMBER,record->record_count);
  GValue bid_price2=gvector(LUA_TNUMBER,record->record_count);
  GValue bid_price3=gvector(LUA_TNUMBER,record->record_count);
  GValue bid_price4=gvector(LUA_TNUMBER,record->record_count);
  GValue bid_price5=gvector(LUA_TNUMBER,record->record_count);

  GValue bid_size1=gvector(LUA_TINT,record->record_count);
  GValue bid_size2=gvector(LUA_TINT,record->record_count);
  GValue bid_size3=gvector(LUA_TINT,record->record_count);
  GValue bid_size4=gvector(LUA_TINT,record->record_count);
  GValue bid_size5=gvector(LUA_TINT,record->record_count);

  GValue volume=gvector(LUA_TLONG,record->record_count);
  GValue amount=gvector(LUA_TNUMBER,record->record_count);
  GValue settle=gvector(LUA_TNUMBER,record->record_count);
  GValue pre_settle=gvector(LUA_TNUMBER,record->record_count);
  GValue position=gvector(LUA_TLONG,record->record_count);
  GValue pre_position=gvector(LUA_TLONG,record->record_count);

  GValue high_limit=gvector(LUA_TNUMBER,record->record_count);
  GValue low_limit=gvector(LUA_TNUMBER,record->record_count);


  CFEL2MarketSnapshot* marketDatas = (CFEL2MarketSnapshot*)record->body;

  for(int i=0;i<record->record_count;i++)
  {
    gI(time)[i]=marketDatas[i].data_time_stamp;
    gI(local_time)[i]=record->local_time;
    gS(sym)[i]=ss(marketDatas[i].wind_code);

    gE(pre_close)[i]=marketDatas[i].pre_close;
    gE(open)[i]=marketDatas[i].open_price;
    gE(high)[i]=marketDatas[i].high_price;
    gE(low)[i]=marketDatas[i].low_price;
    gE(new_price)[i]=marketDatas[i].new_price;

    gE(bid_price1)[i]=marketDatas[i].bid_price1;
    gE(bid_price2)[i]=marketDatas[i].bid_price2;
    gE(bid_price3)[i]=marketDatas[i].bid_price3;
    gE(bid_price4)[i]=marketDatas[i].bid_price4;
    gE(bid_price5)[i]=marketDatas[i].bid_price5;

    gI(bid_size1)[i]=marketDatas[i].bid_size1;
    gI(bid_size2)[i]=marketDatas[i].bid_size2;
    gI(bid_size3)[i]=marketDatas[i].bid_size3;
    gI(bid_size4)[i]=marketDatas[i].bid_size4;
    gI(bid_size5)[i]=marketDatas[i].bid_size5;

    gE(ask_price1)[i]=marketDatas[i].ask_price1;
    gE(ask_price2)[i]=marketDatas[i].ask_price2;
    gE(ask_price3)[i]=marketDatas[i].ask_price3;
    gE(ask_price4)[i]=marketDatas[i].ask_price4;
    gE(ask_price5)[i]=marketDatas[i].ask_price5;

    gI(ask_size1)[i]=marketDatas[i].ask_size1;
    gI(ask_size2)[i]=marketDatas[i].ask_size2;
    gI(ask_size3)[i]=marketDatas[i].ask_size3;
    gI(ask_size4)[i]=marketDatas[i].ask_size4;
    gI(ask_size5)[i]=marketDatas[i].ask_size5;

    gJ(volume)[i]=marketDatas[i].total_volume;
    gE(amount)[i]=marketDatas[i].total_amount;

    gE(settle)[i]=marketDatas[i].settle;
    gE(pre_settle)[i]=marketDatas[i].pre_settle;
    gJ(position)[i]=marketDatas[i].position;
    gJ(pre_position)[i]=marketDatas[i].pre_position;

    gE(high_limit)[i]=marketDatas[i].high_limit;
    gE(low_limit)[i]=marketDatas[i].low_limit;

  }
  GValue data =glist(36,time,local_time,sym,pre_close,open,high,low,new_price,
    ask_price1,ask_price2,ask_price3,ask_price4,ask_price5,ask_size1,ask_size2,ask_size3,ask_size4,ask_size5,
    bid_price1,bid_price2,bid_price3,bid_price4,bid_price5,bid_size1,bid_size2,bid_size3,bid_size4,bid_size5,
    settle,pre_settle,pre_position,position,volume,amount,high_limit,low_limit);
  ExecGavaScript(record->product_id,"upd(x,y)",gstring("cffel2_mkt"),data);
}

void wmdf::GavaFeed::ProcessDCEMarketData(MdfRecord* record)
{
  GValue time=gvector(LUA_TTIME,record->record_count);
  GValue local_time=gvector(LUA_TTIME,record->record_count);
  GValue sym=gvector(LUA_TSTRING,record->record_count);

  GValue pre_close=gvector(LUA_TNUMBER,record->record_count);
  GValue open=gvector(LUA_TNUMBER,record->record_count);
  GValue high=gvector(LUA_TNUMBER,record->record_count);
  GValue low=gvector(LUA_TNUMBER,record->record_count);
  GValue new_price=gvector(LUA_TNUMBER,record->record_count);

  GValue ask_price1=gvector(LUA_TNUMBER,record->record_count);
  GValue ask_size1=gvector(LUA_TINT,record->record_count);
  GValue bid_price1=gvector(LUA_TNUMBER,record->record_count);
  GValue bid_size1=gvector(LUA_TINT,record->record_count);

  GValue settle = gvector(LUA_TNUMBER,record->record_count);
  GValue presettle=gvector(LUA_TNUMBER,record->record_count);
  GValue preposition=gvector(LUA_TLONG,record->record_count);
  GValue position=gvector(LUA_TLONG,record->record_count);

  GValue volume=gvector(LUA_TLONG,record->record_count);
  GValue amount=gvector(LUA_TNUMBER,record->record_count);

  GValue high_limit=gvector(LUA_TNUMBER,record->record_count);
  GValue low_limit=gvector(LUA_TNUMBER,record->record_count);


  FuturesMarketSnapshot* marketDatas = (FuturesMarketSnapshot*)record->body;


  for(int i=0;i<record->record_count;i++)
  {
    gI(time)[i]=marketDatas[i].data_time_stamp;
    gI(local_time)[i]=record->local_time;
    gS(sym)[i]=ss(marketDatas[i].wind_code);

    gE(pre_close)[i]=marketDatas[i].pre_close;
    gE(open)[i]=marketDatas[i].open_price;
    gE(high)[i]=marketDatas[i].high_price;
    gE(low)[i]=marketDatas[i].low_price;
    gE(new_price)[i]=marketDatas[i].new_price;

    gE(bid_price1)[i]=marketDatas[i].bid_price1;
    gI(bid_size1)[i]=marketDatas[i].bid_size1;
    gE(ask_price1)[i]=marketDatas[i].ask_price1;
    gI(ask_size1)[i]=marketDatas[i].ask_size1;

    gE(settle)[i] = marketDatas[i].settle;
    gE(presettle)[i] = marketDatas[i].pre_settle;
    gJ(preposition)[i] = marketDatas[i].pre_position;
    gJ(position)[i] = marketDatas[i].position;

    gJ(volume)[i]=marketDatas[i].total_volume;
    gE(amount)[i]=marketDatas[i].total_amount;

    gE(high_limit)[i]=marketDatas[i].high_limit;
    gE(low_limit)[i]=marketDatas[i].low_limit;

  }
  GValue data =glist(20,time,local_time,sym,pre_close,open,high,low,new_price,
    ask_price1,ask_size1,bid_price1,bid_size1,settle,presettle,preposition,position,
    volume,amount, high_limit,low_limit);
  ExecGavaScript(record->product_id,"upd(x,y)",gstring("dce_mkt"),data);
}

void wmdf::GavaFeed::ProcessSHFEMarketData(MdfRecord* record)
{
  GValue time=gvector(LUA_TTIME,record->record_count);
  GValue local_time=gvector(LUA_TTIME,record->record_count);
  GValue sym=gvector(LUA_TSTRING,record->record_count);

  GValue pre_close=gvector(LUA_TNUMBER,record->record_count);
  GValue open=gvector(LUA_TNUMBER,record->record_count);
  GValue high=gvector(LUA_TNUMBER,record->record_count);
  GValue low=gvector(LUA_TNUMBER,record->record_count);
  GValue new_price=gvector(LUA_TNUMBER,record->record_count);

  GValue ask_price1=gvector(LUA_TNUMBER,record->record_count);
  GValue ask_size1=gvector(LUA_TINT,record->record_count);
  GValue bid_price1=gvector(LUA_TNUMBER,record->record_count);
  GValue bid_size1=gvector(LUA_TINT,record->record_count);

  GValue settle = gvector(LUA_TNUMBER,record->record_count);
  GValue presettle=gvector(LUA_TNUMBER,record->record_count);
  GValue preposition=gvector(LUA_TLONG,record->record_count);
  GValue position=gvector(LUA_TLONG,record->record_count);

  GValue volume=gvector(LUA_TLONG,record->record_count);
  GValue amount=gvector(LUA_TNUMBER,record->record_count);

  GValue high_limit=gvector(LUA_TNUMBER,record->record_count);
  GValue low_limit=gvector(LUA_TNUMBER,record->record_count);


  FuturesMarketSnapshot* marketDatas = (FuturesMarketSnapshot*)record->body;

  for(int i=0;i<record->record_count;i++)
  {
    gI(time)[i]=marketDatas[i].data_time_stamp;
    gI(local_time)[i]=record->local_time;
    gS(sym)[i]=ss(marketDatas[i].wind_code);

    gE(pre_close)[i]=marketDatas[i].pre_close;
    gE(open)[i]=marketDatas[i].open_price;
    gE(high)[i]=marketDatas[i].high_price;
    gE(low)[i]=marketDatas[i].low_price;
    gE(new_price)[i]=marketDatas[i].new_price;

    gE(bid_price1)[i]=marketDatas[i].bid_price1;
    gI(bid_size1)[i]=marketDatas[i].bid_size1;
    gE(ask_price1)[i]=marketDatas[i].ask_price1;
    gI(ask_size1)[i]=marketDatas[i].ask_size1;

    gE(settle)[i] = marketDatas[i].settle;
    gE(presettle)[i] = marketDatas[i].pre_settle;
    gJ(preposition)[i] = marketDatas[i].pre_position;
    gJ(position)[i] = marketDatas[i].position;


    gJ(volume)[i]=marketDatas[i].total_volume;
    gE(amount)[i]=marketDatas[i].total_amount;

    gE(high_limit)[i]=marketDatas[i].high_limit;
    gE(low_limit)[i]=marketDatas[i].low_limit;

  }
  GValue data =glist(20,time,local_time,sym,pre_close,open,high,low,new_price,
    ask_price1,ask_size1,bid_price1,bid_size1,settle,presettle,preposition,position,
    volume,amount,high_limit,low_limit);
  ExecGavaScript(record->product_id,"upd(x,y)",gstring("shfe_mkt"),data);
}

void wmdf::GavaFeed::ProcessCZCEMarketData(MdfRecord* record)
{
  GValue time=gvector(LUA_TTIME,record->record_count);
  GValue local_time=gvector(LUA_TTIME,record->record_count);
  GValue sym=gvector(LUA_TSTRING,record->record_count);

  GValue pre_close=gvector(LUA_TNUMBER,record->record_count);
  GValue open=gvector(LUA_TNUMBER,record->record_count);
  GValue high=gvector(LUA_TNUMBER,record->record_count);
  GValue low=gvector(LUA_TNUMBER,record->record_count);
  GValue new_price=gvector(LUA_TNUMBER,record->record_count);

  GValue ask_price1=gvector(LUA_TNUMBER,record->record_count);
  GValue ask_size1=gvector(LUA_TINT,record->record_count);
  GValue bid_price1=gvector(LUA_TNUMBER,record->record_count);
  GValue bid_size1=gvector(LUA_TINT,record->record_count);

  GValue average_price=gvector(LUA_TNUMBER,record->record_count);
  GValue settle=gvector(LUA_TNUMBER,record->record_count);
  GValue pre_settle=gvector(LUA_TNUMBER,record->record_count);
  GValue volume=gvector(LUA_TLONG,record->record_count);
  GValue position=gvector(LUA_TLONG,record->record_count);
  GValue high_limit=gvector(LUA_TNUMBER,record->record_count);
  GValue low_limit=gvector(LUA_TNUMBER,record->record_count);


  FuturesMarketSnapshot* marketDatas = (FuturesMarketSnapshot*)record->body;
  for(int i=0;i<record->record_count;i++)
  {
    gI(time)[i]=marketDatas[i].data_time_stamp;
    gI(local_time)[i]=record->local_time;
    gS(sym)[i]=ss(marketDatas[i].wind_code);
    gE(pre_close)[i]=marketDatas[i].pre_close;
    gE(open)[i]=marketDatas[i].open_price;
    gE(high)[i]=marketDatas[i].high_price;
    gE(low)[i]=marketDatas[i].low_price;
    gE(new_price)[i]=marketDatas[i].new_price;
    gE(bid_price1)[i]=marketDatas[i].bid_price1;
    gI(bid_size1)[i]=marketDatas[i].bid_size1;
    gE(ask_price1)[i]=marketDatas[i].ask_price1;
    gI(ask_size1)[i]=marketDatas[i].ask_size1;
    gE(average_price)[i]=marketDatas[i].average_price;
    gE(settle)[i]=marketDatas[i].settle;
    gE(pre_settle)[i]=marketDatas[i].pre_settle;
    gJ(volume)[i]=marketDatas[i].total_volume;
    gJ(position)[i]=marketDatas[i].position;
    gE(high_limit)[i]=marketDatas[i].high_limit;
    gE(low_limit)[i]=marketDatas[i].low_limit;
  }
  GValue data =glist(19,time,local_time,sym,pre_close,open,high,low,new_price,
    ask_price1,ask_size1,bid_price1,bid_size1,
    average_price,settle,pre_settle,volume,position,high_limit,low_limit);
  ExecGavaScript(record->product_id,"upd(x,y)",gstring("czce_mkt"),data);
}

void wmdf::GavaFeed::ProcessMarketDateSignal(MdfRecord* record )
{
  MarketDateSignal* date_signal = (MarketDateSignal*)record->body;
  for(int i = 0;i<record->record_count;i++)
  {
    if(date_signal[i].market_operation==MO_CLOSE && date_signal[i].product_id!=PRODEUCT_SYSTEM_ID)
    {
      int32_t day = DateTime::date2ymd(date_signal[i].market_date);
      Logger::instance()->WriteFormatLog(LL_INFO,"End Of Day(Date:%d\t Operation:%d ProductId:%d) ",day,date_signal[i].market_operation,date_signal[i].product_id);
      ExecGavaScript(date_signal[i].product_id,"endofday(x)",gtdate(date_signal[i].market_date),NULL);
    }
  }
  packet_id_map_[record->product_id] = 0;
  ClearCache();
}

bool wmdf::GavaFeed::IsProductExistedInIdMap( int16_t product_id )
{
  std::map<int16_t, uint32_t>::iterator iter = packet_id_map_.find(product_id);
  if (iter != packet_id_map_.end())
  {
    return true;
  }
  return false;
}

void GavaFeed::ProcessL2OrderQueues(MdfRecord* record)
{
  L2OrderQueue* order_queues = (L2OrderQueue*)record->body;
  int length = record->record_count;
  int lines = 0;
  for(int i=0;i<length;i++)
  {
    for(int j=0;j<order_queues[i].price_level_count;j++)
    {
      lines+=order_queues[i].price_levels[j].orders_number;
    }
  }

  GValue time=gvector(LUA_TTIME,lines);
  GValue local_time=gvector(LUA_TTIME,lines);
  GValue windcode=gvector(LUA_TSTRING,lines);
  GValue orderQueueIndex=gvector(LUA_TINT,lines);
  GValue side=gvector(LUA_TINT,lines);
  GValue status=gvector(LUA_TINT,lines);
  GValue priceNum=gvector(LUA_TINT,lines);

  GValue priceIndex=gvector(LUA_TINT,lines);
  GValue price_op=gvector(LUA_TINT,lines);
  GValue price=gvector(LUA_TNUMBER,lines);
  GValue totalTradeNum=gvector(LUA_TINT,lines);
  GValue deltaTradeNum=gvector(LUA_TINT,lines);

  GValue orderIndex=gvector(LUA_TINT,lines);
  GValue orderOp=gvector(LUA_TINT,lines);
  GValue entry_id=gvector(LUA_TINT,lines);
  GValue orderQty=gvector(LUA_TINT,lines);


  for(int i=0,index=0;i<length;i++)
  {
    for(int j=0;j<order_queues[i].price_level_count;j++)
    {
      for(int k=0;k<order_queues[i].price_levels[j].orders_number;k++,index++)
      {
        gI(time)[index]=order_queues[i].data_time_stamp;
        gI(local_time)[i]=record->local_time;
        gS(windcode)[index]=ss(order_queues[i].wind_code);
        gI(orderQueueIndex)[index] = 1;
        gI(side)[index] = order_queues[i].order_side;
        gI(status)[index] = order_queues[i].image_status;
        gI(priceNum)[index] = order_queues[i].price_level_count;

        gI(priceIndex)[index] = j;
        gI(price_op)[index]=order_queues[i].price_levels[j].operator_type;
        gE(price)[index]=order_queues[i].price_levels[j].order_price;
        gI(totalTradeNum)[index]=order_queues[i].price_levels[j].total_number;
        gI(deltaTradeNum)[index]=order_queues[i].price_levels[j].orders_number;

        gI(orderIndex)[index] = k;
        gI(orderOp)[index]=order_queues[i].price_levels[j].order_items[k].operator_type;
        gI(entry_id)[index]=order_queues[i].price_levels[j].order_items[k].entry_id;
        gI(orderQty)[index]=order_queues[i].price_levels[j].order_items[k].size;
      }
    }
  }

  GValue tab=glist(15,time,local_time,windcode,orderQueueIndex,side,status,priceNum,priceIndex,price_op,price,
    totalTradeNum,deltaTradeNum,orderIndex,orderOp,entry_id,orderQty);
  ExecGavaScript(record->product_id,"upd(x,y)",gstring("orders"),tab);
}




void wmdf::GavaFeed::Start()
{
  for(int32_t index=0;index<feed_info_->target_info_count();index++)
  {
    int32_t gava_handle = hopen(feed_info_->targets(index)->dest_ip().c_str(),
      feed_info_->targets(index)->dest_port());
    if (gava_handle> -1)
    {
      Logger::instance()->WriteFormatLog(LL_INFO,"succeed to login gava server.(%s: %d) ",
        feed_info_->targets(index)->dest_ip().c_str(),
        feed_info_->targets(index)->dest_port());
    }
    else
    {
      Logger::instance()->WriteFormatLog(LL_INFO,"failed to login gava server.(%s: %d) ",
        feed_info_->targets(index)->dest_ip().c_str(),
        feed_info_->targets(index)->dest_port());
    }
    for(int32_t subindex=0;subindex<feed_info_->targets(index)->products_count();subindex++)
    {
      gava_handles_.insert(make_pair(feed_info_->targets(index)->products(subindex)->product_id(),gava_handle));
    }
  }

  CallbackArgs callback_args ={0};
  callback_args.connection_id=feed_info_->feed_id();
  callback_args.record_callback =CallBackFunc;
  callback_args.state=this;
  wmdf_set_callback(&callback_args);


    /*FilterArgs filter_args ;
    string filter_masks = "300078.SZ,300077.SZ";
    filter_args.connection_id = i;
    filter_args.filter_masks = (uint8_t*)filter_masks.c_str();
    wmdf_set_filter_mask(&filter_args);*/
  OpenArgs conn_args;
  conn_args.codec_type = (CodecType)feed_info_->codec_type();
  conn_args.connection_id = feed_info_->feed_id();
  strcpy((char*)conn_args.user_name,feed_info_->user_name().c_str());
  strcpy((char*)conn_args.password,feed_info_->password().c_str());
  strcpy((char*)conn_args.server_addr,feed_info_->source_ip().c_str());
  conn_args.server_port= feed_info_->source_port();
  conn_args.marktet_open_time = 00000;
  conn_args.markt_close_time = 235959;
  if (wmdf_open_connection(&conn_args)==SUCCESS)
  {
    Logger::instance()->WriteFormatLog(LL_INFO,"succeed to login feed server.(%s: %d) ",feed_info_->source_ip().c_str(),feed_info_->source_port());
  }
  else
  {
    Logger::instance()->WriteFormatLog(LL_INFO,"failed to login feed server.(%s: %d) ",feed_info_->source_ip().c_str(),feed_info_->source_port());
  }

  if (!isworking_)
  {
    isworking_ = true;
    work_thread_ = new boost::thread(boost::bind(&GavaFeed::GuardGavaConnections,this));
  }
}

void wmdf::GavaFeed::ProcessCFFEL2TransactionData( MdfRecord *record, int32_t &trade_index, std::string table )
{
	CFEL2MarketSnapshot *marketDatas = (CFEL2MarketSnapshot*)record->body;
	
	FutureTransaction *future_data = new FutureTransaction[record->record_count];

	int32_t num_of_cffel2transaction = market_data_container_->ConvertCFFEL2Transaction(record->body,record->record_count,future_data);
	
	GValue time				=	gvector(LUA_TTIME,num_of_cffel2transaction);
	GValue local_time	=	gvector(LUA_TTIME,num_of_cffel2transaction);
	GValue sym				=	gvector(LUA_TSTRING,num_of_cffel2transaction);
	GValue index      = gvector(LUA_TINT,num_of_cffel2transaction);
	GValue price			= gvector(LUA_TNUMBER,num_of_cffel2transaction);
	GValue volume     = gvector(LUA_TINT,num_of_cffel2transaction);
	GValue position		= gvector(LUA_TINT,num_of_cffel2transaction);
	GValue status			=	gvector(LUA_TINT,num_of_cffel2transaction);


	for(int32_t i=0;i!=num_of_cffel2transaction;++i)
	{
		FutureTransaction *data = &future_data[i];
		gI(time)[i] = data->time;
		gI(local_time)[i] = record->local_time;
		gS(sym)[i]		= ss(data->wind_code);
		gI(index)[i] = ++trade_index;
		gE(price)[i]		=	data->new_price;
		gI(volume)[i] = data->delt_volume;
		gI(position)[i] = data->delt_position;
		gI(status)[i] = data->futureoptionstate;
	}

	delete [] future_data;

	GValue data =glist(8,time,local_time,sym,index,price,volume,position,status);
	ExecGavaScript(record->product_id,"upd(x,y)",gstring(table.c_str()),data);
}


void wmdf::GavaFeed::ExecGavaScript(int16_t product_id,const char* func_script,GValue table_name,GValue data)
{
  int sock_id = GetGavaHandle(product_id);
  if(sock_id==WMDF_CONSTS_32BIT_NIL_VALUE || sock_id<=0)
  {
    grelease(table_name);
    grelease(data);
    return;
  }
  if(NULL == hexec(-sock_id,func_script,table_name,data,NULL))
  {
    gava_handles_[product_id] = -1;
    Logger::instance()->WriteFormatLog(LL_INFO,"The link to gava is disconnect! product id:%d .",product_id);
  }
}

void wmdf::GavaFeed::SleepTime( int millsecond )
{
#ifdef WIN32
  Sleep(millsecond);
#else
  usleep(1000*millsecond);
#endif

}

void wmdf::GavaFeed::GuardGavaConnections()
{
  while(isworking_)
  {
    //date_t today = DateTime::today();
    //int32_t cur_time = DateTime::time2hms(DateTime::get_current_time());
    //if(today==next_date_)
    //{
    //  if(cur_time>=clear_cache_time_)
    //  {
    //    ClearCache();
    //    next_date_ += 1;
    //  }
    //}

    for(int32_t j=0;j<feed_info_->target_info_count();j++)
    {
      for(int k=0;k<feed_info_->targets(j)->products_count();k++)
      {
        int16_t product_id = feed_info_->targets(j)->products(k)->product_id();
        int gava_handle = GetGavaHandle(product_id);

        if(gava_handle<0||gava_handle==WMDF_CONSTS_32BIT_NIL_VALUE)
        {
          gava_handle = hopen(feed_info_->targets(j)->dest_ip().c_str(),feed_info_->targets(j)->dest_port());
          if (gava_handle> -1)
          {
            Logger::instance()->WriteFormatLog(LL_INFO,"succeed to login gava server.(%s: %d) ",
              feed_info_->targets(j)->dest_ip().c_str(),
              feed_info_->targets(j)->dest_port());
          }
          else
          {
            Logger::instance()->WriteFormatLog(LL_INFO,"failed to login gava server.(%s: %d) ",
              feed_info_->targets(j)->dest_ip().c_str(),
              feed_info_->targets(j)->dest_port());
          }
          for(int32_t subindex=0;subindex<feed_info_->targets(j)->products_count();subindex++)
          {
            product_id = feed_info_->targets(j)->products(subindex)->product_id();
            gava_handles_[product_id] = gava_handle;
          }
          break;
        }
      }
    }
    SleepTime(10000);
  }
}

void wmdf::GavaFeed::ClearCache()
{
  market_data_container_->Clear();
  Logger::instance()->WriteLog(LL_DEBUG,"Clear data cached in market data container.");
  packet_id_map_.clear();
}

