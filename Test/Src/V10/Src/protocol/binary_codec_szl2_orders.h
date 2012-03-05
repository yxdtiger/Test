#ifndef BINARY_CODEC_SZL2_ORDERS_H
#define BINARY_CODEC_SZL2_ORDERS_H

#include "binary_message_codec.h"
namespace wmdf
{
  class BinaryCodecSZL2Orders:public BinaryMessageCodec
  {
  public:
    BinaryCodecSZL2Orders(void);
    virtual ~BinaryCodecSZL2Orders(void);

    int EncodeData(void* tr,int count);
    virtual int32_t SizeOfRecords(void* items,int count);
    void* DecodeData(uint8_t* bytes,int length,int& count);
  };
}

#endif
