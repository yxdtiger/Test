#ifndef FHDATA_MANAGER_H
#define FHDATA_MANAGER_H

#include "Decoder_SSE_L2.h"
#include "../../include/wmdf_structs.h"



namespace wmdf
{
	
  class FHDataManager
  {
  public:
    FHDataManager();
    virtual ~FHDataManager();
		
		void TransferToStruct();
		void reset();

		vecFeedHandlerData** FHDataContainer(){return &pvecFHdata_;}
		uint32_t orderqueue_count(){return num_of_orderqueue_;}
		uint32_t transaction_count(){return num_of_transaction_;}
		uint32_t marketsnapshot_count(){return num_of_marketsnapshot_;}
		uint32_t marketoverviews_count(){return num_of_marketoverview_;}
		uint32_t virtualauctionprice_count(){return num_of_virtualauctionprice_;}

		L2OrderQueue* Get_OrderQueue(){return orderqueues_;}
		L2Transaction* Get_Transaction(){return transactions_;}
		L2MarketSnapshot* Get_MarketSnapshot(){return marketsnapshots_;}
		MarketOverView* Get_MarketOverView(){return marketoverviews_;}
		VirtualAuctionPrice* Get_VirtualAuctionPrice(){return virtualauctionprices_;}

	
	private:
		vecFeedHandlerData* pvecFHdata_;
		void Calculate();
		void AllocateMemory();
		void Transfer();
		void TransferOrderQueueField(uint32_t pos_of_orderqueues, ptrWnField pField);
		void TransferTransactionField(uint32_t pos_of_transactions, ptrWnField pField);
		void TransferMarketSnapshotField(uint32_t pos_of_marketsnapshot, ptrWnField pField);
		void TransferMarketOverViewField(uint32_t pos_of_marketoverview, ptrWnField pField);
		void TransferVirtualAuctionPriceField(uint32_t pos_of_virtualauctionprice, ptrWnField pField);
		

		L2OrderQueue* orderqueues_;
		L2Transaction* transactions_;
		L2MarketSnapshot* marketsnapshots_;
		MarketOverView* marketoverviews_;
		VirtualAuctionPrice* virtualauctionprices_;

		uint32_t num_of_orderqueue_;
		uint32_t num_of_transaction_;
		uint32_t num_of_marketsnapshot_;
		uint32_t num_of_marketoverview_;
		uint32_t num_of_virtualauctionprice_;
		//uint32_t num_of_AH_marketdata_;
		//uint32_t num_of_level1data_;
  };
}

#endif
