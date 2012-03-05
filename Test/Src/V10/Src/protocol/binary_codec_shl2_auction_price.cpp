#include <assert.h>
#include "../include/wmdf_structs.h"
#include "binary_codec_shl2_auction_price.h"


using namespace wmdf;

BinaryCodecSHL2AuctionPrice::BinaryCodecSHL2AuctionPrice(void)
{

}

BinaryCodecSHL2AuctionPrice::~BinaryCodecSHL2AuctionPrice(void)
{

}


int BinaryCodecSHL2AuctionPrice::EncodeData(void* trans,int count)
{
	binaryCodec->InitEncoder();
	binaryCodec->EncodeInt32(count);
	VirtualAuctionPrice* item = (VirtualAuctionPrice* )trans;
	int length = sizeof(VirtualAuctionPrice) * count;
	binaryCodec->EncodeBytes((uint8_t*)item,length);	
	return binaryCodec->buffer.Get_Buffer_Used();
}

void* BinaryCodecSHL2AuctionPrice::DecodeData(uint8_t* bytes,int length,int& count)
{	
	binaryCodec->InitDecoder(bytes,length);
	count = binaryCodec->DecodeInt32();	
	VirtualAuctionPrice* itemArray=new VirtualAuctionPrice[count];
	int tLength = sizeof(VirtualAuctionPrice)*count;
	assert((tLength+sizeof(count))==length);
	binaryCodec->DecodeBytes((uint8_t*)itemArray,tLength);	
	return itemArray;
}




int32_t wmdf::BinaryCodecSHL2AuctionPrice::SizeOfRecords( void* items,int count )
{
   return count*sizeof(VirtualAuctionPrice);
}
