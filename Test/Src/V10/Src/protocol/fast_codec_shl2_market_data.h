#ifndef FAST_CODEC_SHL2_MARKET_DATA_H
#define FAST_CODEC_SHL2_MARKET_DATA_H

#include "fast_codec.h"
#include "fast_message_codec.h"

namespace wmdf{

	#define SHL2_MARKETDATA_PMAP_BITS 56

	class FastCodecSHL2MarketData:public FastMessageCodec
	{
	public:
		FastCodecSHL2MarketData(void);
		virtual ~FastCodecSHL2MarketData(void);

		int EncodeData(void* tr,int count);
    virtual int32_t SizeOfRecords(void* items,int count);
		void* DecodeData(uint8_t* bytes,int length,int& count);
  protected:
    virtual void ResetValueDict();
	};
}
#endif
