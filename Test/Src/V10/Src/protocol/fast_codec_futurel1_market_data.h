#ifndef FAST_CODEC_FUTUREL1_MARKET_DATA_H
#define FAST_CODEC_FUTUREL1_MARKET_DATA_H

#include "fast_codec.h"
#include "fast_message_codec.h"

namespace wmdf{

#define FUTUREL1_MARKETDATA_PMAP_BITS 21

class FastCodecFutureL1MarketData:public FastMessageCodec
{
public:
  FastCodecFutureL1MarketData(void);
  virtual ~FastCodecFutureL1MarketData(void);

  int EncodeData(void* tr,int count);
  virtual int32_t SizeOfRecords(void* items,int count);
  void* DecodeData(uint8_t* bytes,int length,int& count);
protected:
  virtual void ResetValueDict();
};
}
#endif
