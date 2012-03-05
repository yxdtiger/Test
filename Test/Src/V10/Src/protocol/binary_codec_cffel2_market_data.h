#ifndef BINARY_CODEC_CFFEL2_MARKET_DATA_H
#define BINARY_CODEC_CFFEL2_MARKET_DATA_H

#include "binary_message_codec.h"

namespace wmdf
{
  class BinaryCodecCFFEL2MarketData:public BinaryMessageCodec
  {
  public:
    BinaryCodecCFFEL2MarketData(void);
    virtual ~BinaryCodecCFFEL2MarketData(void);

    int EncodeData(void* tr,int count);
    virtual int32_t SizeOfRecords(void* items,int count);
    void* DecodeData(uint8_t* bytes,int length,int& count);
  };
}

#endif
