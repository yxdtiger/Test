#ifndef BINARY_CODEC_SHL2_ORDER_QUEUE_H
#define BINARY_CODEC_SHL2_ORDER_QUEUE_H

#include "binary_message_codec.h"

namespace wmdf{
#define SSEL2ORDERQUEUELENGTH sizeof(SSEL2OrderQueue)-sizeof(void*);

class BinaryCodecSHL2OrderQueue:public BinaryMessageCodec
{
public:
	BinaryCodecSHL2OrderQueue(void);
	virtual ~BinaryCodecSHL2OrderQueue(void);

	int EncodeData(void* tr,int count);
  virtual int32_t SizeOfRecords(void* items,int count);
	void* DecodeData(uint8_t* bytes,int length,int& count);

};
}
#endif
