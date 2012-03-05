#ifndef FAST_CODEC_DCEL1_MARKET_DATA_H
#define FAST_CODEC_DCEL1_MARKET_DATA_H

#include "fast_message_codec.h"

namespace wmdf{

#define DCEL1_MARKETDATA_PMAP_BITS 21

class FastCodecDCEL1MarketData:public FastMessageCodec
{
public:
  FastCodecDCEL1MarketData(void);
  virtual ~FastCodecDCEL1MarketData(void);

  int EncodeData(void* tr,int count);
  virtual int32_t SizeOfRecords(void* items,int count);
  void* DecodeData(uint8_t* bytes,int length,int& count);
protected:
  virtual void ResetValueDict();
};
}
#endif