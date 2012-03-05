#ifndef BINARY_CODEC_CFFEL2_1_1_MARKET_DATA_H
#define BINARY_CODEC_CFFEL2_1_1_MARKET_DATA_H

#include "binary_message_codec.h"

namespace wmdf
{
  class BinaryCodecCFFEL2_1_1_MarketData:public BinaryMessageCodec
  {
  public:
    BinaryCodecCFFEL2_1_1_MarketData(void);
    virtual ~BinaryCodecCFFEL2_1_1_MarketData(void);

    int EncodeData(void* tr,int count);
    virtual int32_t SizeOfRecords(void* items,int count);
    void* DecodeData(uint8_t* bytes,int length,int& count);
  };
}

#endif
