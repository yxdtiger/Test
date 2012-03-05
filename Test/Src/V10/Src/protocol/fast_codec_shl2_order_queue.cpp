#include <iostream>
#include <assert.h>
#include "fast_codec_shl2_order_queue.h"

using namespace wmdf;

FastCodecSHL2OrderQueue::FastCodecSHL2OrderQueue()
{
	mainPMap=(Fast_PMap_T*)malloc(sizeof(struct Fast_PMap_T)+SHL2_ORDER_QUEUE_PMAP_BITS*sizeof(uint8_t));
	memset(mainPMap,0,sizeof(struct Fast_PMap_T)+SHL2_ORDER_QUEUE_PMAP_BITS*sizeof(uint8_t));
	mainPMap->size=SHL2_ORDER_QUEUE_PMAP_BITS;
	main_DefaultValue = (TValue*)calloc(sizeof(struct tap_TValue), SHL2_ORDER_QUEUE_PMAP_BITS);

  priceLevelPMap=(Fast_PMap_T*)malloc(sizeof(struct Fast_PMap_T)+SHL2_ORDER_QUEUE_PRICELEVEL_PMAP_BITS*sizeof(uint8_t));
	memset(priceLevelPMap,0,sizeof(struct Fast_PMap_T)+SHL2_ORDER_QUEUE_PRICELEVEL_PMAP_BITS*sizeof(uint8_t));
	priceLevelPMap->size=SHL2_ORDER_QUEUE_PRICELEVEL_PMAP_BITS;
	priceLevelDefaultValue = (TValue*)calloc(sizeof(struct tap_TValue), SHL2_ORDER_QUEUE_PRICELEVEL_PMAP_BITS);

  orderPMap=(Fast_PMap_T*)malloc(sizeof(struct Fast_PMap_T)+SHL2_ORDER_QUEUE_ORDER_PMAP_BITS*sizeof(uint8_t));
	memset(orderPMap,0,sizeof(struct Fast_PMap_T)+SHL2_ORDER_QUEUE_ORDER_PMAP_BITS*sizeof(uint8_t));
	orderPMap->size=SHL2_ORDER_QUEUE_ORDER_PMAP_BITS;
 	orderDefaultValue = (TValue*)calloc(sizeof(struct tap_TValue), SHL2_ORDER_QUEUE_ORDER_PMAP_BITS);
}

FastCodecSHL2OrderQueue::~FastCodecSHL2OrderQueue()
{
  if(priceLevelPMap!=NULL)
  {
    free(priceLevelPMap);
    priceLevelPMap=NULL;
  }

  if(orderPMap!=NULL)
  {
    free(orderPMap);
    orderPMap=NULL;
  }

  if(priceLevelDefaultValue!=NULL)
  {
    free(priceLevelDefaultValue);	
    priceLevelDefaultValue=NULL;
  }

  if(orderDefaultValue!=NULL)
  {
    free(orderDefaultValue);	
    orderDefaultValue=NULL;
  }
}

void FastCodecSHL2OrderQueue::TolerateOrderQueue(L2OrderQueue* orderQueue)
{
  for(int i=0;i<orderQueue->price_level_count;i++)
  {
    if(orderQueue->image_status==1)//full image
    {
      orderQueue->price_levels[i].operator_type=WMDF_CONSTS_32BIT_NIL_VALUE;//absent
    }
    if(orderQueue->price_levels[i].operator_type==3)//delete
    {
      orderQueue->price_levels[i].orders_number=0;
    }
    for(int j=0;j<orderQueue->price_levels[i].orders_number;j++)
    {
      if(orderQueue->price_levels[i].operator_type==1||orderQueue->price_levels[i].operator_type==WMDF_CONSTS_32BIT_NIL_VALUE)//add
      {
        orderQueue->price_levels[i].order_items[j].operator_type=WMDF_CONSTS_32BIT_NIL_VALUE;
        orderQueue->price_levels[i].order_items[j].entry_id=WMDF_CONSTS_32BIT_NIL_VALUE;
      }
      else if(orderQueue->price_levels[i].operator_type==2)
      {
        if(orderQueue->price_levels[i].order_items[j].operator_type==1)
        {
          orderQueue->price_levels[i].order_items[j].entry_id=WMDF_CONSTS_32BIT_NIL_VALUE;
        }
        else if(orderQueue->price_levels[i].order_items[j].operator_type==3)
        {
          orderQueue->price_levels[i].order_items[j].size=WMDF_CONSTS_32BIT_NIL_VALUE;
        }
      }
    }
  }
}

