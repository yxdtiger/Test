#include "market_snapshot_container.h"
#include "../../include/date_time.h"
#include "product_target.h"
using namespace std;
#include "../dbf/dbf.h"

wmdf::MarketDataControl::MarketDataControl()
{
}

wmdf::MarketDataControl::~MarketDataControl()
{
	for(map<std::string,L2MarketSnapshot* >::iterator iter = ssel2_market_datas_.begin();iter!=ssel2_market_datas_.end();++iter)
	{
		L2MarketSnapshot *ms = iter->second;
		if(ms!=NULL)
		{
			delete ms;
			ms = NULL;
		}
	}

  for(map<std::string,L2MarketSnapshot* >::iterator iter = szsel2_market_datas_.begin();iter!=szsel2_market_datas_.end();++iter)
  {
    L2MarketSnapshot *ms = iter->second;
    if(ms!=NULL)
    {
      delete ms;
      ms = NULL;
    }
  }

  for(map<std::string,IndexSnapshot* >::iterator iter = ftse_market_datas_.begin();iter!=ftse_market_datas_.end();++iter)
  {
    IndexSnapshot *ms = iter->second;
    if(ms!=NULL)
    {
      delete ms;
      ms = NULL;
    }
  }
}


void wmdf::MarketDataControl::MergeSSEL1MarketSnapshot( L1MarketSnapshot* marketsnapshot )
{
  boost::mutex::scoped_lock lock(ssel1_mkt_mutex_);
  string windcode = marketsnapshot->wind_code;
  map<string,L1MarketSnapshot* >::iterator iter = ssel1_market_datas_.find(windcode);
  if(iter == ssel1_market_datas_.end())
  {
    L1MarketSnapshot *ms = new L1MarketSnapshot();
    memcpy(ms,marketsnapshot,sizeof(L1MarketSnapshot));
    ssel1_market_datas_.insert(pair<string,L1MarketSnapshot*>(windcode,ms));
  }
  else
  {
    memcpy(iter->second,marketsnapshot,sizeof(L1MarketSnapshot));
  }
}


void wmdf::MarketDataControl::MergeSZSEL1MarketSnapshot( L1MarketSnapshot* marketsnapshot )
{
  boost::mutex::scoped_lock lock(szsel1_mkt_mutex_);
  string windcode = marketsnapshot->wind_code;
  map<string,L1MarketSnapshot* >::iterator iter = szsel1_market_datas_.find(windcode);
  if(iter == szsel1_market_datas_.end())
  {
    L1MarketSnapshot *ms = new L1MarketSnapshot();
    memcpy(ms,marketsnapshot,sizeof(L1MarketSnapshot));
    szsel1_market_datas_.insert(pair<string,L1MarketSnapshot*>(windcode,ms));
  }
  else
  {
    memcpy(iter->second,marketsnapshot,sizeof(L1MarketSnapshot));
  }
}

void wmdf::MarketDataControl::MergeSSEMarketSnapshot( L2MarketSnapshot *marketsnapshot )
{
  boost::mutex::scoped_lock lock(ssel2_mkt_mutex_);
	string windcode = marketsnapshot->wind_code;
	map<string,L2MarketSnapshot* >::iterator iter = ssel2_market_datas_.find(windcode);
	if(iter == ssel2_market_datas_.end())
	{
		L2MarketSnapshot *ms = new L2MarketSnapshot();
		memcpy(ms,marketsnapshot,sizeof(L2MarketSnapshot));
		ssel2_market_datas_.insert(pair<string,L2MarketSnapshot*>(windcode,ms));
	}
	else
	{
		L2MarketSnapshot *ms = iter->second;
		UpdateMarketSnapshot(ms,marketsnapshot);
	}
}

void wmdf::MarketDataControl::MergeSZSEMarketSnapshot( L2MarketSnapshot *marketsnapshot )
{
  boost::mutex::scoped_lock lock(ssel2_mkt_mutex_);
  string windcode = marketsnapshot->wind_code;
  map<string,L2MarketSnapshot* >::iterator iter = szsel2_market_datas_.find(windcode);
  if(iter == szsel2_market_datas_.end())
  {
    L2MarketSnapshot *ms = new L2MarketSnapshot();
    memcpy(ms,marketsnapshot,sizeof(L2MarketSnapshot));
    szsel2_market_datas_.insert(pair<string,L2MarketSnapshot*>(windcode,ms));
  }
  else
  {
    L2MarketSnapshot *ms = iter->second;
    UpdateMarketSnapshot(ms,marketsnapshot);
  }
}

void wmdf::MarketDataControl::MergeFTSEIndexSnapshot( IndexSnapshot* marketsnapshot )
{
  boost::mutex::scoped_lock lock(ftse_mkt_mutex_);
  string windcode = marketsnapshot->wind_code;
  map<string,IndexSnapshot* >::iterator iter = ftse_market_datas_.find(windcode);
  if(iter == ftse_market_datas_.end())
  {
    IndexSnapshot *index_s = new IndexSnapshot();
    memcpy(index_s,marketsnapshot,sizeof(IndexSnapshot));
    ftse_market_datas_.insert(pair<string,IndexSnapshot*>(windcode,index_s));
  }
  else
  {
    /*memcpy(iter->second,marketsnapshot,sizeof(IndexSnapshot));*/
    IndexSnapshot *ms = iter->second;
    UpdateIndexSnapshot(ms,marketsnapshot);
  }
}

void wmdf::MarketDataControl::MergeCFFEL1MarketSnapshot( FuturesMarketSnapshot* marketsnapshot )
{
  boost::mutex::scoped_lock lock(cffel1_mkt_mutex_);
  string windcode = marketsnapshot->wind_code;
  map<string,FuturesMarketSnapshot* >::iterator iter = cffel1_market_datas_.find(windcode);
  if(iter == cffel1_market_datas_.end())
  {
    FuturesMarketSnapshot *item = new FuturesMarketSnapshot();
    memcpy(item,marketsnapshot,sizeof(FuturesMarketSnapshot));
    cffel1_market_datas_.insert(pair<string,FuturesMarketSnapshot*>(windcode,item));
  }
  else
  {
    memcpy(iter->second,marketsnapshot,sizeof(FuturesMarketSnapshot));
  }
}

