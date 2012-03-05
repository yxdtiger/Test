#ifndef FHSSEL2_ADAPTER_H
#define FHSSEL2_ADAPTER_H

#include "rawdata_adapter.h"

namespace wmdf
{

	class SSEL2RawDataAdapter : public RawDataAdapter
  {
  public:
    SSEL2RawDataAdapter(ProductSource* product_source);
    virtual ~SSEL2RawDataAdapter();
	private:
		bool Init(bool bIsFilterHB);
		virtual void ParseMessage( uint8_t* pRawData,int32_t RawDataLength);
		virtual void AnalyseDecodedData();
		virtual void ReleaseFHData();
		void Close();

		virtual void LaunchMarketSnapshotL2();
		virtual void LaunchL2Transactions();
		virtual void LaunchOrderQueueL2();
		virtual void LaunchMarketOverView();
		virtual void LaunchVirtualAuctionPrice();
  };
}

#endif
