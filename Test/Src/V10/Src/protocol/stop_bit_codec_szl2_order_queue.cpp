#include <iostream>
#include <assert.h>
#include "stop_bit_codec_szl2_order_queue.h"

using namespace wmdf;

StopBitCodecSZL2OrderQueue::StopBitCodecSZL2OrderQueue()
{
	mainPMap=(Fast_PMap_T*)malloc(sizeof(struct Fast_PMap_T)+STOP_BIT_SZL2_ORDER_QUEUE_PMAP_BITS*sizeof(uint8_t));
	memset(mainPMap,0,sizeof(struct Fast_PMap_T)+STOP_BIT_SZL2_ORDER_QUEUE_PMAP_BITS*sizeof(uint8_t));
	mainPMap->size=STOP_BIT_SZL2_ORDER_QUEUE_PMAP_BITS;
	main_DefaultValue = (TValue*)calloc(sizeof(struct tap_TValue), STOP_BIT_SZL2_ORDER_QUEUE_PMAP_BITS);

  priceLevelPMap=(Fast_PMap_T*)malloc(sizeof(struct Fast_PMap_T)+STOP_BIT_SZL2_ORDER_QUEUE_PRICELEVEL_PMAP_BITS*sizeof(uint8_t));
	memset(priceLevelPMap,0,sizeof(struct Fast_PMap_T)+STOP_BIT_SZL2_ORDER_QUEUE_PRICELEVEL_PMAP_BITS*sizeof(uint8_t));
	priceLevelPMap->size=STOP_BIT_SZL2_ORDER_QUEUE_PRICELEVEL_PMAP_BITS;
	priceLevelDefaultValue = (TValue*)calloc(sizeof(struct tap_TValue), STOP_BIT_SZL2_ORDER_QUEUE_PRICELEVEL_PMAP_BITS);

  orderPMap=(Fast_PMap_T*)malloc(sizeof(struct Fast_PMap_T)+STOP_BIT_SZL2_ORDER_QUEUE_ORDER_PMAP_BITS*sizeof(uint8_t));
	memset(orderPMap,0,sizeof(struct Fast_PMap_T)+STOP_BIT_SZL2_ORDER_QUEUE_ORDER_PMAP_BITS*sizeof(uint8_t));
	orderPMap->size=STOP_BIT_SZL2_ORDER_QUEUE_ORDER_PMAP_BITS;
 	orderDefaultValue = (TValue*)calloc(sizeof(struct tap_TValue), STOP_BIT_SZL2_ORDER_QUEUE_ORDER_PMAP_BITS);
}

StopBitCodecSZL2OrderQueue::~StopBitCodecSZL2OrderQueue()
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

int StopBitCodecSZL2OrderQueue::EncodeData(void* items,int count)
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
	  //fastCodec->EncodeNewMsg(mainPMap,main_DefaultValue);
	  fastCodec->EncodeInt32(FAST_OP_NONE,-1,orderQueue->date_time_stamp);
	  fastCodec->EncodeInt32(FAST_OP_NONE,-1,orderQueue->side);
	  fastCodec->EncodeStr(FAST_OP_NONE,-1,(uint8_t*)orderQueue->wind_code,sizeof(orderQueue->wind_code));
    fastCodec->EncodeInt32(FAST_OP_NONE,-1,orderQueue->image_status);
    fastCodec->EncodeInt32(FAST_OP_NONE,-1,orderQueue->price_level_count);
    for(int j=0;j<orderQueue->price_level_count;j++)
    {
      orderQueueLevel = &orderQueue->order_queue_levels[j];
      //fastCodec->EncodeNewMsg(priceLevelPMap,priceLevelDefaultValue);
      fastCodec->EncodeInt32(FAST_OP_NONE,-1,orderQueueLevel->price_level_operator);
      fastCodec->EncodeDouble(FAST_OP_NONE,-1,orderQueueLevel->price,4);
      fastCodec->EncodeInt32(FAST_OP_NONE,-1,orderQueueLevel->orders_total);
      fastCodec->EncodeInt32(FAST_OP_NONE,-1,orderQueueLevel->orders_count);
      for(int k =0;k<orderQueueLevel->orders_count;k++)
      {
        orderQueueLevelQty = &orderQueueLevel->order_queue_items[k];
        //fastCodec->EncodeNewMsg(orderPMap,orderDefaultValue);
        fastCodec->EncodeInt32(FAST_OP_NONE,-1,orderQueueLevelQty->order_queue_operator);
        fastCodec->EncodeInt32(FAST_OP_NONE,-1,orderQueueLevelQty->order_queue_operator_entry_id);
        fastCodec->EncodeInt32(FAST_OP_NONE,-1,orderQueueLevelQty->order_qty);
        //fastCodec->EncodeEndMsg(priceLevelPMap,priceLevelDefaultValue);
      }
      //fastCodec->EncodeEndMsg(mainPMap,main_DefaultValue);
    }
		//fastCodec->EncodeEndMsg();
		Reset_PMap();
	}
	return fastCodec->buffer.Get_Buffer_Used();
}




