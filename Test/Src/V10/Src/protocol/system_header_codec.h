#ifndef SYSTEM_HEADER_CODEC_H
#define SYSTEM_HEADER_CODEC_H

#include "header_codec.h"

namespace wmdf
{
  class SystemHeaderCodec:public HeaderCodec
  {
  public:
    SystemHeaderCodec(void);
    virtual ~SystemHeaderCodec(void);

    virtual int32_t SizeOfRecords();
    int EncodeData();
    void* DecodeData(uint8_t* bytes,int& length);
  };
}

#endif
