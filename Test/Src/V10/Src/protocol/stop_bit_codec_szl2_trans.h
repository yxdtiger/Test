#ifndef STOP_BIT_CODEC_SZL2_TRANS_H
#define STOP_BIT_CODEC_SZL2_TRANS_H
#include "fast_codec.h"

#include "fast_message_codec.h"

namespace wmdf{
#define STOP_BIT_SZSEL2_TRANS_PMAP_BITS 14
class StopBitCodecSZL2Trans:public FastMessageCodec
{
  public:
	  StopBitCodecSZL2Trans(void);
	  virtual ~StopBitCodecSZL2Trans(void);

	  int EncodeData(void* tr,int count);
    virtual int32_t SizeOfRecords(void* items,int count);
	  void* DecodeData(uint8_t* bytes,int length,int& count);
protected:
  virtual void ResetValueDict(){;}
};
}
#endif