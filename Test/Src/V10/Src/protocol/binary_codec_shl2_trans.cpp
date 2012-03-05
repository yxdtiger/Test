#include <assert.h>
#include "../include/wmdf_structs.h"
#include "binary_codec_shl2_trans.h"


using namespace wmdf;

BinaryCodecSHL2Trans::BinaryCodecSHL2Trans(void)
{

}

BinaryCodecSHL2Trans::~BinaryCodecSHL2Trans(void)
{

}


int BinaryCodecSHL2Trans::EncodeData(void* trans,int count)
{
	binaryCodec->InitEncoder();
	binaryCodec->EncodeInt32(count);
	L2Transaction* item = (L2Transaction* )trans;
	int length = sizeof(L2Transaction) * count;
	binaryCodec->EncodeBytes((uint8_t*)item,length);	
	return binaryCodec->buffer.Get_Buffer_Used();
}

void* BinaryCodecSHL2Trans::DecodeData(uint8_t* bytes,int length,int& count)
{	
	binaryCodec->InitDecoder(bytes,length);
	count = binaryCodec->DecodeInt32();	
	L2Transaction* itemArray=new L2Transaction[count];
	int tLength = sizeof(L2Transaction)*count;
	assert((tLength+sizeof(count))==length);
	binaryCodec->DecodeBytes((uint8_t*)itemArray,tLength);	
	return itemArray;
}




int32_t wmdf::BinaryCodecSHL2Trans::SizeOfRecords( void* items,int count )
{
   return count*sizeof(L2Transaction);
}
