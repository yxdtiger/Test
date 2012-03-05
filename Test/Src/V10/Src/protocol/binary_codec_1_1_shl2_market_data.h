#ifndef BINARY_CODEC_1_1_SHL2_MARKET_DATA_H
#define BINARY_CODEC_1_1_SHL2_MARKET_DATA_H

#include "binary_message_codec.h"

namespace wmdf{


	class BinaryCodec_1_1_SHL2MarketData:public BinaryMessageCodec
	{
	public:
		BinaryCodec_1_1_SHL2MarketData(void);
		virtual ~BinaryCodec_1_1_SHL2MarketData(void);

		int EncodeData(void* tr,int count);
    virtual int32_t SizeOfRecords(void* items,int count);
		void* DecodeData(uint8_t* bytes,int length,int& count);
	};
}
#endif