void wmdf::MarketDataControl::MergeCFFEL2MarketSnapshot( CFEL2MarketSnapshot* marketsnapshot )
{
  boost::mutex::scoped_lock lock(cffel2_mkt_mutex_);
  string windcode = marketsnapshot->wind_code;
  map<string,CFEL2MarketSnapshot* >::iterator iter = cffel2_market_datas_.find(windcode);
  if(iter == cffel2_market_datas_.end())
  {
    CFEL2MarketSnapshot *item = new CFEL2MarketSnapshot();
    memcpy(item,marketsnapshot,sizeof(CFEL2MarketSnapshot));
    cffel2_market_datas_.insert(pair<string,CFEL2MarketSnapshot*>(windcode,item));
  }
  else
  {
    memcpy(iter->second,marketsnapshot,sizeof(CFEL2MarketSnapshot));
  }
}

void wmdf::MarketDataControl::MergeSHFEMarketSnapshot( FuturesMarketSnapshot* marketsnapshot )
{
  boost::mutex::scoped_lock lock(shfe_mkt_mutex_);
  string windcode = marketsnapshot->wind_code;
  map<string,FuturesMarketSnapshot* >::iterator iter = shfe_market_datas_.find(windcode);
  if(iter == shfe_market_datas_.end())
  {
    FuturesMarketSnapshot *item = new FuturesMarketSnapshot();
    memcpy(item,marketsnapshot,sizeof(FuturesMarketSnapshot));
    shfe_market_datas_.insert(pair<string,FuturesMarketSnapshot*>(windcode,item));
  }
  else
  {
    memcpy(iter->second,marketsnapshot,sizeof(FuturesMarketSnapshot));
  }
}

void wmdf::MarketDataControl::MergeDCEMarketSnapshot( FuturesMarketSnapshot* marketsnapshot )
{
  boost::mutex::scoped_lock lock(dce_mkt_mutex_);
  string windcode = marketsnapshot->wind_code;
  map<string,FuturesMarketSnapshot* >::iterator iter = dce_market_datas_.find(windcode);
  if(iter == dce_market_datas_.end())
  {
    FuturesMarketSnapshot *item = new FuturesMarketSnapshot();
    memcpy(item,marketsnapshot,sizeof(FuturesMarketSnapshot));
    dce_market_datas_.insert(pair<string,FuturesMarketSnapshot*>(windcode,item));
  }
  else
  {
    memcpy(iter->second,marketsnapshot,sizeof(FuturesMarketSnapshot));
  }
}

void wmdf::MarketDataControl::MergeCZCEMarketSnapshot( FuturesMarketSnapshot* marketsnapshot )
{
  boost::mutex::scoped_lock lock(czce_mkt_mutex_);
  string windcode = marketsnapshot->wind_code;
  map<string,FuturesMarketSnapshot* >::iterator iter = czce_market_datas_.find(windcode);
  if(iter == czce_market_datas_.end())
  {
    FuturesMarketSnapshot *item = new FuturesMarketSnapshot();
    memcpy(item,marketsnapshot,sizeof(FuturesMarketSnapshot));
    czce_market_datas_.insert(pair<string,FuturesMarketSnapshot*>(windcode,item));
  }
  else
  {
    memcpy(iter->second,marketsnapshot,sizeof(FuturesMarketSnapshot));
  }
}

void wmdf::MarketDataControl::OutputSSEL1MarketData( std::string file_path )
{
  if(ssel1_market_datas_.size()<=0)
    return;
#define SSEL1_FIELD_LIST(OP,N_OP)           \
  OP(wind_code,   S1,     C)                \
  N_OP(NULL,      S2,     C8)               \
  OP(pre_close,   S3,     F83)              \
  OP(open_price,  S4,     F83)              \
  OP(total_amount,S5,     L)                \
  OP(high_price,  S6,     F83)              \
  OP(low_price,   S7,     F83)              \
  OP(new_price,   S8,     F83)              \
  OP(bid_price1,  S9,     F83)              \
  OP(ask_price1,  S10,    F83)              \
  OP(total_volume,S11,    I)                \
  N_OP(NULL,      S13,    F83)              \
  OP(bid_size1,   S15,    I)                \
  OP(bid_price2,  S16,    F83)              \
  OP(bid_size2,   S17,    I)                \
  OP(bid_price3,  S18,    F83)              \
  OP(bid_size3,   S19,    I)                \
  OP(ask_size1,   S21,    I)                \
  OP(ask_price2,  S22,    F83)              \
  OP(ask_size2,   S23,    I)                \
  OP(ask_price3,  S24,    F83)              \
  OP(ask_size3,   S25,    I)                \
  OP(bid_price4,  S26,    F83)              \
  OP(bid_size4,   S27,    I)                \
  OP(bid_price5,  S28,    F83)              \
  OP(bid_size5,   S29,    I)                \
  OP(ask_price4,  S30,    F83)              \
  OP(ask_size4,   S31,    I)                \
  OP(ask_price5,  S32,    F83)              \
  OP(ask_size5,   S33,    I)

  const char* schema_str = SSEL1_FIELD_LIST(MAP_SCHEMA,MAP_SCHEMA);

  boost::mutex::scoped_lock lock(ssel1_mkt_mutex_);
  dbhead_t *dbh = dbf_new(file_path.c_str(),schema_str);
  if(dbh==NULL)
    return;
  dbf_append(dbh, ssel1_market_datas_.size());  
  map<std::string,L1MarketSnapshot* >::iterator iter = ssel1_market_datas_.begin();
  L1MarketSnapshot *item = NULL;
  while(iter!=ssel1_market_datas_.end()&&dbf_next(dbh))
  {
    int field = 0;
    item = iter->second;
    SSEL1_FIELD_LIST(MAP_SETTER,MAP_N_SETTER);
    iter++;
  }
  dbf_flush(dbh);
  dbf_destroy(dbh);
}

