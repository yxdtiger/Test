#ifndef MARKET_SNAPSHOT_CONTAINER_H
#define MARKET_SNAPSHOT_CONTAINER_H

#include <map>
#include <string>
#include <vector>
#include <boost/thread/mutex.hpp>
#include "../../include/wmdf_structs.h"

namespace wmdf
{
  class MdfMessage;

  class MarketSnapshotContainer
  {
  public:
    MarketSnapshotContainer();
    virtual ~MarketSnapshotContainer();
		void MergeMarketSnapshot(L2MarketSnapshot *marketsnapshot);
		L2MarketSnapshot* get_marketsnapshot(const std::string& windcode);
		int32_t marketsnapshot_count(){return num_of_marketsnapshot_;}
    std::vector<MdfMessage*>* GetMdfMessage();
	private:
		void UpdateValue(L2MarketSnapshot *des, L2MarketSnapshot *src);
    MdfMessage* CreateMdfMessage(int32_t record_count);
	private:
		std::map<std::string,L2MarketSnapshot*> market_snapshots_;
		uint32_t num_of_marketsnapshot_;
    boost::mutex mutex_;
  };
}

#endif
