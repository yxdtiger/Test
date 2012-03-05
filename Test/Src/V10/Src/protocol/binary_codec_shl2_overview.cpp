#include <assert.h>
#include "../include/wmdf_structs.h"
#include "binary_codec_shl2_overview.h"


using namespace wmdf;

BinaryCodecSHL2Overview::BinaryCodecSHL2Overview(void)
{

}

BinaryCodecSHL2Overview::~BinaryCodecSHL2Overview(void)
{

}


int BinaryCodecSHL2Overview::EncodeData(void* trans,int count)
{
	binaryCodec->InitEncoder();
	binaryCodec->EncodeInt32(count);
	MarketOverView* item = (MarketOverView* )trans;
	int length = sizeof(MarketOverView) * count;
	binaryCodec->EncodeBytes((uint8_t*)item,length);	
	return binaryCodec->buffer.Get_Buffer_Used();
}

void* BinaryCodecSHL2Overview::DecodeData(uint8_t* bytes,int length,int& count)
{	
	binaryCodec->InitDecoder(bytes,length);
	count = binaryCodec->DecodeInt32();	
	MarketOverView* itemArray=new MarketOverView[count];
	int tLength = sizeof(MarketOverView)*count;
	assert((tLength+sizeof(count))==length);
	binaryCodec->DecodeBytes((uint8_t*)itemArray,tLength);	
	return itemArray;
}




int32_t wmdf::BinaryCodecSHL2Overview::SizeOfRecords( void* items,int count )
{
   return count*sizeof(MarketOverView);
}
