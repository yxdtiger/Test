#include <iostream>
#include <assert.h>
#include "../include/wmdf_structs.h"
#include "binary_codec_shl2_order_queue.h"


using namespace wmdf;

BinaryCodecSHL2OrderQueue::BinaryCodecSHL2OrderQueue()
{

}

BinaryCodecSHL2OrderQueue::~BinaryCodecSHL2OrderQueue()
{
}


int BinaryCodecSHL2OrderQueue::EncodeData(void* items,int count)
{	
	binaryCodec->InitEncoder();
	binaryCodec->EncodeInt32(count);
	L2OrderQueue* orderQueues = (L2OrderQueue* )items;
	int length1 = sizeof(L2OrderQueue)-sizeof(void*);
	int length2= sizeof(L2OrderQueuePriceLevel)-sizeof(void*);
	int length3 = sizeof(L2OrderQueueItem);
	for(int i = 0;i<count;i++)
	{	
    binaryCodec->Check_Msg_Avail();
		binaryCodec->EncodeBytes((uint8_t*)&orderQueues[i],length1);
		for(int j = 0;j<orderQueues[i].price_level_count;j++)
		{			
			binaryCodec->EncodeBytes((uint8_t*)&orderQueues[i].price_levels[j],length2);
			binaryCodec->EncodeBytes((uint8_t*)orderQueues[i].price_levels[j].order_items,length3*orderQueues[i].price_levels[j].orders_number);
		}
	}
		
	return binaryCodec->buffer.Get_Buffer_Used();
}

void* BinaryCodecSHL2OrderQueue::DecodeData(uint8_t* bytes,int length,int& count)
{
	binaryCodec->InitDecoder(bytes,length);
	count = binaryCodec->DecodeInt32();	
	L2OrderQueue* itemArray=new L2OrderQueue[count];
	int length1 = sizeof(L2OrderQueue)-sizeof(void*);
	int length2= sizeof(L2OrderQueuePriceLevel)-sizeof(void*);
	int length3 = sizeof(L2OrderQueueItem);
	for(int i=0;i<count;i++)
	{
		binaryCodec->DecodeBytes((uint8_t*)&itemArray[i],length1);
		itemArray[i].price_levels = new L2OrderQueuePriceLevel[itemArray[i].price_level_count];
		for(int j= 0;j<itemArray[i].price_level_count;j++)
		{
			binaryCodec->DecodeBytes((uint8_t*)&itemArray[i].price_levels[j],length2);
			itemArray[i].price_levels[j].order_items = new L2OrderQueueItem[itemArray[i].price_levels[j].orders_number];
			binaryCodec->DecodeBytes((uint8_t*)itemArray[i].price_levels[j].order_items,length3*itemArray[i].price_levels[j].orders_number);			
		}
	}
	return itemArray;
}




int32_t wmdf::BinaryCodecSHL2OrderQueue::SizeOfRecords( void* items,int count )
{
  int size=0;
  L2OrderQueue* orderQueues = (L2OrderQueue* )items;
  int length1 = sizeof(L2OrderQueue);
  int length2= sizeof(L2OrderQueuePriceLevel);
  int length3 = sizeof(L2OrderQueueItem);
  for(int i = 0;i<count;i++)
  {	
    size+=length1;
    for(int j = 0;j<orderQueues[i].price_level_count;j++)
    {
      size+=length2;
      size+=length3*orderQueues[i].price_levels[j].orders_number;
    }
  }
  return size;
}
