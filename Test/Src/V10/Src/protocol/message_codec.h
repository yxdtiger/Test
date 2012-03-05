#ifndef MESSAGE_CODEC_H
#define MESSAGE_CODEC_H
#include "../include/wmdf_api.h"


namespace wmdf{

#define DEBUG 1
#define DECODE_ARRAY_CAPICITY 20

class MessageCodec
{
public:
  MessageCodec(void)
  {

  }
  virtual ~MessageCodec(void)
  {

  }

  virtual int32_t SizeOfRecords(void* items,int count)=0;
  virtual int EncodeData(void* items,int count) = 0;
  virtual void DequeueBuffer(uint8_t* buffer) = 0;
  virtual void* DecodeData(uint8_t* bytes,int length,int& count) = 0;
};
}
#endif
