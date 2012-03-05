#include "data_feed.h"
#include <iostream>
#include "../../include/date_time.h"
#include "../../include/wmdf_api.h"
#include "../../include/wmdf_structs.h"
#include "feed_environment.h"


using namespace wmdf;

wmdf::DataFeed::DataFeed(void)
{
  
}

wmdf::DataFeed::~DataFeed(void)
{
  
}

void wmdf::DataFeed::Process( MdfRecord* record )
{
  if(record != NULL)
  {
    if (IsProductExistedInIdMap(record->product_id))
    {
      if (packet_id_map_[record->product_id]+1!=record->packet_num)
      {
        printf("packet lost. product id: %d, pre_packet_num：%d, curr_packet_num: %d \n",record->product_id,packet_id_map_[record->product_id],record->packet_num);
      }
      packet_id_map_[record->product_id] = record->packet_num;
    }
    else
    {
      packet_id_map_.insert(make_pair(record->product_id,record->packet_num));
    }
    if(record->product_id!=PRODEUCT_SYSTEM_ID)
    {
      if(FeedEnvironment::Instance()->GetTargetInfo(record->product_id)==NULL)
        return;
    }    
   
    //上交所level1    
    if(record->product_id==PRODUCT_SSE_LEVEL1_ID)
    {
      if(record->message_id==SSEL1_MARKETDATA_ID)
      {
        ProcessSSEL1MarketData(record);
      }
    }
    //深交所level1
    else if(record->product_id==PRODUCT_SZSE_LEVEL1_ID)
    {
      if(record->message_id==SZSEL1_MARKETDATA_ID)
      {
        ProcessSZSEL1MarketData(record);
      }
    }
    //上交所level2
    else if(record->product_id==PRODUCT_SSE_LEVEL2_ID)
    {
      if(record->message_id==SSEL2_MARKETDATA_ID)
      {
        ProcessSSEL2MarketData(record);
      }
      /*else if(record->message_id==SSEL2_TRANSACTION_ID)
      {
        ProcessSSEL2Transaction(record);
      }
      else if (record->message_id==SSEL2_INDEXDATA_ID)
      {
        ProcessIndexSnapshot(record);
      }*/
    }
    //深交所L2
    else if(record->product_id==PRODUCT_SZSE_LEVEL2_ID)
    {
      if(record->message_id==SZSEL2_MARKETDATA_ID)
      {
        ProcessSZSEL2MarketData(record);
      }
      //else if(record->message_id==SZSEL2_TRANSACTION_ID)
      //{
      //  ProcessSZSEL2Transaction(record);
      //}
      //else if (record->message_id==SZSEL2_INDEXDATA_ID)
      //{
      //  ProcessIndexSnapshot(record);
      //}
      //else if(record->message_id == SZSEL2_SINGLEORDER_ID)
      //{
      //  ProcessSZSEL2Order(record);
      //}
    } 
    else if(record->product_id == PRODUCT_FTSE_LEVEL1_ID)
    {
      ProcessIndexSnapshot(record);
    }
    //中金所L1
    else if(record->product_id==PRODUCT_CFFE_LEVEL1_ID)
    {
      ProcessCFFEL1MarketData(record);
    }
    //中金所L2
    else if(record->product_id==PRODUCT_CFFE_LEVEL2_ID)
    {
      ProcessCFFEL2MarketData(record);
    }
    //大商所L2
    else if(record->product_id==PRODUCT_DCE_LEVEL1_ID)
    {
      ProcessDCEMarketData(record);
    }
    //上期所L1
    else if(record->product_id==PRODUCT_SHFE_LEVEL1_ID)
    {
      ProcessSHFEMarketData(record);
    }
    //郑商所L1
    else if(record->product_id==PRODUCT_CZCE_LEVEL1_ID)
    {
      ProcessCZCEMarketData(record);
    }
  }  
}

void wmdf::DataFeed::ProcessSSEL1MarketData( MdfRecord* record )
{
  L1MarketSnapshot* marketDatas = (L1MarketSnapshot*)record->body;

  for(int i=0;i<record->record_count;i++)
  {
    L1MarketSnapshot* market_snapshot = &marketDatas[i];
    FeedEnvironment::Instance()->data_control()->MergeSSEL1MarketSnapshot(market_snapshot);
  }
}

void wmdf::DataFeed::ProcessSZSEL1MarketData( MdfRecord* record )
{
  L1MarketSnapshot* marketDatas = (L1MarketSnapshot*)record->body;

  for(int i=0;i<record->record_count;i++)
  {
    L1MarketSnapshot* market_snapshot = &marketDatas[i];
    FeedEnvironment::Instance()->data_control()->MergeSZSEL1MarketSnapshot(market_snapshot);
  }
}



