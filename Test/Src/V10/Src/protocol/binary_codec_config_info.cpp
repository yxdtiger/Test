#include <assert.h>
#include <iostream>
#include "binary_codec_config_info.h"
#include "../include/wmdf_internal_api.h"

using namespace wmdf;

BinaryCodecConfigInfo::BinaryCodecConfigInfo(void)
{

}

BinaryCodecConfigInfo::~BinaryCodecConfigInfo(void)
{

}


int BinaryCodecConfigInfo::EncodeData(void* items,int count)
{
	binaryCodec->InitEncoder();
  assert(count==1);
  binaryCodec->EncodeInt32(count);
	ConfigInfo* requests = (ConfigInfo* )items;
  binaryCodec->EncodeInt8((int32_t)requests[0].connection_id);
  binaryCodec->EncodeInt8((int32_t)requests[0].config_type);
  binaryCodec->EncodeInt32(requests[0].length);
  binaryCodec->EncodeBytes((uint8_t*)requests[0].bytes,requests[0].length);
	return binaryCodec->buffer.Get_Buffer_Used();
}

void* BinaryCodecConfigInfo::DecodeData(uint8_t* bytes,int length,int& count)
{	
	binaryCodec->InitDecoder(bytes,length);
  count = binaryCodec->DecodeInt32();	
  assert(count==1);
  int8_t conn_id = binaryCodec->DecodeInt8();
  ConfigFileType config_type = (ConfigFileType)binaryCodec->DecodeInt8();
  int32_t bytes_size = binaryCodec->DecodeInt32();

  ConfigInfo* item=(ConfigInfo* )malloc(sizeof(ConfigInfo)+bytes_size);
  memset(item,0,sizeof(ConfigInfo)+bytes_size);
  item->connection_id = conn_id;
  item->config_type = config_type;
  item->length = bytes_size;
  binaryCodec->DecodeBytes((uint8_t*)item->bytes,bytes_size);
	return item;
}

int32_t wmdf::BinaryCodecConfigInfo::SizeOfRecords( void* items,int count )
{
  int size = 0;
  ConfigInfo* requests = (ConfigInfo* )items;
  int32_t length1 = sizeof(ConfigInfo);
  for(int i = 0;i<count;i++)
  {
    size += length1;
    size += requests[i].length;
  }
  return size;
}
