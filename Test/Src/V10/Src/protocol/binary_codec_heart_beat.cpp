#include "binary_codec_heart_beat.h"
#include <assert.h>
#include "../include/wmdf_structs.h"

wmdf::BinaryCodecHeartBeat::BinaryCodecHeartBeat(void)
{
}

wmdf::BinaryCodecHeartBeat::~BinaryCodecHeartBeat(void)
{
}


int32_t wmdf::BinaryCodecHeartBeat::SizeOfRecords( void* items,int count )
{
  return sizeof(HeartBeat) * count;
}


int wmdf::BinaryCodecHeartBeat::EncodeData(void* items,int count)
{	
  binaryCodec->InitEncoder();
  binaryCodec->EncodeInt32(count);
  HeartBeat* item = (HeartBeat* )items;
  int length = sizeof(HeartBeat) * count;
  binaryCodec->EncodeBytes((uint8_t*)item,length);	
  return binaryCodec->buffer.Get_Buffer_Used();
}

void* wmdf::BinaryCodecHeartBeat::DecodeData(uint8_t* bytes,int length,int& count)
{
  binaryCodec->InitDecoder(bytes,length);
  count = binaryCodec->DecodeInt32();	
  HeartBeat* itemArray=new HeartBeat[count];
  int tLength = sizeof(HeartBeat)*count;
  assert((tLength+sizeof(count))==length);
  binaryCodec->DecodeBytes((uint8_t*)itemArray,tLength);	
  return itemArray;
}
