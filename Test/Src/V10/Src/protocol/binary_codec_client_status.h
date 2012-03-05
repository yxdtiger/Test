#ifndef BINARY_CODEC_CLIENT_STATUS_H
#define BINARY_CODEC_CLIENT_STATUS_H
#include "binary_message_codec.h"

namespace wmdf
{
  class BinaryCodecClientStatus:public BinaryMessageCodec
  {
  public:
    BinaryCodecClientStatus(void);
    virtual ~BinaryCodecClientStatus(void);

    virtual int EncodeData(void* tr,int count);
    virtual int32_t SizeOfRecords(void* items,int count);
    virtual void* DecodeData(uint8_t* bytes,int length,int& count);
  };
}

#endif
