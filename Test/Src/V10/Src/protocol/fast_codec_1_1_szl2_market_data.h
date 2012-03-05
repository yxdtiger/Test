#ifndef FAST_CODEC_1_1_SZL2_MARKET_DATA_H
#define FAST_CODEC_1_1_SZL2_MARKET_DATA_H

#include "fast_codec.h"
#include "fast_message_codec.h"

namespace wmdf{

#define SZL2_MARKETDATA_1_1_PMAP_BITS 56

  class FastCodec_1_1_SZL2MarketData:public FastMessageCodec
  {
  public:
    FastCodec_1_1_SZL2MarketData(void);
    virtual ~FastCodec_1_1_SZL2MarketData(void);

    int EncodeData(void* tr,int count);
    virtual int32_t SizeOfRecords(void* items,int count);
    void* DecodeData(uint8_t* bytes,int length,int& count);
  protected:
    virtual void ResetValueDict();
  };
}
#endif
