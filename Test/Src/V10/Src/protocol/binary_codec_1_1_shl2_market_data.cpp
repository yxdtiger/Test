#include <iostream>
#include <assert.h>
#include "../include/wmdf_structs.h"
#include "binary_codec_1_1_shl2_market_data.h"


using namespace wmdf;

BinaryCodec_1_1_SHL2MarketData::BinaryCodec_1_1_SHL2MarketData()
{
  single_length = sizeof(L2MarketSnapshot);
}

BinaryCodec_1_1_SHL2MarketData::~BinaryCodec_1_1_SHL2MarketData()
{
}


int BinaryCodec_1_1_SHL2MarketData::EncodeData(void* items,int count)
{	
	binaryCodec->InitEncoder();
	binaryCodec->EncodeInt32(count);
	L2MarketSnapshot* item = (L2MarketSnapshot* )items;
	int length = single_length * count;
	binaryCodec->EncodeBytes((uint8_t*)item,length);	
	return binaryCodec->buffer.Get_Buffer_Used();
}

void* BinaryCodec_1_1_SHL2MarketData::DecodeData(uint8_t* bytes,int length,int& count)
{
	binaryCodec->InitDecoder(bytes,length);
	count = binaryCodec->DecodeInt32();	
	L2MarketSnapshot* itemArray=new L2MarketSnapshot[count];
	int tLength = single_length * count;
	assert((tLength+sizeof(count))==length);
	binaryCodec->DecodeBytes((uint8_t*)itemArray,tLength);	
	return itemArray;
}




int32_t wmdf::BinaryCodec_1_1_SHL2MarketData::SizeOfRecords( void* items,int count )
{
  return single_length * count;
}
