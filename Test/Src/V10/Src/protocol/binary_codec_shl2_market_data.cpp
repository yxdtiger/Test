#include <iostream>
#include <assert.h>
#include "../include/wmdf_structs.h"
#include "binary_codec_shl2_market_data.h"


using namespace wmdf;

BinaryCodecSHL2MarketData::BinaryCodecSHL2MarketData()
{
  single_length = sizeof(L2MarketSnapshot)-2*sizeof(double)-sizeof(int8_t);
}

BinaryCodecSHL2MarketData::~BinaryCodecSHL2MarketData()
{
}


int BinaryCodecSHL2MarketData::EncodeData(void* items,int count)
{	
	binaryCodec->InitEncoder();
	binaryCodec->EncodeInt32(count);
	L2MarketSnapshot* item = (L2MarketSnapshot* )items;
	int total_length = single_length * count;
	binaryCodec->EncodeBytes((uint8_t*)item,total_length);	
	return binaryCodec->buffer.Get_Buffer_Used();
}

void* BinaryCodecSHL2MarketData::DecodeData(uint8_t* bytes,int length,int& count)
{
	binaryCodec->InitDecoder(bytes,length);
	count = binaryCodec->DecodeInt32();	
	L2MarketSnapshot* itemArray=new L2MarketSnapshot[count];
  int total_length = single_length * count;
	assert((total_length+sizeof(count))==length);
	binaryCodec->DecodeBytes((uint8_t*)itemArray,total_length);	
	return itemArray;
}




int32_t wmdf::BinaryCodecSHL2MarketData::SizeOfRecords( void* items,int count )
{
  return single_length * count ;
}
