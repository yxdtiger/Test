#ifndef BINARY_CODEC_LOGOUT_H
#define BINARY_CODEC_LOGOUT_H
#include "binary_message_codec.h"

namespace wmdf
{
  class BinaryCodecLogout:public BinaryMessageCodec
  {
  public:
    BinaryCodecLogout(void);
    virtual ~BinaryCodecLogout(void);

  virtual int32_t SizeOfRecords(void* items,int count);
	int EncodeData(void* items,int count);
	void* DecodeData(uint8_t* bytes,int length,int& count);
  };
}

#endif