int FastCodecSHL2OrderQueue::EncodeData(void* items,int count)
{
  L2OrderQueue* orderQueues = (L2OrderQueue*)items;
  fastCodec->InitEncoder();
  L2OrderQueue* orderQueue ;
  L2OrderQueuePriceLevel* orderQueueLevel;
  L2OrderQueueItem* orderQueueLevelQty ;
	for (int i=0;i<count;i++)
	{
    fastCodec->Check_Msg_Avail ();
    orderQueue = &orderQueues[i];
    //TolerateOrderQueue(orderQueue);
	  fastCodec->EncodeNewMsg(mainPMap,main_DefaultValue);
	  fastCodec->EncodeInt32(FAST_OP_COPY,0,orderQueue->data_time_stamp);
	  fastCodec->EncodeInt32(FAST_OP_COPY,1,orderQueue->order_side);
	  fastCodec->EncodeStr(FAST_OP_NONE,2,(uint8_t*)orderQueue->wind_code,sizeof(orderQueue->wind_code));
    fastCodec->EncodeInt32(FAST_OP_COPY,3,orderQueue->image_status);
    fastCodec->EncodeInt32(FAST_OP_NONE,-1,orderQueue->price_level_count);
		if (orderQueue->price_level_count >= 0)
		{
			for(int j=0;j<orderQueue->price_level_count;j++)
			{
				orderQueueLevel = &orderQueue->price_levels[j];
				fastCodec->EncodeNewMsg(priceLevelPMap,priceLevelDefaultValue);
				fastCodec->EncodeInt32(FAST_OP_DEFAULT,0,orderQueueLevel->operator_type);
				fastCodec->EncodeDouble(FAST_OP_DEFAULT,1,orderQueueLevel->order_price,4);
				fastCodec->EncodeInt32(FAST_OP_DEFAULT,2,orderQueueLevel->total_number);
				fastCodec->EncodeInt32(FAST_OP_NONE,-1,orderQueueLevel->orders_number);
				if (orderQueueLevel->orders_number >= 0)
				{
					for(int k =0;k<orderQueueLevel->orders_number;k++)
					{
						orderQueueLevelQty = &orderQueueLevel->order_items[k];
						fastCodec->EncodeNewMsg(orderPMap,orderDefaultValue);
						fastCodec->EncodeInt32(FAST_OP_DEFAULT,0,orderQueueLevelQty->operator_type);
						fastCodec->EncodeInt32(FAST_OP_DEFAULT,1,orderQueueLevelQty->entry_id);
						fastCodec->EncodeInt32(FAST_OP_DEFAULT,2,orderQueueLevelQty->size);
						fastCodec->EncodeEndMsg(priceLevelPMap,priceLevelDefaultValue);
					}
				}
				fastCodec->EncodeEndMsg(mainPMap,main_DefaultValue);
			}
		}
		fastCodec->EncodeEndMsg();
		Reset_PMap();
	}
  ResetValueDict();
	return fastCodec->buffer.Get_Buffer_Used();
}




