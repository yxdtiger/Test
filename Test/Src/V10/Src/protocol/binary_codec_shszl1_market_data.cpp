#include <assert.h>
#include "../include/wmdf_structs.h"
#include "binary_codec_shszl1_market_data.h"

using namespace wmdf;
BinaryCodecSHSZL1MarketData::BinaryCodecSHSZL1MarketData(void)
{
}

BinaryCodecSHSZL1MarketData::~BinaryCodecSHSZL1MarketData(void)
{
}


int BinaryCodecSHSZL1MarketData::EncodeData(void* trans,int count)
{
  binaryCodec->InitEncoder();
  binaryCodec->EncodeInt32(count);
  L1MarketSnapshot* item = (L1MarketSnapshot* )trans;
  int length = sizeof(L1MarketSnapshot) * count;
  binaryCodec->EncodeBytes((uint8_t*)item,length);	
  return binaryCodec->buffer.Get_Buffer_Used();
}

void* BinaryCodecSHSZL1MarketData::DecodeData(uint8_t* bytes,int length,int& count)
{	
  binaryCodec->InitDecoder(bytes,length);
  count = binaryCodec->DecodeInt32();	
  L1MarketSnapshot* itemArray=new L1MarketSnapshot[count];
  int tLength = sizeof(L1MarketSnapshot)*count;
  assert((tLength+sizeof(count))==length);
  binaryCodec->DecodeBytes((uint8_t*)itemArray,tLength);	
  return itemArray;
}

int32_t wmdf::BinaryCodecSHSZL1MarketData::SizeOfRecords( void* items,int count )
{
  return sizeof(L1MarketSnapshot)*count;
}