void wmdf::MarketDataControl::OutputSZSEL1MarketData( std::string file_path )
{
  if(szsel1_market_datas_.size()<=0)
    return;
  #define SZSEL1_FIELD_LIST(OP,N_OP)            \
  OP(wind_code,   HQZQDM,     C)                \
  N_OP(NULL,      HQZQJC,     C8)               \
  OP(pre_close,   HQZRSP,     F93)              \
  OP(open_price,  HQJRKP,     F93)              \
  OP(new_price,   HQZJCJ,     F93)              \
  OP(total_volume,HQCJSL,     L)                \
  OP(total_amount,HQCJJE,     F173)             \
  N_OP(NULL,      HQCJBS,     S)                \
  OP(high_price,  HQZGCJ,     F93)              \
  OP(low_price,   HQZDCJ,     F93)              \
  N_OP(NULL,      HQSYL1,     F72)              \
  N_OP(NULL,      HQSYL2,     F72)              \
  N_OP(NULL,      HQJSD1,     F93)              \
  N_OP(NULL,      HQJSD2,     F93)              \
  N_OP(NULL,      HQHYCC,     L)                \
  OP(ask_price5,  HQSJW5,     F93)              \
  OP(ask_size5,   HQSSL5,     L)                \
  OP(ask_price4,  HQSJW4,     F93)              \
  OP(ask_size4,   HQSSL4,     L)                \
  OP(ask_price3,  HQSJW3,     F93)              \
  OP(ask_size3,   HQSSL3,     L)                \
  OP(ask_price2,  HQSJW2,     F93)              \
  OP(ask_size2,   HQSSL2,     L)                \
  OP(ask_price1,  HQSJW1,     F93)              \
  OP(ask_size1,   HQSSL1,     L)                \
  OP(bid_price1,  HQBJW1,     F93)              \
  OP(bid_size1,   HQBSL1,     L)                \
  OP(bid_price2,  HQBJW2,     F93)              \
  OP(bid_size2,   HQBSL2,     L)                \
  OP(bid_price3,  HQBJW3,     F93)              \
  OP(bid_size3,   HQBSL3,     L)                \
  OP(bid_price4,  HQBJW4,     F93)              \
  OP(bid_size4,   HQBSL4,     L)                \
  OP(bid_price5,  HQBJW5,     F93)              \
  OP(bid_size5,   HQBSL5,     L)

  const char* schema_str = SZSEL1_FIELD_LIST(MAP_SCHEMA,MAP_SCHEMA);

  boost::mutex::scoped_lock lock(szsel1_mkt_mutex_);
  dbhead_t *dbh = dbf_new(file_path.c_str(),schema_str);
  if(dbh==NULL)
    return;
  dbf_append(dbh, szsel1_market_datas_.size());  
  map<std::string,L1MarketSnapshot* >::iterator iter = szsel1_market_datas_.begin();
  L1MarketSnapshot *item = NULL;
  while(iter!=szsel1_market_datas_.end()&&dbf_next(dbh))
  {
    int field = 0;
    item = iter->second;
    SZSEL1_FIELD_LIST(MAP_SETTER,MAP_N_SETTER);
    iter++;
  }
  dbf_flush(dbh);
  dbf_destroy(dbh);
}

void wmdf::MarketDataControl::OutputSSEL2MarketData(std::string file_path)
{
  if(ssel2_market_datas_.size()<=0)
    return;
  #define SSE_FIELD_LIST(OP,N_OP)                    \
  OP(data_time_stamp,time,      T)              \
  OP(wind_code,   windcode,    C)               \
  OP(pre_close,   preclose,    F82)               \
  OP(open_price,  open,   F82)                    \
  OP(high_price,  high,   F82)                    \
  OP(low_price,   low,    F82)                    \
  OP(new_price,   new,    F82)                    \
  OP(total_volume, volume,L)                    \
  OP(total_amount, amount,F82)                    \
  OP(transactions_count, dealcount,L)           \
  OP(peratio2,    peratio,    F82)                \
  OP(bid_price1,  bid1,    F82)                   \
  OP(bid_price2,  bid2,    F82)                   \
  OP(bid_price3,  bid3,    F82)                   \
  OP(bid_price4,  bid4,    F82)                   \
  OP(bid_price5,  bid5,    F82)                   \
  OP(bid_price6,  bid6,    F82)                   \
  OP(bid_price7,  bid7,    F82)                   \
  OP(bid_price8,  bid8,    F82)                   \
  OP(bid_price9,  bid9,    F82)                   \
  OP(bid_price10, bid10,   F82)                   \
  OP(bid_size1,   bsize1,     I)                \
  OP(bid_size2,   bsize2,     I)                \
  OP(bid_size3,   bsize3,     I)                \
  OP(bid_size4,   bsize4,     I)                \
  OP(bid_size5,   bsize5,     I)                \
  OP(bid_size6,   bsize6,     I)                \
  OP(bid_size7,   bsize7,     I)                \
  OP(bid_size8,   bsize8,     I)                \
  OP(bid_size9,   bsize9,     I)                \
  OP(bid_size10,  bsize10,    I)                \
  OP(ask_price1,  ask1,    F82)                   \
  OP(ask_price2,  ask2,    F82)                   \
  OP(ask_price3,  ask3,    F82)                   \
  OP(ask_price4,  ask4,    F82)                   \
  OP(ask_price5,  ask5,    F82)                   \
  OP(ask_price6,  ask6,    F82)                   \
  OP(ask_price7,  ask7,    F82)                   \
  OP(ask_price8,  ask8,    F82)                   \
  OP(ask_price9,  ask9,    F82)                   \
  OP(ask_price10, ask10,   F82)                   \
  OP(ask_size1,   asize1,     I)                \
  OP(ask_size2,   asize2,     I)                \
  OP(ask_size3,   asize3,     I)                \
  OP(ask_size4,   asize4,     I)                \
  OP(ask_size5,   asize5,     I)                \
  OP(ask_size6,   asize6,     I)                \
  OP(ask_size7,   asize7,     I)                \
  OP(ask_size8,   asize8,     I)                \
  OP(ask_size9,   asize9,     I)                \
  OP(ask_size10,  asize10,    I)

  const char* schema_str = SSE_FIELD_LIST(MAP_SCHEMA,MAP_SCHEMA);

  boost::mutex::scoped_lock lock(ssel2_mkt_mutex_);
  dbhead_t *dbh = dbf_new(file_path.c_str(),schema_str);
  if(dbh==NULL)
    return;
  dbf_append(dbh, ssel2_market_datas_.size());  
  map<std::string,L2MarketSnapshot* >::iterator iter = ssel2_market_datas_.begin();
  L2MarketSnapshot *item = NULL;
  while(iter!=ssel2_market_datas_.end()&&dbf_next(dbh))
  {
    int field = 0;
    item = iter->second;
    SSE_FIELD_LIST(MAP_SETTER,MAP_SETTER);
    iter++;
  }
  dbf_flush(dbh);
  dbf_destroy(dbh);
}

