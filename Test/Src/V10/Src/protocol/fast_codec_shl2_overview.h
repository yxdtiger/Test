#ifndef FAST_CODEC_SSEL2_OVERVIEW_H
#define FAST_CODEC_SSEL2_OVERVIEW_H
#include "fast_codec.h"

#include "fast_message_codec.h"

namespace wmdf{
#define SSEL2_OVERVIEW_PMAP_BITS 7
class FastCodecSHL2Overview:public FastMessageCodec
{
  public:
	  FastCodecSHL2Overview(void);
	  virtual ~FastCodecSHL2Overview(void);

	  int EncodeData(void* tr,int count);
    virtual int32_t SizeOfRecords(void* items,int count);
	  void* DecodeData(uint8_t* bytes,int length,int& count);
protected:
  virtual void ResetValueDict();
};
}
#endif