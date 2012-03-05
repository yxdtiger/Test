#include "monitor_header_codec.h"
#include "../include/wmdf_structs.h"
using namespace wmdf;

wmdf::MonitorHeaderCodec::MonitorHeaderCodec(void)
{
}

wmdf::MonitorHeaderCodec::~MonitorHeaderCodec(void)
{
}

int32_t wmdf::MonitorHeaderCodec::SizeOfRecords()
{
  return sizeof(MonitorMessageHeader);
}


int MonitorHeaderCodec::EncodeData()
{	
  binaryCodec->InitEncoder();
  int length = sizeof(MonitorMessageHeader);
  binaryCodec->EncodeBytes((uint8_t*)header_,length);	
  return binaryCodec->buffer.Get_Buffer_Used();
}

void* MonitorHeaderCodec::DecodeData(uint8_t* bytes,int& length)
{
  binaryCodec->InitDecoder(bytes,length);
  length = sizeof(MonitorMessageHeader);
  MonitorMessageHeader* header = new MonitorMessageHeader();
  binaryCodec->DecodeBytes((uint8_t*)header,length);
  return header;
}
