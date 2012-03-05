#ifndef FAST_CODEC_SZL2_MARKET_DATA_H
#define FAST_CODEC_SZL2_MARKET_DATA_H

#include "fast_codec.h"
#include "fast_message_codec.h"

namespace wmdf{

#define SZL2_MARKETDATA_PMAP_BITS 56

  class FastCodecSZL2MarketData:public FastMessageCodec
  {
  public:
    FastCodecSZL2MarketData(void);
    virtual ~FastCodecSZL2MarketData(void);

    int EncodeData(void* tr,int count);
    virtual int32_t SizeOfRecords(void* items,int count);
    void* DecodeData(uint8_t* bytes,int length,int& count);
  protected:
    virtual void ResetValueDict();
  };
}
#endif
