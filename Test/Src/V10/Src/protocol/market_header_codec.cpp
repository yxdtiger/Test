#include "market_header_codec.h"
#include "../include/wmdf_structs.h"
using namespace wmdf;

wmdf::MarketHeaderCodec::MarketHeaderCodec(void)
{
}

wmdf::MarketHeaderCodec::~MarketHeaderCodec(void)
{
}

int32_t wmdf::MarketHeaderCodec::SizeOfRecords()
{
  return sizeof(MarketMessageHeader);
}

int MarketHeaderCodec::EncodeData()
{	
  binaryCodec->InitEncoder();
  int length = sizeof(MarketMessageHeader);
  binaryCodec->EncodeBytes((uint8_t*)header_,length);	
  return binaryCodec->buffer.Get_Buffer_Used();
}

void* MarketHeaderCodec::DecodeData(uint8_t* bytes,int& length)
{
  binaryCodec->InitDecoder(bytes,length);
  length = sizeof(MarketMessageHeader);
  MarketMessageHeader* header = new MarketMessageHeader();
  binaryCodec->DecodeBytes((uint8_t*)header,length);
  return header;
}
