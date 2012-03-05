#include "market_snapshot_container.h"
#include "../../include/date_time.h"
#include <cmath>

using namespace std;


wmdf::MarketSnapshotContainer::MarketSnapshotContainer()
{
}

wmdf::MarketSnapshotContainer::~MarketSnapshotContainer()
{
	Clear();
}

void wmdf::MarketSnapshotContainer::MergeL2MarketSnapshot( L2MarketSnapshot *marketsnapshot )
{
	string windcode = marketsnapshot->wind_code;
	map<string,L2MarketSnapshot* >::iterator iter = l2market_snapshots_.find(windcode);
	if(iter == l2market_snapshots_.end())
	{
		L2MarketSnapshot *ms = new L2MarketSnapshot();
		memcpy(ms,marketsnapshot,sizeof(L2MarketSnapshot));
		l2market_snapshots_.insert(pair<string,L2MarketSnapshot*>(windcode,ms));
	}
	else
	{
		L2MarketSnapshot *ms = iter->second;
		UpdateL2MarketSnapshot(ms,marketsnapshot);
	}
}

void wmdf::MarketSnapshotContainer::MergeL1MarketSnapshot( L1MarketSnapshot *marketsnapshot )
{
	string windcode = marketsnapshot->wind_code;
	map<string,L1MarketSnapshot*>::iterator iter = l1_market_snapshots_.find(windcode);
	if(iter == l1_market_snapshots_.end())
	{
		L1MarketSnapshot *ms = new L1MarketSnapshot;
		memcpy(ms,marketsnapshot,sizeof(L1MarketSnapshot));
		l1_market_snapshots_.insert(pair<string,L1MarketSnapshot*>(windcode,ms));
		UpdateL1MarketSnapshot(marketsnapshot,marketsnapshot);
	}
	else
	{
		L1MarketSnapshot *ms  = iter->second;
		UpdateL1MarketSnapshot(ms,marketsnapshot);
	}
}

