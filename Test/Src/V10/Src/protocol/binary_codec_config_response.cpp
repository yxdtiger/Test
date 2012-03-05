#include <assert.h>
#include "binary_codec_config_response.h"
#include "../include/wmdf_internal_api.h"

using namespace wmdf;

BinaryCodecConfigResponse::BinaryCodecConfigResponse(void)
{

}

BinaryCodecConfigResponse::~BinaryCodecConfigResponse(void)
{

}


int BinaryCodecConfigResponse::EncodeData(void* items,int count)
{
	binaryCodec->InitEncoder();
  binaryCodec->EncodeInt32(count);
	UpdateConfigResponse* responses = (UpdateConfigResponse* )items;
  binaryCodec->EncodeInt8((int32_t)responses[0].connection_id);
  binaryCodec->EncodeInt8((int32_t)responses[0].config_type);
  binaryCodec->EncodeInt8((int32_t)responses[0].return_value);
	return binaryCodec->buffer.Get_Buffer_Used();
}

void* BinaryCodecConfigResponse::DecodeData(uint8_t* bytes,int length,int& count)
{	
	binaryCodec->InitDecoder(bytes,length);
  count = binaryCodec->DecodeInt32();
	UpdateConfigResponse* itemArray=new UpdateConfigResponse[count];
  itemArray[0].connection_id = binaryCodec->DecodeInt8();
  itemArray[0].config_type = (ConfigFileType)binaryCodec->DecodeInt8();
  itemArray[0].return_value = (eReturnValue)binaryCodec->DecodeInt8();
	return itemArray;
}

int32_t wmdf::BinaryCodecConfigResponse::SizeOfRecords( void* items,int count )
{
  return sizeof(UpdateConfigResponse) * count;
}