void wmdf::MarketDataControl::OutputSZSEL2MarketData(std::string file_path)
{
  if(szsel2_market_datas_.size()<=0)
    return;
  #define SZSE_FIELD_LIST(OP,N_OP) \
  OP(data_time_stamp,time,T)              \
  OP(wind_code,      windcode,      C)                \
  OP(pre_close,      preclose,      F82)              \
  OP(open_price,     open,     F82)              \
  OP(high_price,     high,     F82)              \
  OP(low_price,      low,      F82)              \
  OP(new_price,      new,      F82)              \
  OP(total_volume,   volume,   L)              \
  OP(total_amount,   amount,   F82)              \
  OP(transactions_count,dealcount,    L)        \
  OP(peratio1,    peratio1,      F82)              \
  OP(peratio2,    peratio2,      F82)              \
  OP(bid_price1,  bid1,    F82)              \
  OP(bid_price2,  bid2,    F82)              \
  OP(bid_price3,  bid3,    F82)              \
  OP(bid_price4,  bid4,    F82)              \
  OP(bid_price5,  bid5,    F82)              \
  OP(bid_price6,  bid6,    F82)              \
  OP(bid_price7,  bid7,    F82)              \
  OP(bid_price8,  bid8,    F82)              \
  OP(bid_price9,  bid9,    F82)              \
  OP(bid_price10, bid10,   F82)              \
  OP(bid_size1,   bsize1,     I)              \
  OP(bid_size2,   bsize2,     I)              \
  OP(bid_size3,   bsize3,     I)              \
  OP(bid_size4,   bsize4,     I)              \
  OP(bid_size5,   bsize5,     I)              \
  OP(bid_size6,   bsize6,     I)              \
  OP(bid_size7,   bsize7,     I)              \
  OP(bid_size8,   bsize8,     I)              \
  OP(bid_size9,   bsize9,     I)              \
  OP(bid_size10,  bsize10,    I)              \
  OP(ask_price1,  ask1,    F82)              \
  OP(ask_price2,  ask2,    F82)              \
  OP(ask_price3,  ask3,    F82)              \
  OP(ask_price4,  ask4,    F82)              \
  OP(ask_price5,  ask5,    F82)              \
  OP(ask_price6,  ask6,    F82)              \
  OP(ask_price7,  ask7,    F82)              \
  OP(ask_price8,  ask8,    F82)              \
  OP(ask_price9,  ask9,    F82)              \
  OP(ask_price10, ask10,   F82)              \
  OP(ask_size1,   asize1,     I)              \
  OP(ask_size2,   asize2,     I)              \
  OP(ask_size3,   asize3,     I)              \
  OP(ask_size4,   asize4,     I)              \
  OP(ask_size5,   asize5,     I)              \
  OP(ask_size6,   asize6,     I)              \
  OP(ask_size7,   asize7,     I)              \
  OP(ask_size8,   asize8,     I)              \
  OP(ask_size9,   asize9,     I)              \
  OP(ask_size10,  asize10,    I)

  boost::mutex::scoped_lock lock(szsel2_mkt_mutex_);
  const char* schema_str = SZSE_FIELD_LIST(MAP_SCHEMA,MAP_SCHEMA);
  dbhead_t *dbh = dbf_new(file_path.c_str(), schema_str);
  if(dbh==NULL)
    return;
  dbf_append(dbh, szsel2_market_datas_.size());

  map<std::string,L2MarketSnapshot* >::iterator iter = szsel2_market_datas_.begin();
  L2MarketSnapshot *item = NULL;
  
  while(iter!=szsel2_market_datas_.end()&&dbf_next(dbh))
  {
    int field = 0;
    item = iter->second;
    SZSE_FIELD_LIST(MAP_SETTER,MAP_SETTER);
    iter++;
  }
  dbf_flush(dbh);
  dbf_destroy(dbh);
}

