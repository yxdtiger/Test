#ifndef BINARY_CODEC_TRANSFER_STAT_H
#define BINARY_CODEC_TRANSFER_STAT_H
#include "binary_message_codec.h"

namespace wmdf
{
  class BinaryCodecTransferStat:public BinaryMessageCodec
  {
  public:
    BinaryCodecTransferStat(void);
    virtual ~BinaryCodecTransferStat(void);

    virtual int EncodeData(void* tr,int count);
    virtual int32_t SizeOfRecords(void* items,int count);
    virtual void* DecodeData(uint8_t* bytes,int length,int& count);
  };
}

#endif