void wmdf::MarketSnapshotContainer::UpdateL2MarketSnapshot( L2MarketSnapshot *des, L2MarketSnapshot *src )
{
	if(src->data_time_stamp != WMDF_CONSTS_32BIT_NIL_VALUE)
		des->data_time_stamp = src->data_time_stamp;
  else
    src->data_time_stamp = des->data_time_stamp;

	if(src->new_price == src->new_price)
		des->new_price = src->new_price;
  else
    src->new_price = des->new_price;

	if(src->pre_close == src->pre_close)
		des->pre_close = src->pre_close;
  else
    src->pre_close = des->pre_close;

	if(src->open_price == src->open_price)
		des->open_price = src->open_price;
  else
    src->open_price = des->open_price;

	if(src->high_price == src->high_price)
		des->high_price = src->high_price;
  else
    src->high_price = des->high_price;

	if(src->low_price == src->low_price)
		des->low_price = src->low_price;
  else
    src->low_price = des->low_price;

	if(src->total_volume != WMDF_CONSTS_64BIT_NIL_VALUE)
		des->total_volume = src->total_volume;
  else
    src->total_volume = des->total_volume;

	if(src->total_amount == src->total_amount)
		des->total_amount = src->total_amount;
  else
    src->total_amount = des->total_amount;

	if(src->bid_price1 == src->bid_price1)
		des->bid_price1 = src->bid_price1;
  else
    src->bid_price1 = des->bid_price1;

	if(src->bid_price2 == src->bid_price2)
		des->bid_price2 = src->bid_price2;
  else
    src->bid_price2 = des->bid_price2;

	if(src->bid_price3 == src->bid_price3)
		des->bid_price3 = src->bid_price3;
  else
    src->bid_price3 = des->bid_price3;

	if(src->bid_price4 == src->bid_price4)
		des->bid_price4 = src->bid_price4;
  else
    src->bid_price4 = des->bid_price4;

	if(src->bid_price5 == src->bid_price5)
		des->bid_price5 = src->bid_price5;
  else
    src->bid_price5 = des->bid_price5;

	if(src->bid_price6 == src->bid_price6)
		des->bid_price6 = src->bid_price6;
  else
    src->bid_price6 = des->bid_price6;

	if(src->bid_price7 == src->bid_price7)
		des->bid_price7 = src->bid_price7;
  else
    src->bid_price7 = des->bid_price7;

	if(src->bid_price8 == src->bid_price8)
		des->bid_price8 = src->bid_price8;
  else
    src->bid_price8 = des->bid_price8;

	if(src->bid_price9 == src->bid_price9)
		des->bid_price9 = src->bid_price9;
  else
    src->bid_price9 = des->bid_price9;

	if(src->bid_price10 == src->bid_price10)
		des->bid_price10 = src->bid_price10;
  else
    src->bid_price10 = des->bid_price10;

	if(src->ask_price1 == src->ask_price1)
		des->ask_price1 = src->ask_price1;
  else
    src->ask_price1 = des->ask_price1;

	if(src->ask_price2 == src->ask_price2)
		des->ask_price2 = src->ask_price2;
  else
    src->ask_price2 = des->ask_price2;

	if(src->ask_price3 == src->ask_price3)
		des->ask_price3 = src->ask_price3;
  else
    src->ask_price3 = des->ask_price3;

	if(src->ask_price4 == src->ask_price4)
		des->ask_price4 = src->ask_price4;
  else
    src->ask_price4 = des->ask_price4;

	if(src->ask_price5 == src->ask_price5)
		des->ask_price5 = src->ask_price5;
  else
    src->ask_price5 = des->ask_price5;

	if(src->ask_price6 == src->ask_price6)
		des->ask_price6 = src->ask_price6;
  else
    src->ask_price6 = des->ask_price6;

	if(src->ask_price7 == src->ask_price7)
		des->ask_price7 = src->ask_price7;
  else
    src->ask_price7 = des->ask_price7;

	if(src->ask_price8 == src->ask_price8)
		des->ask_price8 = src->ask_price8;
  else
    src->ask_price8 = des->ask_price8;

	if(src->ask_price9 == src->ask_price9 )
		des->ask_price9 = src->ask_price9;
  else
    src->ask_price9 = des->ask_price9;

	if(src->ask_price10 == src->ask_price10)
		des->ask_price10 = src->ask_price10;
  else
    src->ask_price10 = des->ask_price10;


	if(src->bid_size1 != WMDF_CONSTS_32BIT_NIL_VALUE)
		des->bid_size1 = src->bid_size1;
  else
    src->bid_size1 = des->bid_size1;

	if(src->bid_size2 != WMDF_CONSTS_32BIT_NIL_VALUE)
		des->bid_size2 = src->bid_size2;
  else
    src->bid_size2 = des->bid_size2;

	if(src->bid_size3 != WMDF_CONSTS_32BIT_NIL_VALUE)
		des->bid_size3 = src->bid_size3;
  else
    src->bid_size3 = des->bid_size3;

	if(src->bid_size4 != WMDF_CONSTS_32BIT_NIL_VALUE)
		des->bid_size4 = src->bid_size4;
  else
    src->bid_size4 = des->bid_size4;

	if(src->bid_size5 != WMDF_CONSTS_32BIT_NIL_VALUE)
		des->bid_size5 = src->bid_size5;
  else
    src->bid_size5 = des->bid_size5;

	if(src->bid_size6 != WMDF_CONSTS_32BIT_NIL_VALUE)
		des->bid_size6 = src->bid_size6;
  else
    src->bid_size6 = des->bid_size6;

	if(src->bid_size7 != WMDF_CONSTS_32BIT_NIL_VALUE)
		des->bid_size7 = src->bid_size7;
  else
    src->bid_size7 = des->bid_size7;

	if(src->bid_size8 != WMDF_CONSTS_32BIT_NIL_VALUE)
		des->bid_size8 = src->bid_size8;
  else
    src->bid_size8 = des->bid_size8;

	if(src->bid_size9 != WMDF_CONSTS_32BIT_NIL_VALUE)
		des->bid_size9 = src->bid_size9;
  else
    src->bid_size9 = des->bid_size9;

	if(src->bid_size10 != WMDF_CONSTS_32BIT_NIL_VALUE)
		des->bid_size10 = src->bid_size10;
  else
    src->bid_size10 = des->bid_size10;

	if(src->ask_size1 != WMDF_CONSTS_32BIT_NIL_VALUE)
		des->ask_size1 = src->ask_size1;
  else
    src->ask_size1 = des->ask_size1;

	if(src->ask_size2 != WMDF_CONSTS_32BIT_NIL_VALUE)
		des->ask_size2 = src->ask_size2;
  else
    src->ask_size2 = des->ask_size2;

	if(src->ask_size3 != WMDF_CONSTS_32BIT_NIL_VALUE)
		des->ask_size3 = src->ask_size3;
  else
    src->ask_size3 = des->ask_size3;

	if(src->ask_size4 != WMDF_CONSTS_32BIT_NIL_VALUE)
		des->ask_size4 = src->ask_size4;
  else
    src->ask_size4 = des->ask_size4;

	if(src->ask_size5 != WMDF_CONSTS_32BIT_NIL_VALUE)
		des->ask_size5 = src->ask_size5;
  else
    src->ask_size5 = des->ask_size5;

	if(src->ask_size6 != WMDF_CONSTS_32BIT_NIL_VALUE)
		des->ask_size6 = src->ask_size6;
  else
    src->ask_size6 = des->ask_size6;

	if(src->ask_size7 != WMDF_CONSTS_32BIT_NIL_VALUE)
		des->ask_size7 = src->ask_size7;
  else
    src->ask_size7 = des->ask_size7;

	if(src->ask_size8 != WMDF_CONSTS_32BIT_NIL_VALUE)
		des->ask_size8 = src->ask_size8;
  else
    src->ask_size8 = des->ask_size8;

	if(src->ask_size9 != WMDF_CONSTS_32BIT_NIL_VALUE)
		des->ask_size9 = src->ask_size9;
  else
    src->ask_size9 = des->ask_size9;

	if(src->ask_size10 != WMDF_CONSTS_32BIT_NIL_VALUE)
		des->ask_size10 = src->ask_size10;
  else
    src->ask_size10 = des->ask_size10;

	if(src->transactions_count != WMDF_CONSTS_64BIT_NIL_VALUE)
		des->transactions_count = src->transactions_count;
  else
    src->transactions_count = des->transactions_count;

	if(src->weighted_average_bid_price == src->weighted_average_bid_price)
		des->weighted_average_bid_price = src->weighted_average_bid_price;
  else
    src->weighted_average_bid_price = des->weighted_average_bid_price;

	if(src->weighted_average_ask_price == src->weighted_average_ask_price)
		des->weighted_average_ask_price = src->weighted_average_ask_price;
  else
    src->weighted_average_ask_price = des->weighted_average_ask_price;


	if(src->total_bid_size != WMDF_CONSTS_64BIT_NIL_VALUE)
		des->total_bid_size = src->total_bid_size;
  else
    src->total_bid_size = des->total_bid_size;

	if(src->total_ask_size != WMDF_CONSTS_64BIT_NIL_VALUE)
		des->total_ask_size = src->total_ask_size;
  else
    src->total_ask_size = des->total_ask_size;

	if(src->peratio1 == src->peratio1)
		des->peratio1 = src->peratio1;
	else
		src->peratio1 = des->peratio1;

	if(src->peratio2 == src->peratio2)
		des->peratio2 = src->peratio2;
	else
		src->peratio2 = des->peratio2;

	if(src->trade_status != WMDF_CONSTS_8BIT_NIL_VALUE )
		des->trade_status = src->trade_status;
	else
		src->trade_status = des->trade_status;
}

