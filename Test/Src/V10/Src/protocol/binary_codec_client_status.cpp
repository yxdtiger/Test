#include "binary_codec_client_status.h"
#include <assert.h>
#include "../include/wmdf_structs.h"

using namespace wmdf;
wmdf::BinaryCodecClientStatus::BinaryCodecClientStatus(void)
{
}

wmdf::BinaryCodecClientStatus::~BinaryCodecClientStatus(void)
{
}


int BinaryCodecClientStatus::EncodeData(void* items,int count)
{
  binaryCodec->InitEncoder();
  binaryCodec->EncodeInt32(count);
  ClientStatus* clients = (ClientStatus* )items;
  int length1 = sizeof(ClientStatus)-sizeof(void*);
  int length2 = sizeof(ProductInfo);
  
  for(int i = 0;i<count;i++)
  {
    binaryCodec->EncodeBytes((uint8_t*)&clients[i],length1);    
    binaryCodec->EncodeBytes((uint8_t*)clients[i].product_infos,length2*clients[i].product_count);
  }
  return binaryCodec->buffer.Get_Buffer_Used();
}

void* BinaryCodecClientStatus::DecodeData(uint8_t* bytes,int length,int& count)
{	
  binaryCodec->InitDecoder(bytes,length);
  count = binaryCodec->DecodeInt32();	
  ClientStatus* itemArray=new ClientStatus[count];
  int length1 = sizeof(ClientStatus)-sizeof(void*);
  int length2 = sizeof(ProductInfo);
  for(int i=0;i<count;i++)
  {
    binaryCodec->DecodeBytes((uint8_t*)&itemArray[i],length1); 
    itemArray[i].product_infos = new ProductInfo[itemArray[i].product_count];
    binaryCodec->DecodeBytes((uint8_t*)itemArray[i].product_infos,length2*itemArray[i].product_count);
  }
  return itemArray;
}




int32_t wmdf::BinaryCodecClientStatus::SizeOfRecords( void* items,int count )
{
  int32_t size=0;
  ClientStatus* clients = (ClientStatus* )items;
  int length1 = sizeof(ClientStatus)-sizeof(void*);
  int length2 = sizeof(ProductInfo);
  for(int i = 0;i<count;i++)
  {
    size+=length1;
    size+=length2*clients[i].product_count;
    
  }
  return size;
}
