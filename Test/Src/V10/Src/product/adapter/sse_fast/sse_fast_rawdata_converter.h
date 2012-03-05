#ifndef SSE_FAST_RAWDATA_CONVERTER_H
#define SSE_FAST_RAWDATA_CONVERTER_H

#include "sse_marketdata_3202_merger.h"
#include "../../../environment/utils.h"

namespace wmdf
{
	class SSEFastRawDataConverter
	{
	public:
		SSEFastRawDataConverter();
		~SSEFastRawDataConverter();
    void Reset();
		void convertMarketsnapshot(WindFastMessage*,uint32_t& pack_id);
		void convertOrderqueue(WindFastMessage*,uint32_t& pack_id);
		void convertTransaction(WindFastMessage*,uint32_t& pack_id);
		void convertVirtualauctionprice(WindFastMessage*,uint32_t& pack_id);
		void convertIndexData(WindFastMessage*,uint32_t& pack_id);
		void convertMarketdata3202(WindFastMessage*, uint32_t& pack_id);
		void convertOrderqueue3202(WindFastMessage*, uint32_t &pack_id);
  private:
    int32_t GetOrderQueuePriceLevelCount(PriceLevel_3202* price_levels,int32_t count);
    void ConvertOrderQueue3202(PriceLevel_3202* src,int32_t src_count,L2OrderQueue& dst);
	private:
		static const char* sse_suffix;
    SSE_MarketData_3202_Merger* market_data_3202_merger_;

	};
}

#endif
