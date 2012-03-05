#ifndef FAST_CODEC_INDEX_MARKET_DATA_H
#define FAST_CODEC_INDEX_MARKET_DATA_H

#include "fast_codec.h"
#include "fast_message_codec.h"

namespace wmdf{

#define INDEX_MARKETDATA_PMAP_BITS 14

class FastCodecIndexMarketData:public FastMessageCodec
{
public:
  FastCodecIndexMarketData(void);
  virtual ~FastCodecIndexMarketData(void);

  int EncodeData(void* tr,int count);
  virtual int32_t SizeOfRecords(void* items,int count);
  void* DecodeData(uint8_t* bytes,int length,int& count);
protected:
  virtual void ResetValueDict();
};
}
#endif
