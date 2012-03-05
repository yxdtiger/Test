#ifndef MONITOR_HEADER_CODEC_H
#define MONITOR_HEADER_CODEC_H
#include "header_codec.h"

namespace wmdf
{
  class MonitorHeaderCodec:public HeaderCodec
  {
  public:
    MonitorHeaderCodec(void);
    virtual ~MonitorHeaderCodec(void);

    int EncodeData();
    virtual int32_t SizeOfRecords();
    void* DecodeData(uint8_t* bytes,int& length);
  };
}

#endif
