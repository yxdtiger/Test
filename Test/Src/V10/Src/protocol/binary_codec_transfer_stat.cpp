#include "binary_codec_transfer_stat.h"
#include <assert.h>
#include "../include/wmdf_structs.h"

using namespace wmdf;
wmdf::BinaryCodecTransferStat::BinaryCodecTransferStat(void)
{
}


wmdf::BinaryCodecTransferStat::~BinaryCodecTransferStat( void )
{

}


int BinaryCodecTransferStat::EncodeData(void* trans,int count)
{
  binaryCodec->InitEncoder();
  binaryCodec->EncodeInt32(count);
  TransferDataStat* item = (TransferDataStat* )trans;
  int length = sizeof(TransferDataStat) * count;
  binaryCodec->EncodeBytes((uint8_t*)item,length);	
  return binaryCodec->buffer.Get_Buffer_Used();
}

void* BinaryCodecTransferStat::DecodeData(uint8_t* bytes,int length,int& count)
{	
  binaryCodec->InitDecoder(bytes,length);
  count = binaryCodec->DecodeInt32();	
  TransferDataStat* itemArray=new TransferDataStat[count];
  int tLength = sizeof(TransferDataStat)*count;
  assert((tLength+sizeof(count))==length);
  binaryCodec->DecodeBytes((uint8_t*)itemArray,tLength);	
  return itemArray;
}

int32_t wmdf::BinaryCodecTransferStat::SizeOfRecords( void* items,int count )
{
  return sizeof(TransferDataStat)*count;
}
