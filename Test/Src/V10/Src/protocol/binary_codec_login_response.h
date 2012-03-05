#ifndef BINARY_CODEC_LOGIN_RESPONSE_H
#define BINARY_CODEC_LOGIN_RESPONSE_H

#include "binary_message_codec.h"
namespace wmdf
{
  class BinaryCodecLoginResponse:public BinaryMessageCodec
  {
  public:
    BinaryCodecLoginResponse(void);
    virtual ~BinaryCodecLoginResponse(void);

    virtual int32_t SizeOfRecords(void* items,int count);
    int EncodeData(void* items,int count);
    void* DecodeData(uint8_t* bytes,int length,int& count);
  };
}

#endif
