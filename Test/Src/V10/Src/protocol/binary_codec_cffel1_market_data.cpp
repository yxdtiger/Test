#include <assert.h>
#include "../include/wmdf_structs.h"
#include "binary_codec_cffel1_market_data.h"

using namespace wmdf;
wmdf::BinaryCodecCFFEL1MarketData::BinaryCodecCFFEL1MarketData(void)
{

}

wmdf::BinaryCodecCFFEL1MarketData::~BinaryCodecCFFEL1MarketData(void)
{

}


int wmdf::BinaryCodecCFFEL1MarketData::EncodeData(void* trans,int count)
{
  binaryCodec->InitEncoder();
  binaryCodec->EncodeInt32(count);
  FuturesMarketSnapshot* item = (FuturesMarketSnapshot* )trans;
  int length = sizeof(FuturesMarketSnapshot) * count;
  binaryCodec->EncodeBytes((uint8_t*)item,length);	
  return binaryCodec->buffer.Get_Buffer_Used();
}

void* wmdf::BinaryCodecCFFEL1MarketData::DecodeData(uint8_t* bytes,int length,int& count)
{	
  binaryCodec->InitDecoder(bytes,length);
  count = binaryCodec->DecodeInt32();	
  FuturesMarketSnapshot* itemArray=new FuturesMarketSnapshot[count];
  int tLength = sizeof(FuturesMarketSnapshot)*count;
  assert((tLength+sizeof(count))==length);
  binaryCodec->DecodeBytes((uint8_t*)itemArray,tLength);	
  return itemArray;
}

int32_t wmdf::BinaryCodecCFFEL1MarketData::SizeOfRecords( void* items,int count )
{
  return sizeof(FuturesMarketSnapshot)*count;
}
