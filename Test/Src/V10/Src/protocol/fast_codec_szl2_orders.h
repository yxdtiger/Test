#ifndef FAST_CODEC_SZL2_ORDERS_H
#define FAST_CODEC_SZL2_ORDERS_H

#include "fast_message_codec.h"

namespace wmdf
{
  #define SZL2_ORDERS_PMAP_BITS 7
  class FastCodecSZL2Orders:public FastMessageCodec
  {
  public:
    FastCodecSZL2Orders(void);
    virtual ~FastCodecSZL2Orders(void);

    int EncodeData(void* tr,int length);
    virtual int32_t SizeOfRecords(void* items,int count);
    void* DecodeData(uint8_t* bytes,int length,int& count);
  protected:
    virtual void ResetValueDict();
  };
}

#endif