void wmdf::DataFeed::ProcessSSEL2MarketData(MdfRecord* record ) 
{
  L2MarketSnapshot* marketDatas = (L2MarketSnapshot*)record->body;
  for(int i=0;i<record->record_count;i++)
  {
    L2MarketSnapshot* market_snapshot = &marketDatas[i];
    FeedEnvironment::Instance()->data_control()->MergeSSEMarketSnapshot(market_snapshot);    
  }
  //hexec(-FeedEnvironment::Instance()->GetClientId(record->product_id),"upd(x,y)",gstring("mkt"),mkt,NULL);
}

void wmdf::DataFeed::ProcessSSEL2IndexData( MdfRecord* record )
{
  //IndexSnapshot* indexDatas = (IndexSnapshot*)record->body;
  //for(int i=0;i<record->record_count;i++)
  //{
  //  IndexSnapshot* index_data = &indexDatas[i];
  //  
  //}
 
}
void wmdf::DataFeed::ProcessSSEL2Transaction(MdfRecord* record ) 
{
  //L2Transaction* trans = (L2Transaction*)record->body;

  //for(int i=0;i<record->record_count;i++)
  //{

  //}
}
void wmdf::DataFeed::ProcessIndexSnapshot( MdfRecord* record )
{
  IndexSnapshot* marketDatas = (IndexSnapshot*)record->body;

  for(int i=0;i<record->record_count;i++)
  {
    IndexSnapshot* market_snapshot = &marketDatas[i];
    FeedEnvironment::Instance()->data_control()->MergeFTSEIndexSnapshot(market_snapshot);
  }
}
void wmdf::DataFeed::ProcessSZSEL2MarketData( MdfRecord* record )
{
  L2MarketSnapshot* marketDatas = (L2MarketSnapshot*)record->body;

  for(int i=0;i<record->record_count;i++)
  {
    L2MarketSnapshot* market_snapshot = &marketDatas[i];
    FeedEnvironment::Instance()->data_control()->MergeSZSEMarketSnapshot(market_snapshot);
  }
}

void wmdf::DataFeed::ProcessSZSEL2Transaction( MdfRecord* record )
{
  /*L2Transaction* trans = (L2Transaction*)record->body;

  for(int i=0;i<record->record_count;i++)
  {

  }*/
}
void wmdf::DataFeed::ProcessSZSEL2Order( MdfRecord* record )
{
  /*SZSEL2Order* single_orders = (SZSEL2Order*)record->body;

  for(int i=0;i<record->record_count;i++)
  {

  }*/
}

void wmdf::DataFeed::ProcessCFFEL1MarketData(MdfRecord* record) 
{
  FuturesMarketSnapshot* marketDatas = (FuturesMarketSnapshot*)record->body;
  for(int i=0;i<record->record_count;i++)
  {
    FuturesMarketSnapshot* market_snapshot = &marketDatas[i];
    FeedEnvironment::Instance()->data_control()->MergeCFFEL1MarketSnapshot(market_snapshot);
  }
}

void wmdf::DataFeed::ProcessCFFEL2MarketData(MdfRecord* record) 
{
  CFEL2MarketSnapshot* marketDatas = (CFEL2MarketSnapshot*)record->body;
  for(int i=0;i<record->record_count;i++)
  {
    CFEL2MarketSnapshot* market_snapshot = &marketDatas[i];
    FeedEnvironment::Instance()->data_control()->MergeCFFEL2MarketSnapshot(market_snapshot);
  }
}

void wmdf::DataFeed::ProcessDCEMarketData(MdfRecord* record) 
{
  FuturesMarketSnapshot* marketDatas = (FuturesMarketSnapshot*)record->body;
  for(int i=0;i<record->record_count;i++)
  {
    FuturesMarketSnapshot* market_snapshot = &marketDatas[i];
    FeedEnvironment::Instance()->data_control()->MergeDCEMarketSnapshot(market_snapshot);
  }
}

void wmdf::DataFeed::ProcessSHFEMarketData(MdfRecord* record) 
{
  FuturesMarketSnapshot* marketDatas = (FuturesMarketSnapshot*)record->body;
  for(int i=0;i<record->record_count;i++)
  {
    FuturesMarketSnapshot* market_snapshot = &marketDatas[i];
    FeedEnvironment::Instance()->data_control()->MergeSHFEMarketSnapshot(market_snapshot);
  }
}

void wmdf::DataFeed::ProcessCZCEMarketData(MdfRecord* record) 
{
  FuturesMarketSnapshot* marketDatas = (FuturesMarketSnapshot*)record->body;
  for(int i=0;i<record->record_count;i++)
  {
    FuturesMarketSnapshot* market_snapshot = &marketDatas[i];
    FeedEnvironment::Instance()->data_control()->MergeCZCEMarketSnapshot(market_snapshot);
  }
}

bool wmdf::DataFeed::IsProductExistedInIdMap( int16_t product_id )
{
  std::map<int16_t, uint32_t>::iterator iter = packet_id_map_.find(product_id);
  if (iter != packet_id_map_.end())
  {
    return true;
  }
  return false;
}


