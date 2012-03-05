#ifndef FAST_MESSAGE_CODEC_H
#define FAST_MESSAGE_CODEC_H
#include "message_codec.h"
#include "fast_codec.h"

namespace wmdf{

  class FastMessageCodec:public MessageCodec
{
public:
	FastMessageCodec();
	virtual ~FastMessageCodec(void);

  virtual void DequeueBuffer(uint8_t* buffer);
  virtual int32_t SizeOfRecords(void* items,int count)=0;
  virtual int EncodeData(void* tr,int length) = 0;
  virtual void* DecodeData(uint8_t* bytes,int length,int& count) = 0;
protected:
  virtual void ResetValueDict() = 0;

	Fast_PMap_T* mainPMap;
	TValue* main_DefaultValue;
	FastCodec* fastCodec;
	void Reset_PMap ()
	{
		memset (mainPMap->bits, 0, mainPMap->size);
		mainPMap->max_pos = 0;
	}
};

}
#endif
