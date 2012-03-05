#include <assert.h>
#include "system_header_codec.h"
#include "../include/wmdf_structs.h"
using namespace wmdf;

wmdf::SystemHeaderCodec::SystemHeaderCodec(void)
{
}

wmdf::SystemHeaderCodec::~SystemHeaderCodec(void)
{
}

int32_t wmdf::SystemHeaderCodec::SizeOfRecords()
{
  return sizeof(SystemMessageHeader);
}


int SystemHeaderCodec::EncodeData()
{	
  binaryCodec->InitEncoder();
  int length = sizeof(SystemMessageHeader);
  binaryCodec->EncodeBytes((uint8_t*)header_,length);	
  return binaryCodec->buffer.Get_Buffer_Used();
}

void* SystemHeaderCodec::DecodeData(uint8_t* bytes,int& length)
{
  binaryCodec->InitDecoder(bytes,length);
  length = sizeof(SystemMessageHeader);
  SystemMessageHeader* header = new SystemMessageHeader();
  binaryCodec->DecodeBytes((uint8_t*)header,length);
  return header;
}
