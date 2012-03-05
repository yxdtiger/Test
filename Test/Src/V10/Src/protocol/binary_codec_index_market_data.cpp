#include <assert.h>
#include "../include/wmdf_structs.h"
#include "binary_codec_index_market_data.h"

using namespace wmdf;
wmdf::BinaryCodecIndexMarketData::BinaryCodecIndexMarketData(void)
{

}

wmdf::BinaryCodecIndexMarketData::~BinaryCodecIndexMarketData(void)
{

}


int wmdf::BinaryCodecIndexMarketData::EncodeData(void* trans,int count)
{
  binaryCodec->InitEncoder();
  binaryCodec->EncodeInt32(count);
  IndexSnapshot* item = (IndexSnapshot* )trans;
  int length = sizeof(IndexSnapshot) * count;
  binaryCodec->EncodeBytes((uint8_t*)item,length);	
  return binaryCodec->buffer.Get_Buffer_Used();
}

void* wmdf::BinaryCodecIndexMarketData::DecodeData(uint8_t* bytes,int length,int& count)
{	
  binaryCodec->InitDecoder(bytes,length);
  count = binaryCodec->DecodeInt32();	
  IndexSnapshot* itemArray=new IndexSnapshot[count];
  int tLength = sizeof(IndexSnapshot)*count;
  assert((tLength+sizeof(count))==length);
  binaryCodec->DecodeBytes((uint8_t*)itemArray,tLength);	
  return itemArray;
}

int32_t wmdf::BinaryCodecIndexMarketData::SizeOfRecords( void* items,int count )
{
  return sizeof(IndexSnapshot)*count;
}
