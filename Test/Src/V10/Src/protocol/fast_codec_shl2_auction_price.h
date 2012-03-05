#ifndef FAST_CODEC_SHL2_AUCTION_PRICE_H
#define FAST_CODEC_SHL2_AUCTION_PRICE_H
#include "fast_codec.h"

#include "fast_message_codec.h"

namespace wmdf{
#define SSEL2_AuctionPrice_PMAP_BITS 7
class FastCodecSHL2AuctionPrice:public FastMessageCodec
{
  public:
	  FastCodecSHL2AuctionPrice(void);
	  virtual ~FastCodecSHL2AuctionPrice(void);

	  int EncodeData(void* tr,int count);
    virtual int32_t SizeOfRecords(void* items,int count);
	  void* DecodeData(uint8_t* bytes,int length,int& count);
protected:
  virtual void ResetValueDict();
};
}
#endif