void* FastCodecSHL2OrderQueue::DecodeData(uint8_t* bytes,int length,int& count)
{
	int capicity=DECODE_ARRAY_CAPICITY;
	int size =0;
	int int32_value=WMDF_CONSTS_32BIT_NIL_VALUE;
	fastCodec->InitDecoder(bytes,length);
  L2OrderQueue* itemArray=new L2OrderQueue[capicity];
  L2OrderQueue* currentItem =NULL;
  L2OrderQueuePriceLevel* orderQueueLevel;
  L2OrderQueueItem* orderQueueLevelQty ;

	while(fastCodec->DecodeNewMsg(mainPMap,main_DefaultValue)>0)
	{
		if(size>=capicity)
		{
			L2OrderQueue* temp = new L2OrderQueue[capicity*2];
      memset(temp,0,sizeof(L2OrderQueue)*capicity*2);
			memcpy(temp,itemArray,sizeof(L2OrderQueue)*capicity);
			capicity=capicity*2;
			delete[] itemArray;
      itemArray=NULL;
			itemArray = temp;
		}
		currentItem=&itemArray[size++];

		fastCodec->DecodeInt32(FAST_OP_COPY,0,currentItem->data_time_stamp);

    fastCodec->DecodeInt32(FAST_OP_COPY,1,int32_value);
    currentItem->order_side = int32_value;
    fastCodec->DecodeStr(FAST_OP_NONE,2,(uint8_t*)currentItem->wind_code,sizeof(currentItem->wind_code));
    fastCodec->DecodeInt32(FAST_OP_COPY,3,currentItem->image_status);
    fastCodec->DecodeInt32(FAST_OP_NONE,-1,currentItem->price_level_count);
    if (currentItem->price_level_count >= 0)
    {
			L2OrderQueuePriceLevel* levels =new L2OrderQueuePriceLevel[currentItem->price_level_count];    
			currentItem->price_levels = levels;
			for(int i=0;i<currentItem->price_level_count;i++)
			{ 
				orderQueueLevel = &currentItem->price_levels[i];
				fastCodec->DecodeNewMsg(priceLevelPMap,priceLevelDefaultValue);
				fastCodec->DecodeInt32(FAST_OP_DEFAULT,0,(orderQueueLevel->operator_type));
				fastCodec->DecodeDouble(FAST_OP_DEFAULT,1,orderQueueLevel->order_price,4);
				fastCodec->DecodeInt32(FAST_OP_DEFAULT,2,(orderQueueLevel->total_number));
				fastCodec->DecodeInt32(FAST_OP_NONE,-1,(orderQueueLevel->orders_number));
				if (orderQueueLevel->orders_number >= 0)
				{
					orderQueueLevel->order_items = new L2OrderQueueItem[orderQueueLevel->orders_number];
					for(int j=0;j<orderQueueLevel->orders_number;j++)
					{   
						orderQueueLevelQty=&orderQueueLevel->order_items[j];
						fastCodec->DecodeNewMsg(orderPMap,orderDefaultValue);
						fastCodec->DecodeInt32(FAST_OP_DEFAULT,0,(orderQueueLevelQty->operator_type));
						fastCodec->DecodeInt32(FAST_OP_DEFAULT,0,(orderQueueLevelQty->entry_id));
						fastCodec->DecodeInt32(FAST_OP_DEFAULT,0,(orderQueueLevelQty->size));
						fastCodec->DecodeEndMsg(priceLevelPMap,priceLevelDefaultValue);
					}
				}
				fastCodec->DecodeEndMsg(mainPMap,main_DefaultValue);
			}	
    }
    	
		fastCodec->DecodeEndMsg();
	}
	count = size;
  ResetValueDict();
	return (void*)itemArray;
}

int32_t wmdf::FastCodecSHL2OrderQueue::SizeOfRecords( void* items,int count )
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

void wmdf::FastCodecSHL2OrderQueue::ResetValueDict()
{
  memset(main_DefaultValue,0,sizeof(struct tap_TValue)*SHL2_ORDER_QUEUE_PMAP_BITS);
  memset(priceLevelDefaultValue,0,sizeof(struct tap_TValue)*SHL2_ORDER_QUEUE_PRICELEVEL_PMAP_BITS);
  memset(orderDefaultValue,0,sizeof(struct tap_TValue)*SHL2_ORDER_QUEUE_ORDER_PMAP_BITS);
}
