#include <assert.h>
#include "binary_codec_login_response.h"
#include "../include/wmdf_structs.h"

using namespace wmdf;

BinaryCodecLoginResponse::BinaryCodecLoginResponse(void)
{

}

BinaryCodecLoginResponse::~BinaryCodecLoginResponse(void)
{

}


int BinaryCodecLoginResponse::EncodeData(void* items,int count)
{
	binaryCodec->InitEncoder();
	binaryCodec->EncodeInt32(count);
	LoginResponse* responses = (LoginResponse* )items;

	int length1 = sizeof(ServerInfo);
	int length2 = sizeof(ProductStatus)-sizeof(void*);
  int length3 = sizeof(MessageInfo);
	for(int i = 0;i<count;i++)
	{
    binaryCodec->EncodeInt8(responses[i].ack_type);
    binaryCodec->EncodeInt32((int32_t)responses[i].user_id);
    binaryCodec->EncodeInt8(responses[i].server_count);
		binaryCodec->EncodeBytes((uint8_t*)responses[i].serverinfo,length1*responses[i].server_count);
    binaryCodec->EncodeInt8(responses[i].product_count);
    for(int j=0;j<responses[i].product_count;j++)
    {
      binaryCodec->EncodeBytes((uint8_t*)&responses[i].product_status[j],length2);
      binaryCodec->EncodeBytes((uint8_t*)responses[i].product_status[j].message_info,length3*responses[i].product_status[j].message_count);
    }		
	}
	return binaryCodec->buffer.Get_Buffer_Used();
}

void* BinaryCodecLoginResponse::DecodeData(uint8_t* bytes,int length,int& count)
{	
	binaryCodec->InitDecoder(bytes,length);
	count = binaryCodec->DecodeInt32();	
	LoginResponse* itemArray=new LoginResponse[count];
  int length1 = sizeof(ServerInfo);
  int length2 = sizeof(ProductStatus)-sizeof(void*);
  int length3 = sizeof(MessageInfo);
	for(int i=0;i<count;i++)
	{
		itemArray[i].ack_type = binaryCodec->DecodeInt8();
    itemArray[i].user_id = (uint32_t)binaryCodec->DecodeInt32();
    itemArray[i].server_count = binaryCodec->DecodeInt8();    
		itemArray[i].serverinfo = new ServerInfo[itemArray[i].server_count];
		binaryCodec->DecodeBytes((uint8_t*)itemArray[i].serverinfo,length1*itemArray[i].server_count);
    itemArray[i].product_count = binaryCodec->DecodeInt8();
    itemArray[i].product_status = new ProductStatus[itemArray[i].product_count];
    for(int j=0;j<itemArray[i].product_count;j++)
    {
      binaryCodec->DecodeBytes((uint8_t*)&itemArray[i].product_status[j],length2);
      itemArray[i].product_status[j].message_info = new MessageInfo[itemArray[i].product_status[j].message_count];
      binaryCodec->DecodeBytes((uint8_t*)itemArray[i].product_status[j].message_info,length3*itemArray[i].product_status[j].message_count);
    }    
	}
	return itemArray;
}




int32_t wmdf::BinaryCodecLoginResponse::SizeOfRecords( void* items,int count )
{
  int32_t size=0;
  LoginResponse* responses = (LoginResponse* )items;
  int length0 = sizeof(LoginResponse);
  int length1 = sizeof(ServerInfo);
  int length2 = sizeof(ProductStatus);
  int length3 = sizeof(MessageInfo);
  for(int i = 0;i<count;i++)
  {
    size+=length0;
    size+=length1*responses[i].server_count;
    for(int j=0;j<responses[i].product_count;j++)
    {
      size+=length2;
      size+=length3*responses[i].product_status[j].message_count;
    }
  }
  return size;
}
