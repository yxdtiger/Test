#ifndef BINARY_CODEC_DATE_SIGNAL_H
#define BINARY_CODEC_DATE_SIGNAL_H
#include "binary_message_codec.h"

namespace wmdf
{
  class BinaryCodecDateSignal:public BinaryMessageCodec
  {
  public:
    BinaryCodecDateSignal(void);
    virtual ~BinaryCodecDateSignal(void);

    virtual int32_t SizeOfRecords(void* items,int count);
    int EncodeData(void* items,int count);
    void* DecodeData(uint8_t* bytes,int length,int& count);
  };
}

#endif