void wmdf::MarketDataControl::OutputFTSEIndexMarketData( std::string file_path )
{
  if(ftse_market_datas_.size()<=0)
    return;
  #define FTSE_FIELD_LIST(OP,N_OP)                 \
  OP(   wind_code,      L1,      C)                \
  N_OP( NULL,           L2,      C8)                \
  OP(   pre_close,      L3,      F82)                \
  OP(   open_price,     L4,      F82)                \
  N_OP( amount,         L5,      L)                \
  OP(   high_price,     L6,      F82)                \
  OP(   low_price,      L7,      F82)                \
  OP(   new_price,      L8,      F82)                \
  N_OP( NULL,           L9,      F82)                \
  N_OP( NULL,           L10,     F82)                \
  N_OP( volume,         L11,     I)                \
  N_OP( NULL,           L13,     F82)                \
  N_OP( NULL,           L15,     I)                \
  N_OP( NULL,           L16,     F82)                \
  N_OP( NULL,           L17,     I)                \
  N_OP( NULL,           L18,     F82)                \
  N_OP( NULL,           L19,     I)                \
  N_OP( NULL,           L21,     I)                \
  N_OP( NULL,           L22,     F82)                \
  N_OP( NULL,           L23,     I)                \
  N_OP( NULL,           L24,     F82)                \
  N_OP( NULL,           L25,     I)                
  

  boost::mutex::scoped_lock lock(ftse_mkt_mutex_);
  const char* schema_str = FTSE_FIELD_LIST(MAP_SCHEMA,MAP_SCHEMA);
  dbhead_t *dbh = dbf_new(file_path.c_str(), schema_str);
  if(dbh==NULL)
    return;
  dbf_append(dbh, ftse_market_datas_.size());

  map<std::string,IndexSnapshot* >::iterator iter = ftse_market_datas_.begin();
  IndexSnapshot *item = NULL;

  while(iter!=ftse_market_datas_.end()&&dbf_next(dbh))
  {
    int field = 0;
    item = iter->second;
    FTSE_FIELD_LIST(MAP_SETTER,MAP_N_SETTER);
    iter++;
  }
  dbf_flush(dbh);
  dbf_destroy(dbh);

}

void wmdf::MarketDataControl::OutputCFFEL1MarketData( std::string file_path )
{
  if(cffel1_market_datas_.size()<=0)
    return;
#define CFFEL1_FIELD_LIST(OP,N_OP)                  \
  OP(data_time_stamp,time,T)                   \
  OP(wind_code,      windcode,      C)         \
  OP(pre_close,      preclose,      F82)         \
  OP(open_price,     open,     F82)              \
  OP(high_price,     high,     F82)              \
  OP(low_price,      low,      F82)              \
  OP(new_price,      new,      F82)              \
  OP(total_volume, volume,     L)              \
  OP(total_amount, amount,     F82)              \
  OP(bid_price1,  bid1,        F82)              \
  OP(bid_size1,   bsize1,      I)              \
  OP(ask_price1,  ask1,        F82)              \
  OP(ask_size1,   asize1,      I)              \
  OP(pre_settle,  presettle,   F82)              \
  OP(pre_position,preposition, L)              \
  OP(position,    position,    L)              \
  OP(high_limit,  highlimit,   F82)              \
  OP(low_limit,   lowlimit,    F82)

  boost::mutex::scoped_lock lock(cffel1_mkt_mutex_);
  const char* schema_str = CFFEL1_FIELD_LIST(MAP_SCHEMA,MAP_SCHEMA);
  dbhead_t *dbh = dbf_new(file_path.c_str(), schema_str);
  if(dbh==NULL)
    return;
  dbf_append(dbh, cffel1_market_datas_.size());

  map<std::string,FuturesMarketSnapshot* >::iterator iter = cffel1_market_datas_.begin();
  FuturesMarketSnapshot *item = NULL;

  while(iter!=cffel1_market_datas_.end()&&dbf_next(dbh))
  {
    int field = 0;
    item = iter->second;
    CFFEL1_FIELD_LIST(MAP_SETTER,MAP_N_SETTER); 
    iter++;
  }
  dbf_flush(dbh);
  dbf_destroy(dbh);
}

void wmdf::MarketDataControl::OutputCFFEL2MarketData( std::string file_path )
{
  if(cffel2_market_datas_.size()<=0)
    return;
#define CFFEL2_FIELD_LIST(OP,N_OP)             \
  OP(data_time_stamp,time,T)                   \
  OP(wind_code,      windcode,      C)         \
  OP(pre_close,      preclose,      F82)       \
  OP(open_price,     open,     F82)            \
  OP(high_price,     high,     F82)            \
  OP(low_price,      low,      F82)            \
  OP(new_price,      new,      F82)            \
  OP(total_volume, volume,     L)              \
  OP(total_amount, amount,     F82)            \
  OP(bid_price1,  bid1,        F82)            \
  OP(bid_price2,  bid2,        F82)            \
  OP(bid_price3,  bid3,        F82)            \
  OP(bid_price4,  bid4,        F82)            \
  OP(bid_price5,  bid5,        F82)            \
  OP(bid_size1,   bsize1,      I)              \
  OP(bid_size2,   bsize2,      I)              \
  OP(bid_size3,   bsize3,      I)              \
  OP(bid_size4,   bsize4,      I)              \
  OP(bid_size5,   bsize5,      I)              \
  OP(ask_price1,  ask1,        F82)            \
  OP(ask_price2,  ask2,        F82)            \
  OP(ask_price3,  ask3,        F82)            \
  OP(ask_price4,  ask4,        F82)            \
  OP(ask_price5,  ask5,        F82)            \
  OP(ask_size1,   asize1,      I)              \
  OP(ask_size2,   asize2,      I)              \
  OP(ask_size3,   asize3,      I)              \
  OP(ask_size4,   asize4,      I)              \
  OP(ask_size5,   asize5,      I)              \
  OP(settle,  settle,   F82)                   \
  OP(pre_settle,  presettle,   F82)            \
  OP(pre_position,preposition, L)              \
  OP(position,    position,    L)              \
  OP(high_limit,  highlimit,   F82)            \
  OP(low_limit,   lowlimit,    F82)

  boost::mutex::scoped_lock lock(cffel2_mkt_mutex_);
  const char* schema_str = CFFEL2_FIELD_LIST(MAP_SCHEMA,MAP_SCHEMA);
  dbhead_t *dbh = dbf_new(file_path.c_str(), schema_str);
  if(dbh==NULL)
    return;
  dbf_append(dbh, cffel2_market_datas_.size());

  map<std::string,CFEL2MarketSnapshot* >::iterator iter = cffel2_market_datas_.begin();
  CFEL2MarketSnapshot *item = NULL;

  while(iter!=cffel2_market_datas_.end()&&dbf_next(dbh))
  {
    int field = 0;
    item = iter->second;
    CFFEL2_FIELD_LIST(MAP_SETTER,MAP_SETTER); 
    iter++;
  }
  dbf_flush(dbh);
  dbf_destroy(dbh);
}

