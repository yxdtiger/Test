#ifndef BINARY_CODEC_CONFIG_RESPONSE_H
#define BINARY_CODEC_CONFIG_RESPONSE_H

#include "binary_message_codec.h"
namespace wmdf
{
  class BinaryCodecConfigResponse:public BinaryMessageCodec
  {
  public:
    BinaryCodecConfigResponse(void);
    virtual ~BinaryCodecConfigResponse(void);

    int EncodeData(void* items,int count);
    virtual int32_t SizeOfRecords(void* items,int count);
    void* DecodeData(uint8_t* bytes,int length,int& count);
  };
}

#endif