void* StopBitCodecSZL2OrderQueue::DecodeData(uint8_t* bytes,int length,int& count)
{
	int capicity=DECODE_ARRAY_CAPICITY;
	int size =0;
	int int32_value=WMDF_CONSTS_32BIT_NIL_VALUE;
	fastCodec->InitDecoder(bytes,length);
  L2OrderQueue* itemArray=new L2OrderQueue[capicity];
  L2OrderQueue* currentItem =NULL;
  L2OrderQueuePriceLevel* orderQueueLevel;
  L2OrderQueueItem* orderQueueLevelQty ;

	while(fastCodec->buffer.Get_Buffer_Left()>0)
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

		fastCodec->DecodeInt32(FAST_OP_NONE,-1,currentItem->date_time_stamp);

    fastCodec->DecodeInt32(FAST_OP_NONE,-1,int32_value);
    currentItem->side = int32_value;
    fastCodec->DecodeStr(FAST_OP_NONE,-1,(uint8_t*)currentItem->wind_code,sizeof(currentItem->wind_code));
    fastCodec->DecodeInt32(FAST_OP_NONE,-1,currentItem->image_status);
    fastCodec->DecodeInt32(FAST_OP_NONE,-1,currentItem->price_level_count);
    
    L2OrderQueuePriceLevel* levels =new L2OrderQueuePriceLevel[currentItem->price_level_count];    
    currentItem->order_queue_levels = levels;
    for(int i=0;i<currentItem->price_level_count;i++)
    { 
      orderQueueLevel = &currentItem->order_queue_levels[i];
      //fastCodec->DecodeNewMsg(priceLevelPMap,priceLevelDefaultValue);
      fastCodec->DecodeInt32(FAST_OP_NONE,-1,(orderQueueLevel->price_level_operator));
      fastCodec->DecodeDouble(FAST_OP_NONE,-1,orderQueueLevel->price,4);
      fastCodec->DecodeInt32(FAST_OP_NONE,-1,(orderQueueLevel->orders_total));
      fastCodec->DecodeInt32(FAST_OP_NONE,-1,(orderQueueLevel->orders_count));
      orderQueueLevel->order_queue_items = new L2OrderQueueItem[orderQueueLevel->orders_count];
      for(int j=0;j<orderQueueLevel->orders_count;j++)
      {   
        orderQueueLevelQty=&orderQueueLevel->order_queue_items[j];
        //fastCodec->DecodeNewMsg(orderPMap,orderDefaultValue);
        fastCodec->DecodeInt32(FAST_OP_NONE,-1,(orderQueueLevelQty->order_queue_operator));
        fastCodec->DecodeInt32(FAST_OP_NONE,-1,(orderQueueLevelQty->order_queue_operator_entry_id));
        fastCodec->DecodeInt32(FAST_OP_NONE,-1,(orderQueueLevelQty->order_qty));
        //fastCodec->DecodeEndMsg(priceLevelPMap,priceLevelDefaultValue);
      }
      //fastCodec->DecodeEndMsg(mainPMap,main_DefaultValue);
    }		
		//fastCodec->DecodeEndMsg();
	}
	count = size;
	return (void*)itemArray;
}

int32_t wmdf::StopBitCodecSZL2OrderQueue::SizeOfRecords( void* items,int count )
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
      size+=length3*orderQueues[i].order_queue_levels[j].orders_count;
    }
  }
  return size;
}
