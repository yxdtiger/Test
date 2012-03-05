#ifndef MARKET_SNAPSHOT_CONTAINER_H
#define MARKET_SNAPSHOT_CONTAINER_H

#include <map>
#include <string>
#include <vector>
#include <boost/thread/mutex.hpp>
#include "../../include/wmdf_structs.h"

namespace wmdf
{
#define DBF_SPEC_C "C,6;"
#define DBF_SPEC_C8 "C,8;"

#define DBF_SPEC_F72 "N,7,2;"
#define DBF_SPEC_F82 "N,8,2;"
#define DBF_SPEC_F83 "N,8,3;"
#define DBF_SPEC_F93 "N,9,3;"
#define DBF_SPEC_F173 "N,9,3;"

#define DBF_SPEC_S "N,9,0;"
#define DBF_SPEC_I "N,10,0;"
#define DBF_SPEC_L "N,12,0;"

#define DBF_SPEC_T "N,13,0;"

#define MAP_SCHEMA(field_name,short_name, T)  #short_name ":" DBF_SPEC_##T

#define SET_TYPE_C(field_name) \
  dbf_set_string(dbh, field++, item->field_name);

#define SET_TYPE_F(field_name) \
  dbf_set_num(dbh, field++, item->field_name);

#define SET_TYPE_F72(field_name) \
  SET_TYPE_F(field_name)

#define SET_TYPE_F82(field_name) \
  SET_TYPE_F(field_name)

#define SET_TYPE_F83(field_name) \
  SET_TYPE_F(field_name)

#define SET_TYPE_F93(field_name) \
  SET_TYPE_F(field_name)

#define SET_TYPE_F173(field_name) \
  SET_TYPE_F(field_name)

#define SET_TYPE_S(field_name) \
  dbf_set_int(dbh, field++, item->field_name);

#define SET_TYPE_I(field_name) \
  dbf_set_int(dbh, field++, item->field_name);

#define SET_TYPE_L(field_name) \
  dbf_set_long(dbh, field++, item->field_name);

#define SET_TYPE_T(field_name)\
  dbf_set_val(dbh,field++,wmdf::DateTime::time2hmsms(item->field_name,":").c_str());


#define MAP_SETTER(field_name,short_name, T) SET_TYPE_##T(field_name)
#define MAP_N_SETTER(field_name,short_name, T) field++;

  class MarketDataControl
  {
  public:
    MarketDataControl();
    virtual ~MarketDataControl();

    void MergeSSEL1MarketSnapshot(L1MarketSnapshot* marketsnapshot);
    void MergeSZSEL1MarketSnapshot(L1MarketSnapshot* marketsnapshot);

		void MergeSSEMarketSnapshot(L2MarketSnapshot* marketsnapshot);
    void MergeSZSEMarketSnapshot(L2MarketSnapshot* marketsnapshot);

    void MergeFTSEIndexSnapshot(IndexSnapshot* marketsnapshot);
    void MergeCFFEL1MarketSnapshot(FuturesMarketSnapshot* marketsnapshot);
    void MergeCFFEL2MarketSnapshot(CFEL2MarketSnapshot* marketsnapshot);
    void MergeSHFEMarketSnapshot(FuturesMarketSnapshot* marketsnapshot);
    void MergeDCEMarketSnapshot(FuturesMarketSnapshot* marketsnapshot);
    void MergeCZCEMarketSnapshot(FuturesMarketSnapshot* marketsnapshot);

    void OutputMarketData(int16_t product_id,std::string sse_file_path);
    void Clear(int16_t product_id);
	private:
    void OutputSSEL1MarketData(std::string file_path);
    void OutputSZSEL1MarketData(std::string file_path);
    void OutputSSEL2MarketData(std::string file_path);
    void OutputSZSEL2MarketData(std::string file_path);
    void OutputFTSEIndexMarketData(std::string file_path);
    void OutputCFFEL1MarketData(std::string file_path);
    void OutputCFFEL2MarketData(std::string file_path);
    void OutputSHFEMarketData(std::string file_path);
    void OutputDCEMarketData(std::string file_path);
    void OutputCZCEMarketData(std::string file_path);

    void UpdateMarketSnapshot(L2MarketSnapshot* des, L2MarketSnapshot* src);
    void UpdateL1MarketSnapshot(L1MarketSnapshot* des, L1MarketSnapshot* src);
    void UpdateFutureSnapshot(FuturesMarketSnapshot* des, FuturesMarketSnapshot* src);
    void UpdateCFFEL2Snapshot(CFEL2MarketSnapshot* des, CFEL2MarketSnapshot* src);
    void UpdateIndexSnapshot(IndexSnapshot* des,IndexSnapshot* src);

    void ClearSSEL1MarketData();
    void ClearSZSEL1MarketData();
    void ClearSSEL2MarketData();
    void ClearSZSEL2MarketData();
    void ClearFTSEMarketData();
    void ClearCFFEL1MarketData();
    void ClearCFFEL2MarketData();
    void ClearSHFEMarketData();
    void ClearDCEMarketData();
    void ClearCZCEMarketData();
	private:
    std::map<std::string,L1MarketSnapshot*> ssel1_market_datas_;
    std::map<std::string,L1MarketSnapshot*> szsel1_market_datas_;
		std::map<std::string,L2MarketSnapshot*> ssel2_market_datas_;
    std::map<std::string,L2MarketSnapshot*> szsel2_market_datas_;
    std::map<std::string,IndexSnapshot*> ftse_market_datas_;
    std::map<std::string,FuturesMarketSnapshot*> cffel1_market_datas_;
    std::map<std::string,CFEL2MarketSnapshot*> cffel2_market_datas_;
    std::map<std::string,FuturesMarketSnapshot*> shfe_market_datas_;
    std::map<std::string,FuturesMarketSnapshot*> dce_market_datas_;
    std::map<std::string,FuturesMarketSnapshot*> czce_market_datas_;

    boost::mutex ssel1_mkt_mutex_;
    boost::mutex szsel1_mkt_mutex_;
    boost::mutex ssel2_mkt_mutex_;
    boost::mutex szsel2_mkt_mutex_;
    boost::mutex ftse_mkt_mutex_;
    boost::mutex cffel1_mkt_mutex_;
    boost::mutex cffel2_mkt_mutex_;
    boost::mutex shfe_mkt_mutex_;
    boost::mutex dce_mkt_mutex_;
    boost::mutex czce_mkt_mutex_;
  };
}

#endif
