#ifndef	 BINARY_CODEC_SHL2_OVERVIEW_H
#define  BINARY_CODEC_SHL2_OVERVIEW_H

#include "binary_message_codec.h"

namespace wmdf{

class BinaryCodecSHL2Overview:public BinaryMessageCodec
{
public:
	BinaryCodecSHL2Overview(void);
	virtual ~BinaryCodecSHL2Overview(void);

	int EncodeData(void* tr,int count);
  virtual int32_t SizeOfRecords(void* items,int count);
	void* DecodeData(uint8_t* bytes,int length,int& count);
};
}
#endif