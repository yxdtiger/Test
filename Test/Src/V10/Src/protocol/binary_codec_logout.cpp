#include <assert.h>
#include "../include/wmdf_structs.h"
#include "binary_codec_logout.h"
using namespace wmdf;

wmdf::BinaryCodecLogout::BinaryCodecLogout(void)
{
}

wmdf::BinaryCodecLogout::~BinaryCodecLogout(void)
{
}

int32_t wmdf::BinaryCodecLogout::SizeOfRecords( void* items,int count )
{
  return sizeof(Logout) * count;
}


int BinaryCodecLogout::EncodeData(void* items,int count)
{	
	binaryCodec->InitEncoder();
	binaryCodec->EncodeInt32(count);
	Logout* item = (Logout* )items;
	int length = sizeof(Logout) * count;
	binaryCodec->EncodeBytes((uint8_t*)item,length);	
	return binaryCodec->buffer.Get_Buffer_Used();
}

void* BinaryCodecLogout::DecodeData(uint8_t* bytes,int length,int& count)
{
	binaryCodec->InitDecoder(bytes,length);
	count = binaryCodec->DecodeInt32();	
	Logout* itemArray=new Logout[count];
	int tLength = sizeof(Logout)*count;
	assert((tLength+sizeof(count))==length);
	binaryCodec->DecodeBytes((uint8_t*)itemArray,tLength);	
	return itemArray;
}
