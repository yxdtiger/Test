#ifndef STOPBIT_CODEC_SZL2_MARKET_DATA_H
#define STOPBIT_CODEC_SZL2_MARKET_DATA_H

#include "fast_codec.h"
#include "fast_message_codec.h"

namespace wmdf{

#define STOPBIT_SZL2_MARKETDATA_PMAP_BITS 21

  class StopBitCodecSZL2MarketData:public FastMessageCodec
  {
  public:
    StopBitCodecSZL2MarketData(void);
    virtual ~StopBitCodecSZL2MarketData(void);

    int EncodeData(void* tr,int count);
    virtual int32_t SizeOfRecords(void* items,int count);
    void* DecodeData(uint8_t* bytes,int length,int& count);
  protected:
    virtual void ResetValueDict(){;}
  };
}
#endif