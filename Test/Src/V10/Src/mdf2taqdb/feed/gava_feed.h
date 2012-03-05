#ifndef GAVA_FEED_H
#define GAVA_FEED_H
#include <map>
#include "../../include/wmdf_api.h"
#include "../include/lclientapi.h"
#include "market_snapshot_container.h"

#include <boost/thread.hpp>

namespace wmdf
{
  class FeedInfo;
  class GavaFeed
  {
  public:
    GavaFeed(FeedInfo* feed_info);
    virtual ~GavaFeed(void);
    void Start();
    void Process(MdfRecord* record);

  private:
    void ProcessSHSZL1MarketData(MdfRecord* record);
		void ProcessSHSZL1Transaction(MdfRecord *record,int32_t &trade_index);

    void ProcessSSEL2MarketData(MdfRecord* record);
    void ProcessSSEL2Transaction(MdfRecord* record);
    
		void ProcessSZSEL2MarketData(MdfRecord* record);
    void ProcessSZSEL2Transaction(MdfRecord* record);
    
		void ProcessIndexSnapshot(MdfRecord* record,std::string table);
    void ProcessCFFEL1MarketData(MdfRecord* record);
    void ProcessDCEMarketData(MdfRecord* record);
    void ProcessSHFEMarketData(MdfRecord* record);
    void ProcessCZCEMarketData(MdfRecord* record);
    void ProcessCFFEL2MarketData(MdfRecord* record);
    void ProcessL2OrderQueues(MdfRecord* record);
    void ProcessSZSEL2Order(MdfRecord* record);
    void ProcessMarketDateSignal(MdfRecord* record);

		void ProcessFutureL1TransactionData(MdfRecord *record, int32_t &trade_index,std::string table);
		void ProcessCFFEL2TransactionData(MdfRecord *record, int32_t &trade_index, std::string table);

  private:
    void SleepTime( int millsecond );
    bool IsProductExistedInIdMap(int16_t product_id);
    void ExecGavaScript(int16_t product_id,const char* func_script,GValue table_name,GValue data);
    void ClearCache();
    void GuardGavaConnections();
    int32_t GetGavaHandle(int16_t product_id )
    {
      std::map<int16_t, int32_t>::iterator iter = gava_handles_.find(product_id);
      if (iter != gava_handles_.end())
      {
        return iter->second;
      }
      return WMDF_CONSTS_32BIT_NIL_VALUE;
    }

  private:

    MarketSnapshotContainer* market_data_container_; 
		int32_t sse_trade_index_;
		int32_t szse_trade_index_;
		int32_t cffel1_trade_index_;
		int32_t dcel1_trade_index_;
		int32_t shfel1_trade_index_;
		int32_t czcel1_trade_index_;
		int32_t cffel2_trade_index_;
    //用来校验单个产品的packetid是否连续。
    std::map<int16_t,uint32_t> packet_id_map_;
    std::map<int16_t,int32_t> gava_handles_;
    FeedInfo* feed_info_;

    //date_t next_date_;
    //int32_t clear_cache_time_;
    boost::thread* work_thread_;
    volatile bool isworking_;
  };



}

#endif