void wmdf::MarketDataControl::OutputSHFEMarketData( std::string file_path )
{
  if(shfe_market_datas_.size()<=0)
    return;
#define SHFE_FIELD_LIST(OP,N_OP)               \
  OP(data_time_stamp,time,T)                   \
  OP(wind_code,      windcode,      C)         \
  OP(pre_close,      preclose,      F82)       \
  OP(open_price,     open,     F82)            \
  OP(high_price,     high,     F82)            \
  OP(low_price,      low,      F82)            \
  OP(new_price,      new,      F82)            \
  OP(total_volume, volume,     L)              \
  OP(total_amount, amount,     F82)            \
  OP(bid_price1,  bid1,        F82)            \
  OP(bid_size1,   bsize1,      I)              \
  OP(ask_price1,  ask1,        F82)            \
  OP(ask_size1,   asize1,      I)              \
  OP(high_limit,  highlimit,   F82)            \
  OP(low_limit,   lowlimit,    F82)            \
  OP(pre_settle,  presettle,   F82)            \
  OP(settle,      settle,      F82)            \
  OP(pre_position,preposition, L)              \
  OP(position,    position,    L)

  boost::mutex::scoped_lock lock(shfe_mkt_mutex_);
  const char* schema_str = SHFE_FIELD_LIST(MAP_SCHEMA,MAP_SCHEMA);
  dbhead_t *dbh = dbf_new(file_path.c_str(), schema_str);
  if(dbh==NULL)
    return;
  dbf_append(dbh, shfe_market_datas_.size());

  map<std::string,FuturesMarketSnapshot* >::iterator iter = shfe_market_datas_.begin();
  FuturesMarketSnapshot *item = NULL;

  while(iter!=shfe_market_datas_.end()&&dbf_next(dbh))
  {
    int field = 0;
    item = iter->second;
    SHFE_FIELD_LIST(MAP_SETTER,MAP_SETTER); 
    iter++;
  }
  dbf_flush(dbh);
  dbf_destroy(dbh);
}

void wmdf::MarketDataControl::OutputDCEMarketData( std::string file_path )
{
  if(dce_market_datas_.size()<=0)
    return;
#define DCE_FIELD_LIST(OP,N_OP)                \
  OP(data_time_stamp,time,T)                   \
  OP(wind_code,      windcode,      C)         \
  OP(pre_close,      preclose,      F82)       \
  OP(open_price,     open,     F82)            \
  OP(high_price,     high,     F82)            \
  OP(low_price,      low,      F82)            \
  OP(new_price,      new,      F82)            \
  OP(total_volume, volume,     L)              \
  OP(total_amount, amount,     F82)            \
  OP(bid_price1,  bid1,        F82)            \
  OP(bid_size1,   bsize1,      I)              \
  OP(ask_price1,  ask1,        F82)            \
  OP(ask_size1,   asize1,      I)              \
  OP(high_limit,  highlimit,   F82)            \
  OP(low_limit,   lowlimit,    F82)            \
  OP(pre_settle,  presettle,   F82)            \
  OP(settle,      settle,      F82)            \
  OP(pre_position,preposition, L)              \
  OP(position,    position,    L)

  boost::mutex::scoped_lock lock(dce_mkt_mutex_);
  const char* schema_str = DCE_FIELD_LIST(MAP_SCHEMA,MAP_SCHEMA);
  dbhead_t *dbh = dbf_new(file_path.c_str(), schema_str);
  if(dbh==NULL)
    return;
  dbf_append(dbh, dce_market_datas_.size());

  map<std::string,FuturesMarketSnapshot* >::iterator iter = dce_market_datas_.begin();
  FuturesMarketSnapshot *item = NULL;

  while(iter!=dce_market_datas_.end()&&dbf_next(dbh))
  {
    int field = 0;
    item = iter->second;
    DCE_FIELD_LIST(MAP_SETTER,MAP_SETTER); 
    iter++;
  }
  dbf_flush(dbh);
  dbf_destroy(dbh);
}

void wmdf::MarketDataControl::OutputCZCEMarketData( std::string file_path )
{
  if(czce_market_datas_.size()<=0)
    return;
#define CZCE_FIELD_LIST(OP,N_OP)                  \
  OP(data_time_stamp,time,T)                   \
  OP(wind_code,      windcode,      C)         \
  OP(pre_close,      preclose,      F82)         \
  OP(open_price,     open,     F82)              \
  OP(high_price,     high,     F82)              \
  OP(low_price,      low,      F82)              \
  OP(new_price,      new,      F82)              \
  OP(total_volume, volume,     L)              \
  OP(total_amount, amount,     F82)              \
  OP(bid_price1,  bid1,        F82)              \
  OP(bid_size1,   bsize1,      I)              \
  OP(ask_price1,  ask1,        F82)              \
  OP(ask_size1,   asize1,      I)              \
  OP(pre_settle,  presettle,   F82)              \
  OP(settle,      settle,      F82)              \
  OP(position,    position,    L)              \
  OP(average_price,avgprice,   F82)              \
  OP(high_limit,  highlimit,   F82)              \
  OP(low_limit,   lowlimit,    F82)

  boost::mutex::scoped_lock lock(cffel1_mkt_mutex_);
  const char* schema_str = CZCE_FIELD_LIST(MAP_SCHEMA,MAP_SCHEMA);
  dbhead_t *dbh = dbf_new(file_path.c_str(), schema_str);
  if(dbh==NULL)
    return;
  dbf_append(dbh, czce_market_datas_.size());

  map<std::string,FuturesMarketSnapshot* >::iterator iter = czce_market_datas_.begin();
  FuturesMarketSnapshot *item = NULL;

  while(iter!=czce_market_datas_.end()&&dbf_next(dbh))
  {
    int field = 0;
    item = iter->second;
    CZCE_FIELD_LIST(MAP_SETTER,MAP_SETTER); 
    iter++;
  }
  dbf_flush(dbh);
  dbf_destroy(dbh);
}

