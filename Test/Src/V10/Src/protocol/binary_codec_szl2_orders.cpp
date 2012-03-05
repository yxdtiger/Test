#include "binary_codec_szl2_orders.h"
#include <assert.h>
#include "../include/wmdf_structs.h"

using namespace wmdf;

wmdf::BinaryCodecSZL2Orders::BinaryCodecSZL2Orders(void)
{
}

wmdf::BinaryCodecSZL2Orders::~BinaryCodecSZL2Orders(void)
{
}

int32_t wmdf::BinaryCodecSZL2Orders::SizeOfRecords( void* items,int count )
{
  return sizeof(SZSEL2Order)*count;
}


int BinaryCodecSZL2Orders::EncodeData(void* trans,int count)
{
  binaryCodec->InitEncoder();
  binaryCodec->EncodeInt32(count);
  SZSEL2Order* item = (SZSEL2Order* )trans;
  int length = sizeof(SZSEL2Order) * count;
  binaryCodec->EncodeBytes((uint8_t*)item,length);	
  return binaryCodec->buffer.Get_Buffer_Used();
}

void* BinaryCodecSZL2Orders::DecodeData(uint8_t* bytes,int length,int& count)
{	
  binaryCodec->InitDecoder(bytes,length);
  count = binaryCodec->DecodeInt32();	
  SZSEL2Order* itemArray=new SZSEL2Order[count];
  int tLength = sizeof(SZSEL2Order)*count;
  assert((tLength+sizeof(count))==length);
  binaryCodec->DecodeBytes((uint8_t*)itemArray,tLength);	
  return itemArray;
}
