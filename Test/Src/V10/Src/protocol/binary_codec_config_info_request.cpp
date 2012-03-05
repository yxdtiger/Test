#include <assert.h>
#include "binary_codec_config_info_request.h"
#include "../include/wmdf_internal_api.h"

using namespace wmdf;

BinaryCodecConfigInfoRequest::BinaryCodecConfigInfoRequest(void)
{

}

BinaryCodecConfigInfoRequest::~BinaryCodecConfigInfoRequest(void)
{

}


int BinaryCodecConfigInfoRequest::EncodeData(void* items,int count)
{
	binaryCodec->InitEncoder();
  binaryCodec->EncodeInt32(count);
	ConfigInfoRequset* requests = (ConfigInfoRequset* )items;
  binaryCodec->EncodeInt8((int32_t)requests[0].connection_id);
  binaryCodec->EncodeInt8((int32_t)requests[0].config_type);
	return binaryCodec->buffer.Get_Buffer_Used();
}

void* BinaryCodecConfigInfoRequest::DecodeData(uint8_t* bytes,int length,int& count)
{	
	binaryCodec->InitDecoder(bytes,length);
  count = binaryCodec->DecodeInt32();
	ConfigInfoRequset* itemArray=new ConfigInfoRequset[count];
  itemArray[0].connection_id = binaryCodec->DecodeInt8();
  itemArray[0].config_type = (ConfigFileType)binaryCodec->DecodeInt8();
	return itemArray;
}

int32_t wmdf::BinaryCodecConfigInfoRequest::SizeOfRecords( void* items,int count )
{
  return sizeof(ConfigInfoRequset) * count;
}
