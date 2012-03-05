#ifndef RAWDATA_ADAPTER_H
#define RAWDATA_ADAPTER_H

#include "iproduct_adapter.h"
#include "Decoder_SSE_L2.h"
#include "../../session/feed_client.h"
#include <iostream>

namespace wmdf
{
  class MessagePublisher;
	class FHDataManager;
	class MdfMessage;


	class RawDataAdapter : public IProductAdapter
  {
  public:
    RawDataAdapter(ProductSource* product_source);
    virtual ~RawDataAdapter();
		virtual void Start();
		virtual void Stop();
    virtual bool is_connected();
		virtual void OnRecvMsg(Message* msg);
		virtual void OnCloseCallback(EventMsg* args);
		virtual void OnEventCallback(EventMsg* args);
		void PublishMdfMessage(MdfMessage* mdf_message);
	protected:
		virtual void ParseMessage( uint8_t* pRawData,int32_t RawDataLength)=0;
		virtual void AnalyseDecodedData()=0;
		virtual void ReleaseFHData()=0;
		virtual void LaunchMarketSnapshotL2()=0;
		virtual void LaunchL2Transactions()=0;
		virtual void LaunchOrderQueueL2()=0;
		virtual void LaunchMarketOverView()=0;
		virtual void LaunchVirtualAuctionPrice()=0;

		FHDataManager* FH_data_manager_;
	private:
		bool is_working_;
    FeedClient* data_receiver_;
  };
}

#endif
