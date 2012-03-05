#ifndef MARKET_SNAPSHOT_CONTAINER_H
#define MARKET_SNAPSHOT_CONTAINER_H

#include <map>
#include <string>
#include <vector>
#include "../../include/wmdf_structs.h"
#ifdef __linux__
#include <cstring>
#endif

namespace wmdf
{

	struct FutureTransaction
	{
		time_stamp_t time;
		char wind_code[16];
		double new_price;
		int32_t delt_volume;
		int32_t delt_position;
		int32_t futureoptionstate;
	};

	struct L1Transaction
	{
		time_stamp_t time;
		char wind_code[16];
		double new_price;
		int32_t delt_volume;
		int32_t delt_amount;
	};

  class MarketSnapshotContainer
  {
		enum FutureDealType{fdtBuy=0,fdtSell=1,fdtUnknown=2};
		enum FutureOptionState
		{
			///1：双开仓；2：双平仓；3：多换手；4：空换手；
			//5：多头开仓；6：多头平仓；7：空头开仓；8 空头平仓）
			fosUnknown=0,
			fosOpenOption=1,
			fosCloseOption=2,
			fosChangeLongOption=3,
			fosChangeShortOption=4,
			fosOpenLongOption=5,
			fosCloseLongOption=6,
			fosOpenShortOption=7,
			fosCloseShortOption=8
		}	;
  public:
    MarketSnapshotContainer();
    virtual ~MarketSnapshotContainer();
		void MergeL2MarketSnapshot(L2MarketSnapshot* marketsnapshot);

		void MergeL1MarketSnapshot(L1MarketSnapshot *marketsnapshot);
		L1Transaction* GetL1Transaction(int32_t index);
		int32_t L1TransactionCount(){return l1_transactions_.size();}

    void MergeIndexSnapshot(IndexSnapshot* index_snapshot);
		FuturesMarketSnapshot* LastFutureL1MarketSnapshot(FuturesMarketSnapshot *);
		void UpdateFutureL1MarketData(FuturesMarketSnapshot *des, FuturesMarketSnapshot *src);
		CFEL2MarketSnapshot* LastCFFEL2MarketSnapshot(CFEL2MarketSnapshot *);
		void UpdateCFFEL2MarketData(CFEL2MarketSnapshot *des,CFEL2MarketSnapshot *src);

		int32_t ConvertCFFEL2Transaction(void *data, int32_t count,FutureTransaction *cffel2_transactions);
		int32_t ConvertFutureL1Transaction(void *data, int32_t count,FutureTransaction *cffel2_transactions);
    void Clear();
		void ClearL1Transaction();

	private:
		void UpdateL2MarketSnapshot(L2MarketSnapshot *des, L2MarketSnapshot *src);
		void UpdateL1MarketSnapshot(L1MarketSnapshot *des, L1MarketSnapshot *src);
		void UpdateCurrentL1Marketdata(L1MarketSnapshot *des, L1MarketSnapshot *src);
		void InsertL1Transaction(L1MarketSnapshot *des, L1MarketSnapshot *src);
		void UpdateLastL1Marketdata(L1MarketSnapshot *des,L1MarketSnapshot *src);
    void UpdateIndexSnapshot(IndexSnapshot* des,IndexSnapshot* src);
		FutureOptionState GetFutureTradeState(double, double, FutureDealType);
	private:
		std::map<std::string,L2MarketSnapshot*> l2market_snapshots_;
		std::map<std::string,L1MarketSnapshot*> l1_market_snapshots_;
		std::vector<L1Transaction*> l1_transactions_; 
    std::map<std::string,IndexSnapshot*> index_snapshots_;
		std::map<std::string,FuturesMarketSnapshot*> future_l1_snapshots_;
		std::map<std::string,CFEL2MarketSnapshot*> cffe_l2_snapshots_;
    
	
  };
}

#endif
