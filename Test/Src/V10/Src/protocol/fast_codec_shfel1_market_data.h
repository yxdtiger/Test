#ifndef FAST_CODEC_SHFEL1_MARKET_DATA_H
#define FAST_CODEC_SHFEL1_MARKET_DATA_H

#include "fast_message_codec.h"

namespace wmdf{

#define SHFEL1_MARKETDATA_PMAP_BITS 21

class FastCodecSHFEL1MarketData:public FastMessageCodec
{
public:
  FastCodecSHFEL1MarketData(void);
  virtual ~FastCodecSHFEL1MarketData(void);

  int EncodeData(void* tr,int count);
  virtual int32_t SizeOfRecords(void* items,int count);
  void* DecodeData(uint8_t* bytes,int length,int& count);
protected:
  virtual void ResetValueDict();
};
}
#endif