void wmdf::MarketDataControl::OutputMarketData( int16_t product_id,std::string file_path )
{
  switch(product_id)
  {
  case PRODUCT_SSE_LEVEL1_ID:
    OutputSSEL1MarketData(file_path);
    break;
  case PRODUCT_SZSE_LEVEL1_ID:
    OutputSZSEL1MarketData(file_path);
    break;
  case PRODUCT_SSE_LEVEL2_ID:
    OutputSSEL2MarketData(file_path);
    break;
  case PRODUCT_SZSE_LEVEL2_ID:
    OutputSZSEL2MarketData(file_path);
    break;
  case PRODUCT_CFFE_LEVEL1_ID:
    OutputCFFEL1MarketData(file_path);
    break;
  case PRODUCT_CFFE_LEVEL2_ID:
    OutputCFFEL2MarketData(file_path);
    break;
  case PRODUCT_SHFE_LEVEL1_ID:
    OutputSHFEMarketData(file_path);
    break;
  case PRODUCT_DCE_LEVEL1_ID:
    OutputDCEMarketData(file_path);
    break;
  case PRODUCT_CZCE_LEVEL1_ID:
    OutputCZCEMarketData(file_path);
    break;
  case PRODUCT_FTSE_LEVEL1_ID:
    OutputFTSEIndexMarketData(file_path);
    break;
  default:
    break;
  }
}

void wmdf::MarketDataControl::UpdateL1MarketSnapshot( L1MarketSnapshot* des, L1MarketSnapshot* src )
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
}

void wmdf::MarketDataControl::UpdateMarketSnapshot( L2MarketSnapshot *des, L2MarketSnapshot *src )
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
}



void wmdf::MarketDataControl::UpdateIndexSnapshot( IndexSnapshot* des,IndexSnapshot* src )
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


void wmdf::MarketDataControl::UpdateFutureSnapshot( FuturesMarketSnapshot* des, FuturesMarketSnapshot* src )
{
  if(src->data_time_stamp != WMDF_CONSTS_32BIT_NIL_VALUE)
    des->data_time_stamp = src->data_time_stamp;
  else
    src->data_time_stamp = des->data_time_stamp;

  if(src->trade_date != WMDF_CONSTS_32BIT_NIL_VALUE)
    des->trade_date = src->trade_date;
  else
    src->trade_date = des->trade_date;

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

  if(src->bid_price1 == src->bid_price1)
    des->bid_price1 = src->bid_price1;
  else
    src->bid_price1 = des->bid_price1;

  if(src->ask_price1 == src->ask_price1)
    des->ask_price1 = src->ask_price1;
  else
    src->ask_price1 = des->ask_price1;

  if(src->ask_size1 != WMDF_CONSTS_32BIT_NIL_VALUE)
    des->ask_size1 = src->ask_size1;
  else
    src->ask_size1 = des->ask_size1;

  if(src->bid_size1 != WMDF_CONSTS_32BIT_NIL_VALUE)
    des->bid_size1 = src->bid_size1;
  else
    src->bid_size1 = des->bid_size1;

  if(src->pre_settle == src->pre_settle)
    des->pre_settle = src->pre_settle;
  else
    src->pre_settle = des->pre_settle;

  if(src->pre_position != WMDF_CONSTS_64BIT_NIL_VALUE)
    des->pre_position = src->pre_position;
  else
    src->pre_position = des->pre_position;

  if(src->settle == src->settle)
    des->settle = src->settle;
  else
    src->settle = des->settle;

  if(src->position != WMDF_CONSTS_64BIT_NIL_VALUE)
    des->position = src->position;
  else
    src->position = des->position;

  if(src->average_price == src->average_price)
    des->average_price = src->average_price;
  else
    src->average_price = des->average_price;

  if(src->high_limit == src->high_limit)
    des->high_limit = src->high_limit;
  else
    src->high_limit = des->high_limit;

  if(src->low_limit == src->low_limit)
    des->low_limit = src->low_limit;
  else
    src->low_limit = des->low_limit;

}

void wmdf::MarketDataControl::UpdateCFFEL2Snapshot( CFEL2MarketSnapshot* des, CFEL2MarketSnapshot* src )
{
  if(src->data_time_stamp != WMDF_CONSTS_32BIT_NIL_VALUE)
    des->data_time_stamp = src->data_time_stamp;
  else
    src->data_time_stamp = des->data_time_stamp;

  if(src->trade_date != WMDF_CONSTS_32BIT_NIL_VALUE)
    des->trade_date = src->trade_date;
  else
    src->trade_date = des->trade_date;

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

  if(src->pre_settle == src->pre_settle)
    des->pre_settle = src->pre_settle;
  else
    src->pre_settle = des->pre_settle;

  if(src->pre_position != WMDF_CONSTS_64BIT_NIL_VALUE)
    des->pre_position = src->pre_position;
  else
    src->pre_position = des->pre_position;

  if(src->position != WMDF_CONSTS_64BIT_NIL_VALUE)
    des->position = src->position;
  else
    src->position = des->position;

  if(src->high_limit == src->high_limit)
    des->high_limit = src->high_limit;
  else
    src->high_limit = des->high_limit;

  if(src->low_limit == src->low_limit)
    des->low_limit = src->low_limit;
  else
    src->low_limit = des->low_limit;

	if(src->settle == src->settle)
		des->settle = src->settle;
}

