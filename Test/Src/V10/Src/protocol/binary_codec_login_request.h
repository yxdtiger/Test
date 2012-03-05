#ifndef BINARY_CODEC_LOGIN_REQUEST_H
#define BINARY_CODEC_LOGIN_REQUEST_H

#include "binary_message_codec.h"
namespace wmdf
{
  class BinaryCodecLoginRequest:public BinaryMessageCodec
  {
  public:
    BinaryCodecLoginRequest(void);
    virtual ~BinaryCodecLoginRequest(void);

    int EncodeData(void* items,int count);
    virtual int32_t SizeOfRecords(void* items,int count);
    void* DecodeData(uint8_t* bytes,int length,int& count);
  };
}

#endif
