#ifndef BINARY_CODEC_CFFEL1_MARKET_DATA_H
#define BINARY_CODEC_CFFEL1_MARKET_DATA_H

#include "binary_message_codec.h"

namespace wmdf
{
  class BinaryCodecCFFEL1MarketData:public BinaryMessageCodec
  {
  public:
    BinaryCodecCFFEL1MarketData(void);
    virtual ~BinaryCodecCFFEL1MarketData(void);

    int EncodeData(void* tr,int count);
    virtual int32_t SizeOfRecords(void* items,int count);
    void* DecodeData(uint8_t* bytes,int length,int& count);
  };
}

#endif
