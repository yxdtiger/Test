#ifndef FAST_CODEC_SHL2_ORDER_QUEUE_H
#define FAST_CODEC_SHL2_ORDER_QUEUE_H

#include "fast_codec.h"
#include "fast_message_codec.h"
#include "../include/wmdf_structs.h"

namespace wmdf{

	#define SHL2_ORDER_QUEUE_PMAP_BITS 7
  #define SHL2_ORDER_QUEUE_PRICELEVEL_PMAP_BITS 7
  #define SHL2_ORDER_QUEUE_ORDER_PMAP_BITS 7

class FastCodecSHL2OrderQueue:public FastMessageCodec
{
public:
	FastCodecSHL2OrderQueue(void);
	virtual ~FastCodecSHL2OrderQueue(void);

	int EncodeData(void* tr,int count);
  virtual int32_t SizeOfRecords(void* items,int count);
	void* DecodeData(uint8_t* bytes,int length,int& count);

protected:
  virtual void ResetValueDict();
private:
  void TolerateOrderQueue(L2OrderQueue* orderQueue);

	Fast_PMap_T* priceLevelPMap;
	Fast_PMap_T* orderPMap;
	TValue* priceLevelDefaultValue;
	TValue* orderDefaultValue;
};
}
#endif
