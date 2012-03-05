#ifndef	 BINARY_CODEC_SHL2_TRANS_H
#define  BINARY_CODEC_SHL2_TRANS_H

#include "binary_message_codec.h"

namespace wmdf{

class BinaryCodecSHL2Trans:public BinaryMessageCodec
{
public:
	BinaryCodecSHL2Trans(void);
	virtual ~BinaryCodecSHL2Trans(void);

	int EncodeData(void* tr,int count);
  virtual int32_t SizeOfRecords(void* items,int count);
	void* DecodeData(uint8_t* bytes,int length,int& count);
};
}
#endif
