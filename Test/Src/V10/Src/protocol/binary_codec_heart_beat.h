#ifndef BINARY_CODEC_HEART_BEAT_H
#define BINARY_CODEC_HEART_BEAT_H
#include "binary_message_codec.h"

namespace wmdf
{
  class BinaryCodecHeartBeat:public BinaryMessageCodec
  {
  public:
    BinaryCodecHeartBeat(void);
    virtual ~BinaryCodecHeartBeat(void);

    virtual int32_t SizeOfRecords(void* items,int count);
    int EncodeData(void* items,int count);
    void* DecodeData(uint8_t* bytes,int length,int& count);
  };
}

#endif