void wmdf::MarketSnapshotContainer::UpdateL1MarketSnapshot( L1MarketSnapshot *des, L1MarketSnapshot *src )
{
	UpdateCurrentL1Marketdata(des,src);
	InsertL1Transaction(des,src);
	UpdateLastL1Marketdata(des,src);
}

wmdf::L1Transaction* wmdf::MarketSnapshotContainer::GetL1Transaction( int32_t index )
{
	return l1_transactions_[index];
}



void wmdf::MarketSnapshotContainer::UpdateCurrentL1Marketdata( L1MarketSnapshot *des, L1MarketSnapshot *src )
{
	if(des != src)
	{
		if(src->data_time_stamp == WMDF_CONSTS_32BIT_NIL_VALUE)
			src->data_time_stamp = des->data_time_stamp;

		if(src->pre_close != src->pre_close)
			src->pre_close = des->pre_close;

		if(src->open_price != src->open_price)
			src->open_price = des->open_price;

		if(src->high_price != src->high_price)
			src->high_price = des->high_price;

		if(src->low_price != src->low_price)
			src->low_price = des->low_price;

		if(src->new_price != src->new_price)
			src->new_price = des->new_price;

		if(src->total_volume == WMDF_CONSTS_64BIT_NIL_VALUE)
			src->total_volume = des->total_volume;

		if(src->total_amount != src->total_amount)
			src->total_amount = des->total_amount;

		if(src->bid_price1 != src->bid_price1)
			src->bid_price1 = des->bid_price1;

		if(src->bid_price2 != src->bid_price2)
			src->bid_price2 = des->bid_price2;

		if(src->bid_price3 != src->bid_price3)
			src->bid_price3 = des->bid_price3;

		if(src->bid_price4 != src->bid_price4)
			src->bid_price4 = des->bid_price4;

		if(src->bid_price5 != src->bid_price5)
			src->bid_price5 = des->bid_price5;

		if(src->ask_price1 != src->ask_price1)
			src->ask_price1 = des->ask_price1;

		if(src->ask_price2 != src->ask_price2)
			src->ask_price2 = des->ask_price2;

		if(src->ask_price3 != src->ask_price3)
			src->ask_price3 = des->ask_price3;

		if(src->ask_price4 != src->ask_price4)
			src->ask_price4 = des->ask_price4;

		if(src->ask_price5 != src->ask_price5)
			src->ask_price5 = des->ask_price5;

		if(src->ask_size1 == WMDF_CONSTS_32BIT_NIL_VALUE)
			src->ask_size1 = des->ask_size1;

		if(src->ask_size2 == WMDF_CONSTS_32BIT_NIL_VALUE)
			src->ask_size2 = des->ask_size2;

		if(src->ask_size3 == WMDF_CONSTS_32BIT_NIL_VALUE)
			src->ask_size3 = des->ask_size3;

		if(src->ask_size4 == WMDF_CONSTS_32BIT_NIL_VALUE)
			src->ask_size4 = des->ask_size4;

		if(src->ask_size5 == WMDF_CONSTS_32BIT_NIL_VALUE)
			src->ask_size5 = des->ask_size5;

		if(src->bid_size1 == WMDF_CONSTS_32BIT_NIL_VALUE)
			src->bid_size1 = des->bid_size1;

		if(src->bid_size2 == WMDF_CONSTS_32BIT_NIL_VALUE)
			src->bid_size2 = des->bid_size2;

		if(src->bid_size3 == WMDF_CONSTS_32BIT_NIL_VALUE)
			src->bid_size3 = des->bid_size3;

		if(src->bid_size4 == WMDF_CONSTS_32BIT_NIL_VALUE)
			src->bid_size4 = des->bid_size4;

		if(src->bid_size5 == WMDF_CONSTS_32BIT_NIL_VALUE)
			src->bid_size5 = des->bid_size5;

		if(src->total_volume == 0 || src->total_volume == WMDF_CONSTS_64BIT_NIL_VALUE)
			src->total_amount = 0;

	}
}


