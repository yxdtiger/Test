#ifndef FAST_CODEC_SZL2_TRANS_H
#define FAST_CODEC_SZL2_TRANS_H
#include "fast_codec.h"

#include "fast_message_codec.h"

namespace wmdf{
#define SZSEL2_TRANS_PMAP_BITS 14
class FastCodecSZL2Trans:public FastMessageCodec
{
  public:
	  FastCodecSZL2Trans(void);
	  virtual ~FastCodecSZL2Trans(void);

	  int EncodeData(void* tr,int count);
    virtual int32_t SizeOfRecords(void* items,int count);
	  void* DecodeData(uint8_t* bytes,int length,int& count);
protected:
  virtual void ResetValueDict();
};
}
#endif
