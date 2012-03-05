#ifndef BINARY_CODEC_MONITOR_EVENT_H
#define BINARY_CODEC_MONITOR_EVENT_H
#include "binary_message_codec.h"

namespace wmdf
{
  class BinaryCodecMonitorEvent:public BinaryMessageCodec
  {
  public:
    BinaryCodecMonitorEvent(void);
    virtual ~BinaryCodecMonitorEvent(void);

    virtual int EncodeData(void* tr,int count);
    virtual int32_t SizeOfRecords(void* items,int count);
    virtual void* DecodeData(uint8_t* bytes,int length,int& count);
  };
}

#endif