void wmdf::MarketSnapshotContainer::InsertL1Transaction( L1MarketSnapshot *des, L1MarketSnapshot *src )
{
	int64_t delt_volume = 0;
	double delt_amount = 0;

	if(des == src)
	{
		delt_volume = src->total_volume;
		delt_amount = src->total_amount;
	}
	else
	{
		delt_volume = src->total_volume - des->total_volume;
		delt_amount = src->total_amount - des->total_amount;
	}


	if(delt_volume>0)
	{
		L1Transaction *curr_transaction = new L1Transaction();
		curr_transaction->time = src->data_time_stamp;
		memcpy(curr_transaction->wind_code,src->wind_code,sizeof(src->wind_code));
		curr_transaction->new_price = src->new_price;
		curr_transaction->delt_volume = delt_volume;
		curr_transaction->delt_amount = delt_amount;
		l1_transactions_.push_back(curr_transaction);
	}
}


void wmdf::MarketSnapshotContainer::UpdateLastL1Marketdata( L1MarketSnapshot *des,L1MarketSnapshot *src )
{
	if(des != src)
	{
		if(src->data_time_stamp != WMDF_CONSTS_32BIT_NIL_VALUE)
			des->data_time_stamp = src->data_time_stamp;

		if(src->pre_close = src->pre_close)
			des->pre_close = src->pre_close;

		if(src->open_price = src->open_price)
			des->open_price = src->open_price;

		if(src->high_price = src->high_price)
			des->high_price = src->high_price;

		if(src->low_price = src->low_price)
			des->low_price = src->low_price;

		if(src->new_price = src->new_price)
			des->new_price = src->new_price;

		if(src->total_volume != WMDF_CONSTS_64BIT_NIL_VALUE)
			des->total_volume = src->total_volume ;

		if(src->total_amount = src->total_amount)
			des->total_amount = src->total_amount ;

		if(src->bid_price1 = src->bid_price1)
			des->bid_price1 = src->bid_price1 ;

		if(src->bid_price2 = src->bid_price2)
			des->bid_price2 = src->bid_price2 ;

		if(src->bid_price3 = src->bid_price3)
			des->bid_price3 = src->bid_price3 ;

		if(src->bid_price4 = src->bid_price4)
			des->bid_price4 = src->bid_price4;

		if(src->bid_price5 = src->bid_price5)
			des->bid_price5 = src->bid_price5 ;

		if(src->ask_price1 = src->ask_price1)
			des->ask_price1 = src->ask_price1 ;

		if(src->ask_price2 = src->ask_price2)
			des->ask_price2 = src->ask_price2 ;

		if(src->ask_price3 = src->ask_price3)
			des->ask_price3 = src->ask_price3 ;

		if(src->ask_price4 = src->ask_price4)
			des->ask_price4 = src->ask_price4  ;

		if(src->ask_price5 = src->ask_price5)
			des->ask_price5 = src->ask_price5 ;

		if(src->ask_size1 != WMDF_CONSTS_32BIT_NIL_VALUE)
			des->ask_size1 = src->ask_size1  ;

		if(src->ask_size2 != WMDF_CONSTS_32BIT_NIL_VALUE)
			des->ask_size2 = src->ask_size2 ;

		if(src->ask_size3 != WMDF_CONSTS_32BIT_NIL_VALUE)
			des->ask_size3 = src->ask_size3;

		if(src->ask_size4 != WMDF_CONSTS_32BIT_NIL_VALUE)
			des->ask_size4 = src->ask_size4  ;

		if(src->ask_size5 != WMDF_CONSTS_32BIT_NIL_VALUE)
			des->ask_size5 = src->ask_size5 ;

		if(src->bid_size1 != WMDF_CONSTS_32BIT_NIL_VALUE)
			des->bid_size1 = src->bid_size1 ;

		if(src->bid_size2 != WMDF_CONSTS_32BIT_NIL_VALUE)
			des->bid_size2 = src->bid_size2  ;

		if(src->bid_size3 != WMDF_CONSTS_32BIT_NIL_VALUE)
			des->bid_size3 = src->bid_size3  ;

		if(src->bid_size4 != WMDF_CONSTS_32BIT_NIL_VALUE)
			des->bid_size4 = src->bid_size4  ;

		if(src->bid_size5 != WMDF_CONSTS_32BIT_NIL_VALUE)
			des->bid_size5 = src->bid_size5 ;
	}
}





