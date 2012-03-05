#ifndef FAST_CODEC_CFFEL2_1_1_MARKET_DATA_H
#define FAST_CODEC_CFFEL2_1_1_MARKET_DATA_H

#include "fast_codec.h"
#include "fast_message_codec.h"

namespace wmdf{

#define CFFEL2_1_1_MARKETDATA_PMAP_BITS 21

	class FastCodecCFFEL2_1_1_MarketData:public FastMessageCodec
	{
	public:
		FastCodecCFFEL2_1_1_MarketData(void);
		virtual ~FastCodecCFFEL2_1_1_MarketData(void);

		int EncodeData(void* tr,int count);
    virtual int32_t SizeOfRecords(void* items,int count);
		void* DecodeData(uint8_t* bytes,int length,int& count);
  protected:
    virtual void ResetValueDict();
	};
}
#endif
