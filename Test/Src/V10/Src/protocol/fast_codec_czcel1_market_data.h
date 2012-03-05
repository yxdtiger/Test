#ifndef FAST_CODEC_CZCEL1_MARKET_DATA_H
#define FAST_CODEC_CZCEL1_MARKET_DATA_H

#include "fast_message_codec.h"

namespace wmdf{

#define CZCEL1_MARKETDATA_PMAP_BITS 21

class FastCodecCZCEL1MarketData:public FastMessageCodec
{
public:
  FastCodecCZCEL1MarketData(void);
  virtual ~FastCodecCZCEL1MarketData(void);

  int EncodeData(void* tr,int length);
  virtual int32_t SizeOfRecords(void* items,int count);
  void* DecodeData(uint8_t* bytes,int length,int& count);
protected:
  virtual void ResetValueDict();
};
}
#endif