void wmdf::MarketSnapshotContainer::MergeIndexSnapshot( IndexSnapshot* index_snapshot )
{
  string windcode = index_snapshot->wind_code;
  map<string,IndexSnapshot* >::iterator iter = index_snapshots_.find(windcode);
  if(iter == index_snapshots_.end())
  {
    IndexSnapshot *index_s = new IndexSnapshot();
    memcpy(index_s,index_snapshot,sizeof(IndexSnapshot));
    index_snapshots_.insert(pair<string,IndexSnapshot*>(windcode,index_s));
  }
  else
  {
    IndexSnapshot *ms = iter->second;
    UpdateIndexSnapshot(ms,index_snapshot);
  }
}

void wmdf::MarketSnapshotContainer::UpdateIndexSnapshot( IndexSnapshot* des,IndexSnapshot* src )
{
  if(src->data_time_stamp != WMDF_CONSTS_32BIT_NIL_VALUE)
    des->data_time_stamp = src->data_time_stamp;
  else
    src->data_time_stamp = des->data_time_stamp;

  if(src->pre_close == src->pre_close)
    des->pre_close = src->pre_close;
  else
    src->pre_close = des->pre_close;

  if(src->open_price == src->open_price)
    des->open_price = src->open_price;
  else
    src->open_price = des->open_price;

  if(src->high_price == src->high_price)
    des->high_price = src->high_price;
  else
    src->high_price = des->high_price;

  if(src->low_price == src->low_price)
    des->low_price = src->low_price;
  else
    src->low_price = des->low_price;

  if(src->new_price == src->new_price)
    des->new_price = src->new_price;
  else
    src->new_price = des->new_price;

  if(src->total_volume != WMDF_CONSTS_64BIT_NIL_VALUE)
    des->total_volume = src->total_volume;
  else
    src->total_volume = des->total_volume;

  if(src->total_amount == src->total_amount)
    des->total_amount = src->total_amount;
  else
    src->total_amount = des->total_amount;
}