void wmdf::MarketDataControl::Clear( int16_t product_id )
{
  switch(product_id)
  {
  case PRODUCT_SSE_LEVEL1_ID:
    ClearSSEL1MarketData();
    break;
  case PRODUCT_SZSE_LEVEL1_ID:
    ClearSZSEL1MarketData();
    break;
  case PRODUCT_SSE_LEVEL2_ID:
    ClearSSEL2MarketData();
    break;
  case PRODUCT_SZSE_LEVEL2_ID:
    ClearSZSEL2MarketData();
    break;
  case PRODUCT_CFFE_LEVEL1_ID:
    ClearCFFEL1MarketData();
    break;
  case PRODUCT_CFFE_LEVEL2_ID:
    ClearCFFEL2MarketData();
    break;
  case PRODUCT_SHFE_LEVEL1_ID:
    ClearSHFEMarketData();
    break;
  case PRODUCT_DCE_LEVEL1_ID:
    ClearDCEMarketData();
    break;
  case PRODUCT_CZCE_LEVEL1_ID:
    ClearCZCEMarketData();
    break;
  case PRODUCT_FTSE_LEVEL1_ID:
    ClearFTSEMarketData();
    break;
  default:
    break;
  }
}

void wmdf::MarketDataControl::ClearSSEL1MarketData()
{
  for(map<std::string,L1MarketSnapshot* >::iterator iter = ssel1_market_datas_.begin();iter!=ssel1_market_datas_.end();++iter)
  {
    L1MarketSnapshot *ms = iter->second;
    if(ms!=NULL)
    {
      delete ms;
      ms = NULL;
    }
  }
  ssel1_market_datas_.clear();
}

void wmdf::MarketDataControl::ClearSZSEL1MarketData()
{
  for(map<std::string,L1MarketSnapshot* >::iterator iter = szsel1_market_datas_.begin();iter!=szsel1_market_datas_.end();++iter)
  {
    L1MarketSnapshot *ms = iter->second;
    if(ms!=NULL)
    {
      delete ms;
      ms = NULL;
    }
  }
  szsel1_market_datas_.clear();
}

void wmdf::MarketDataControl::ClearSSEL2MarketData()
{
  for(map<std::string,L2MarketSnapshot* >::iterator iter = ssel2_market_datas_.begin();iter!=ssel2_market_datas_.end();++iter)
  {
    L2MarketSnapshot *ms = iter->second;
    if(ms!=NULL)
    {
      delete ms;
      ms = NULL;
    }
  }
  ssel2_market_datas_.clear();
}

void wmdf::MarketDataControl::ClearSZSEL2MarketData()
{
  for(map<std::string,L2MarketSnapshot* >::iterator iter = szsel2_market_datas_.begin();iter!=szsel2_market_datas_.end();++iter)
  {
    L2MarketSnapshot *ms = iter->second;
    if(ms!=NULL)
    {
      delete ms;
      ms = NULL;
    }
  }
  szsel2_market_datas_.clear();
}

void wmdf::MarketDataControl::ClearFTSEMarketData()
{
  for(map<std::string,IndexSnapshot* >::iterator iter = ftse_market_datas_.begin();iter!=ftse_market_datas_.end();++iter)
  {
    IndexSnapshot *ms = iter->second;
    if(ms!=NULL)
    {
      delete ms;
      ms = NULL;
    }
  }
  ftse_market_datas_.clear();
}

void wmdf::MarketDataControl::ClearCFFEL1MarketData()
{
  for(map<std::string,FuturesMarketSnapshot* >::iterator iter = cffel1_market_datas_.begin();iter!=cffel1_market_datas_.end();++iter)
  {
    FuturesMarketSnapshot *ms = iter->second;
    if(ms!=NULL)
    {
      delete ms;
      ms = NULL;
    }
  }
  cffel1_market_datas_.clear();
}

void wmdf::MarketDataControl::ClearCFFEL2MarketData()
{
  for(map<std::string,CFEL2MarketSnapshot* >::iterator iter = cffel2_market_datas_.begin();iter!=cffel2_market_datas_.end();++iter)
  {
    CFEL2MarketSnapshot *ms = iter->second;
    if(ms!=NULL)
    {
      delete ms;
      ms = NULL;
    }
  }
  cffel2_market_datas_.clear();
}

void wmdf::MarketDataControl::ClearSHFEMarketData()
{
  for(map<std::string,FuturesMarketSnapshot* >::iterator iter = shfe_market_datas_.begin();iter!=shfe_market_datas_.end();++iter)
  {
    FuturesMarketSnapshot *ms = iter->second;
    if(ms!=NULL)
    {
      delete ms;
      ms = NULL;
    }
  }
  shfe_market_datas_.clear();
}

void wmdf::MarketDataControl::ClearDCEMarketData()
{
  for(map<std::string,FuturesMarketSnapshot* >::iterator iter = dce_market_datas_.begin();iter!=dce_market_datas_.end();++iter)
  {
    FuturesMarketSnapshot *ms = iter->second;
    if(ms!=NULL)
    {
      delete ms;
      ms = NULL;
    }
  }
  dce_market_datas_.clear();
}

void wmdf::MarketDataControl::ClearCZCEMarketData()
{
  for(map<std::string,FuturesMarketSnapshot* >::iterator iter = czce_market_datas_.begin();iter!=czce_market_datas_.end();++iter)
  {
    FuturesMarketSnapshot *ms = iter->second;
    if(ms!=NULL)
    {
      delete ms;
      ms = NULL;
    }
  }
  czce_market_datas_.clear();
}





