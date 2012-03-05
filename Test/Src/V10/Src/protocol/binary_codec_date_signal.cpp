#include "binary_codec_date_signal.h"
#include <assert.h>
#include "../include/wmdf_structs.h"

wmdf::BinaryCodecDateSignal::BinaryCodecDateSignal(void)
{
}

wmdf::BinaryCodecDateSignal::~BinaryCodecDateSignal(void)
{
}


int32_t wmdf::BinaryCodecDateSignal::SizeOfRecords( void* items,int count )
{
  return sizeof(MarketDateSignal) * count;
}


int wmdf::BinaryCodecDateSignal::EncodeData(void* items,int count)
{	
  binaryCodec->InitEncoder();
  binaryCodec->EncodeInt32(count);
  MarketDateSignal* item = (MarketDateSignal* )items;
  int length = sizeof(MarketDateSignal) * count;
  binaryCodec->EncodeBytes((uint8_t*)item,length);	
  return binaryCodec->buffer.Get_Buffer_Used();
}

void* wmdf::BinaryCodecDateSignal::DecodeData(uint8_t* bytes,int length,int& count)
{
  binaryCodec->InitDecoder(bytes,length);
  count = binaryCodec->DecodeInt32();	
  MarketDateSignal* itemArray=new MarketDateSignal[count];
  int tLength = sizeof(MarketDateSignal)*count;
  assert((tLength+sizeof(count))==length);
  binaryCodec->DecodeBytes((uint8_t*)itemArray,tLength);	
  return itemArray;
}
