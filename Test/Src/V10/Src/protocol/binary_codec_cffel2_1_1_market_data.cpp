#include "binary_codec_cffel2_1_1_market_data.h"
#include <assert.h>
#include "../include/wmdf_structs.h"

using namespace wmdf;
wmdf::BinaryCodecCFFEL2_1_1_MarketData::BinaryCodecCFFEL2_1_1_MarketData(void)
{
  single_length = sizeof(CFEL2MarketSnapshot);
}

wmdf::BinaryCodecCFFEL2_1_1_MarketData::~BinaryCodecCFFEL2_1_1_MarketData(void)
{

}

int wmdf::BinaryCodecCFFEL2_1_1_MarketData::EncodeData(void* trans,int count)
{
  binaryCodec->InitEncoder();
  binaryCodec->EncodeInt32(count);
  CFEL2MarketSnapshot* item = (CFEL2MarketSnapshot* )trans;
  int length = single_length * count;
  binaryCodec->EncodeBytes((uint8_t*)item,length);	
  return binaryCodec->buffer.Get_Buffer_Used();
}

void* wmdf::BinaryCodecCFFEL2_1_1_MarketData::DecodeData(uint8_t* bytes,int length,int& count)
{	
  binaryCodec->InitDecoder(bytes,length);
  count = binaryCodec->DecodeInt32();	
  CFEL2MarketSnapshot* itemArray=new CFEL2MarketSnapshot[count];
  int tLength = single_length * count;
  assert((tLength+sizeof(count))==length);
  binaryCodec->DecodeBytes((uint8_t*)itemArray,tLength);	
  return itemArray;
}

int32_t wmdf::BinaryCodecCFFEL2_1_1_MarketData::SizeOfRecords( void* items,int count )
{
  return single_length * count;
}
