#ifndef SSE_STEP_RAWDATA_CONVERTER_H
#define SSE_STEP_RAWDATA_CONVERTER_H

#include <vector>
#include "../../../include/wmdf_structs.h"

namespace wmdf
{
  struct WindStepMessage;

	class SSEStepRawDataConverter
	{
	public:
		SSEStepRawDataConverter();
		~SSEStepRawDataConverter();

    virtual void ConvertData(std::vector<WindStepMessage*>& msgs);
		uint32_t indexdata_count(){return num_of_indexdata_;}
		IndexSnapshot* get_indexdata(){return indexdata_;}

    uint32_t orderqueue_count(){return num_of_orderqueue_;}
    L2OrderQueue* get_orderqueue(){return orderqueues_;}

    uint32_t transaction_count(){return num_of_transaction_;}
    L2Transaction* get_transaction(){return transactions_;}

    uint32_t marketsnapshot_count(){return num_of_marketsnapshot_;}
    L2MarketSnapshot* get_marketsnapshot(){return marketsnapshots_;}

	private:
		virtual void reset();
		virtual bool calculate();
		virtual void allocateMemory();
		virtual void convert();

		void convertMarketsnapshot(uint32_t pos_of_marketsnapshot, WindStepMessage *wsm);
		void convertOrderqueue(uint32_t pos_of_orderqueue, WindStepMessage *wsm);
		void convertTransaction(uint32_t pos_of_transaction, WindStepMessage* wsm);
		void convertVirtualauctionprice(uint32_t pos_of_virtualauctionprice, WindStepMessage* wsm);
		void convertIndexData(uint32_t pos_of_indexdata, WindStepMessage* wsm);
	private:
		std::vector<WindStepMessage*>* windstep_msgs_;

		IndexSnapshot *indexdata_;
		uint32_t num_of_indexdata_;

    uint32_t num_of_orderqueue_;
    L2OrderQueue* orderqueues_;

    uint32_t num_of_transaction_;
    L2Transaction* transactions_;

    uint32_t num_of_marketsnapshot_;
    L2MarketSnapshot* marketsnapshots_;
	};
}

#endif