// void wmdf::MarketSnapshotContainer::UpdateL1MarketSnapshot( L1MarketSnapshot *des, L1MarketSnapshot *src )
// {
// 	if(des != NULL)
// 	{
//     memcpy(des,src,sizeof(L1MarketSnapshot));
// 		//des->total_amount = src->total_amount;
// 		//des->total_volume = src->total_volume;
// 	}
// }

FuturesMarketSnapshot* wmdf::MarketSnapshotContainer::LastFutureL1MarketSnapshot( FuturesMarketSnapshot *futuredata )
{
	string windcode = futuredata->wind_code;
	map<string,FuturesMarketSnapshot*>::iterator iter = future_l1_snapshots_.find(windcode);
	if(iter == future_l1_snapshots_.end())
	{
		FuturesMarketSnapshot *fs = new FuturesMarketSnapshot();
		memcpy(fs,futuredata,sizeof(FuturesMarketSnapshot));
		future_l1_snapshots_.insert(pair<string,FuturesMarketSnapshot*>(windcode,fs));
		return futuredata;
	}
	else
	{
		FuturesMarketSnapshot *fs = iter->second;
		return fs;
	}
}

void wmdf::MarketSnapshotContainer::UpdateFutureL1MarketData( FuturesMarketSnapshot *des, FuturesMarketSnapshot *src )
{
	if(des != src)
	{
    memcpy(des,src,sizeof(FuturesMarketSnapshot));
    //des->total_volume = src->total_volume;
		//des->position = src->position;
	}
}

CFEL2MarketSnapshot* wmdf::MarketSnapshotContainer::LastCFFEL2MarketSnapshot( CFEL2MarketSnapshot *futuredata)
{
	string windcode = futuredata->wind_code;
	map<string,CFEL2MarketSnapshot*>::iterator iter = cffe_l2_snapshots_.find(windcode);
	if(iter == cffe_l2_snapshots_.end())
	{
		CFEL2MarketSnapshot *fs = new CFEL2MarketSnapshot();
		memcpy(fs,futuredata,sizeof(CFEL2MarketSnapshot));
		cffe_l2_snapshots_.insert(pair<string,CFEL2MarketSnapshot*>(windcode,fs));
		return futuredata;
	}
	else
	{
		CFEL2MarketSnapshot *fs = iter->second;
		return fs;
	}
}

void wmdf::MarketSnapshotContainer::UpdateCFFEL2MarketData( CFEL2MarketSnapshot *des,CFEL2MarketSnapshot *src )
{
	if(des != src)
	{
		memcpy(des,src,sizeof(CFEL2MarketSnapshot));
	}
}

int32_t wmdf::MarketSnapshotContainer::ConvertCFFEL2Transaction( void *data, int32_t count,FutureTransaction *cffel2_transactions )
{
	CFEL2MarketSnapshot *marketDatas = (CFEL2MarketSnapshot*)data;

	int32_t cffel2_transactions_count =0 ;
	for(int32_t i=0;i!=count;++i)
	{
		CFEL2MarketSnapshot *curr_data = &marketDatas[i];

		CFEL2MarketSnapshot *lastfuturedata = LastCFFEL2MarketSnapshot(curr_data);
		int32_t delt_volume=0;
		int32_t delt_position=0;

		if(lastfuturedata == curr_data)
		{
			delt_volume = (int32_t)curr_data->total_volume;
			delt_position = (int32_t)curr_data->position;
		}
		else
		{
			delt_volume = (int32_t)(curr_data->total_volume - lastfuturedata->total_volume);
			delt_position = (int32_t)(curr_data->position - lastfuturedata->position);
		}

		if(delt_volume>0)
		{
			memcpy(cffel2_transactions[cffel2_transactions_count].wind_code,curr_data->wind_code,sizeof(curr_data->wind_code));
			cffel2_transactions[cffel2_transactions_count].time = curr_data->data_time_stamp;
			cffel2_transactions[cffel2_transactions_count].new_price = curr_data->new_price;
			cffel2_transactions[cffel2_transactions_count].delt_volume = delt_volume;
			cffel2_transactions[cffel2_transactions_count].delt_position = delt_position;
			FutureDealType dealtype;
			double delt_bid = abs(curr_data->new_price-lastfuturedata->bid_price1);
			double delt_ask = abs(curr_data->new_price-lastfuturedata->ask_price1);
			if(delt_bid>delt_ask)
				dealtype = fdtBuy;
			else
				dealtype = fdtSell;
			FutureOptionState futureoptionstate = GetFutureTradeState(delt_position,delt_volume,dealtype);
			cffel2_transactions[cffel2_transactions_count].futureoptionstate = (int32_t)futureoptionstate;
			++cffel2_transactions_count;
		}
		UpdateCFFEL2MarketData(lastfuturedata,curr_data);
	}
	return cffel2_transactions_count;
}




