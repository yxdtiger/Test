#include <assert.h>
#include "binary_codec_login_request.h"
#include "../include/wmdf_structs.h"

using namespace wmdf;

BinaryCodecLoginRequest::BinaryCodecLoginRequest(void)
{

}

BinaryCodecLoginRequest::~BinaryCodecLoginRequest(void)
{

}


int BinaryCodecLoginRequest::EncodeData(void* items,int count)
{
	binaryCodec->InitEncoder();
	binaryCodec->EncodeInt32(count);
	LoginRequest* requests = (LoginRequest* )items;
  int size = sizeof(ProductInfo);
	for(int i = 0;i<count;i++)
	{
		binaryCodec->EncodeBytes((uint8_t*)requests[i].user_name,sizeof(requests[i].user_name));
    binaryCodec->EncodeBytes((uint8_t*)requests[i].password,sizeof(requests[i].password));
    binaryCodec->EncodeInt32(requests[i].product_count);
		binaryCodec->EncodeBytes((uint8_t*)requests[i].product,size*requests[i].product_count);
	}
	return binaryCodec->buffer.Get_Buffer_Used();
}

void* BinaryCodecLoginRequest::DecodeData(uint8_t* bytes,int length,int& count)
{	
	binaryCodec->InitDecoder(bytes,length);
	count = binaryCodec->DecodeInt32();	
	LoginRequest* itemArray=new LoginRequest[count];
	int size = sizeof(ProductInfo);
	for(int i=0;i<count;i++)
	{
		binaryCodec->DecodeBytes((uint8_t*)itemArray[i].user_name,sizeof(itemArray[i].user_name));
    binaryCodec->DecodeBytes((uint8_t*)itemArray[i].password,sizeof(itemArray[i].password));
    itemArray[i].product_count = binaryCodec->DecodeInt32();
		itemArray[i].product = new ProductInfo[itemArray[i].product_count];
		binaryCodec->DecodeBytes((uint8_t*)itemArray[i].product,size*itemArray[i].product_count);
	}
	return itemArray;
}




int32_t wmdf::BinaryCodecLoginRequest::SizeOfRecords( void* items,int count )
{
  int size = 0;
  LoginRequest* requests = (LoginRequest* )items;
  int32_t length1 = sizeof(LoginRequest);
  int32_t length2 = sizeof(ProductInfo);
  for(int i = 0;i<count;i++)
  {
    size+=length1;
    size+=length2*requests[i].product_count;
  }
  return size;
}
