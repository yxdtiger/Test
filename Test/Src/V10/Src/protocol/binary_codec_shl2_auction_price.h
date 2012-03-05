#ifndef	 BINARY_CODEC_SHL2_AUCTION_PRICE_H
#define  BINARY_CODEC_SHL2_AUCTION_PRICE_H

#include "binary_message_codec.h"

namespace wmdf{

class BinaryCodecSHL2AuctionPrice:public BinaryMessageCodec
{
public:
	BinaryCodecSHL2AuctionPrice(void);
	virtual ~BinaryCodecSHL2AuctionPrice(void);

	int EncodeData(void* tr,int count);
  virtual int32_t SizeOfRecords(void* items,int count);
	void* DecodeData(uint8_t* bytes,int length,int& count);
};
}
#endif