wmdf::MarketSnapshotContainer::FutureOptionState wmdf::MarketSnapshotContainer::GetFutureTradeState( double positionChange, double deltaVolume, FutureDealType dealType)
{
	double openPosition = (deltaVolume + positionChange) / 2;
	double closePosition = (deltaVolume - positionChange) / 2;

	FutureOptionState state = fosUnknown;

	if (dealType != fdtUnknown)
	{
		if (positionChange == 0 && deltaVolume == 0)
		{
			state = fosUnknown;
		}
		else
		{
			if (((openPosition == deltaVolume) && (closePosition == 0))
				|| ((openPosition == 0) && (closePosition == deltaVolume))
				|| ((openPosition == closePosition) && (openPosition == (deltaVolume / 2))))
			{
				if (deltaVolume == abs(positionChange))
				{
					if ((openPosition == deltaVolume) && (closePosition == 0))
					{
						state = fosOpenOption;
					}
					else if (((openPosition == 0) && (closePosition == deltaVolume)))
					{
						state = fosCloseOption;
					}
				}
				else if (((openPosition == closePosition) && (openPosition == deltaVolume / 2) && (positionChange == 0)))
				{
					if (dealType == fdtBuy)
						state = fosChangeLongOption;
					else
						state = fosChangeShortOption;
				}
			}
			else if (((openPosition * closePosition) != 0) && (openPosition != (deltaVolume / 2)))
			{
				if ((deltaVolume > positionChange) && (positionChange > 0))
				{
					if (dealType == fdtBuy)
						state = fosOpenLongOption;
					else
						state = fosOpenShortOption;
				}
				else if ((positionChange < 0) && (abs(positionChange) < deltaVolume))
				{
					if (dealType == fdtBuy)
						state = fosCloseShortOption;
					else
						state = fosCloseLongOption;
				}
			}
		}
	}
	return state;
}

