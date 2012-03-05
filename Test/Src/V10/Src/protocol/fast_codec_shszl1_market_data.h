#ifndef FAST_CODEC_SHSZL1_MARKET_DATA_H
#define FAST_CODEC_SHSZL1_MARKET_DATA_H
#include "fast_codec.h"
#include "fast_message_codec.h"

namespace wmdf{

	#define SHSZL1_MARKETDATA_PMAP_BITS 21

	class FastCodecSHSZL1MarketData:public FastMessageCodec
	{
	public:
		FastCodecSHSZL1MarketData(void);
		virtual ~FastCodecSHSZL1MarketData(void);

		int EncodeData(void* tr,int length);
    virtual int32_t SizeOfRecords(void* items,int count);
		void* DecodeData(uint8_t* bytes,int length,int& count);
  protected:
    virtual void ResetValueDict();
	};
}
#endif
