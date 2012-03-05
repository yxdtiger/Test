#ifndef BINARY_CODEC_CONFIG_INFO_H
#define BINARY_CODEC_CONFIG_INFO_H

#include "binary_message_codec.h"
namespace wmdf
{
  class BinaryCodecConfigInfo:public BinaryMessageCodec
  {
  public:
    BinaryCodecConfigInfo(void);
    virtual ~BinaryCodecConfigInfo(void);

    int EncodeData(void* items,int count);
    virtual int32_t SizeOfRecords(void* items,int count);
    void* DecodeData(uint8_t* bytes,int length,int& count);
  };
}

#endif
