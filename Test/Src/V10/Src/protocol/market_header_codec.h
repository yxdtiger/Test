#ifndef MARKET_HEADER_CODEC_H
#define MARKET_HEADER_CODEC_H
#include "header_codec.h"

namespace wmdf
{
  class MarketHeaderCodec:public HeaderCodec
  {
  public:
    MarketHeaderCodec(void);
    virtual ~MarketHeaderCodec(void);

    int EncodeData();
    virtual int32_t SizeOfRecords();
    void* DecodeData(uint8_t* bytes,int& length);
  };
}

#endif