int32_t wmdf::MarketSnapshotContainer::ConvertFutureL1Transaction( void *data, int32_t count,FutureTransaction *future_transactions )
{
	FuturesMarketSnapshot *marketDatas = (FuturesMarketSnapshot*)data;

	int32_t future_transactions_count =0 ;
	for(int32_t i=0;i!=count;++i)
	{
		FuturesMarketSnapshot *curr_data = &marketDatas[i];

		FuturesMarketSnapshot *lastfuturedata = LastFutureL1MarketSnapshot(curr_data);
		int32_t delt_volume=0;
		int32_t delt_position=0;
		

		
		if(lastfuturedata == curr_data)
		{
			delt_volume = (int32_t)curr_data->total_volume;
			delt_position = (int32_t)(curr_data->position-curr_data->pre_position);
		}
		else
		{
			delt_volume = (int32_t)(curr_data->total_volume - lastfuturedata->total_volume);
			delt_position = (int32_t)(curr_data->position - lastfuturedata->position);
		}

		if(delt_volume>0)
		{
			memcpy(future_transactions[future_transactions_count].wind_code,curr_data->wind_code,sizeof(curr_data->wind_code));
			future_transactions[future_transactions_count].time = curr_data->data_time_stamp;
			future_transactions[future_transactions_count].new_price = curr_data->new_price;
			future_transactions[future_transactions_count].delt_volume = delt_volume;
			future_transactions[future_transactions_count].delt_position = delt_position;
			FutureDealType dealtype;
			double delt_bid = abs(curr_data->new_price-lastfuturedata->bid_price1);
			double delt_ask = abs(curr_data->new_price-lastfuturedata->ask_price1);
			if(delt_bid>delt_ask)
				dealtype = fdtBuy;
			else
				dealtype = fdtSell;
			FutureOptionState futureoptionstate = GetFutureTradeState(delt_position,delt_volume,dealtype);
			future_transactions[future_transactions_count].futureoptionstate = (int32_t)futureoptionstate;
			++future_transactions_count;
		}
		UpdateFutureL1MarketData(lastfuturedata,curr_data);
	}
	return future_transactions_count;
}
//
// int32_t wmdf::MarketSnapshotContainer::ConvertL1Transaction( void *data, int32_t count, L1Transaction *l1transactions )
// {
// 	L1MarketSnapshot *marketDatas = (L1MarketSnapshot*)data;
//
// 	int32_t l1transactions_count =0 ;
// 	for(int32_t i=0;i!=count;++i)
// 	{
// 		L1MarketSnapshot *curr_data = &marketDatas[i];
//
// 		L1MarketSnapshot *last_data = LastL1MarketSnapshot(curr_data);
// 		int32_t delt_volume = 0;
// 		double delt_amount = 0;
//
// 		if(last_data == curr_data)
// 		{
// 			delt_volume = (int32_t)curr_data->total_volume;
// 			delt_amount = curr_data->total_amount;
// 		}
// 		else
// 		{
// 			delt_volume = (int32_t)(curr_data->total_volume - last_data->total_volume);
// 			delt_amount = curr_data->total_amount = last_data->total_amount;
// 		}
//
// 		if(delt_volume>0)
// 		{
// 			memcpy(l1transactions[l1transactions_count].wind_code,curr_data->wind_code,sizeof(curr_data->wind_code));
// 			l1transactions[l1transactions_count].time = curr_data->data_time_stamp;
// 			l1transactions[l1transactions_count].new_price = curr_data->new_price;
// 			l1transactions[l1transactions_count].delt_volume = delt_volume;
// 			l1transactions[l1transactions_count].delt_amount = delt_amount;
//
// 			++l1transactions_count;
// 		}
// 		UpdateL1MarketSnapshot(last_data,curr_data);
// 	}
// 	return l1transactions_count;
// }

void wmdf::MarketSnapshotContainer::Clear()
{
  for(map<std::string,L2MarketSnapshot* >::iterator iter = l2market_snapshots_.begin();iter!=l2market_snapshots_.end();++iter)
  {
    L2MarketSnapshot *ms = iter->second;
    if(ms!=NULL)
    {
      delete ms;
      ms = NULL;
    }
  }
  l2market_snapshots_.clear();

  for(map<std::string,IndexSnapshot* >::iterator iter = index_snapshots_.begin();iter!=index_snapshots_.end();++iter)
  {
    IndexSnapshot *ms = iter->second;
    if(ms!=NULL)
    {
      delete ms;
      ms = NULL;
    }
  }
  index_snapshots_.clear();

  for(map<std::string,L1MarketSnapshot* >::iterator iter = l1_market_snapshots_.begin();iter!=l1_market_snapshots_.end();++iter)
  {
    L1MarketSnapshot *ms = iter->second;
    if(ms!=NULL)
    {
      delete ms;
      ms = NULL;
    }
  }
  l1_market_snapshots_.clear();

  for(map<std::string,FuturesMarketSnapshot* >::iterator iter = future_l1_snapshots_.begin();iter!=future_l1_snapshots_.end();++iter)
  {
    FuturesMarketSnapshot *ms = iter->second;
    if(ms!=NULL)
    {
      delete ms;
      ms = NULL;
    }
  }
  future_l1_snapshots_.clear();

  for(map<std::string,CFEL2MarketSnapshot* >::iterator iter = cffe_l2_snapshots_.begin();iter!=cffe_l2_snapshots_.end();++iter)
  {
    CFEL2MarketSnapshot *ms = iter->second;
    if(ms!=NULL)
    {
      delete ms;
      ms = NULL;
    }
  }
  cffe_l2_snapshots_.clear();

}

void wmdf::MarketSnapshotContainer::ClearL1Transaction()
{
	for(int32_t i=0;i!=l1_transactions_.size();++i)
	{
		if(l1_transactions_[i]!=NULL)
		{
			delete l1_transactions_[i];
			l1_transactions_[i] = NULL;
		}
	}
	l1_transactions_.clear();
}








