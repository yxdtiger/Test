#include "../include/wmdf_structs.h"
#include "stop_bit_codec_szl2_orders.h"

using namespace wmdf;
wmdf::StopBitCodecSZL2Orders::StopBitCodecSZL2Orders(void)
{
  mainPMap=(Fast_PMap_T*)malloc(sizeof(struct Fast_PMap_T)+STOP_BIT_SZL2_ORDERS_PMAP_BITS*sizeof(uint8_t));
  memset(mainPMap,0,sizeof(struct Fast_PMap_T)+STOP_BIT_SZL2_ORDERS_PMAP_BITS*sizeof(uint8_t));
  mainPMap->size=STOP_BIT_SZL2_ORDERS_PMAP_BITS;

  main_DefaultValue = (TValue*)calloc(sizeof(struct tap_TValue), STOP_BIT_SZL2_ORDERS_PMAP_BITS);
}

wmdf::StopBitCodecSZL2Orders::~StopBitCodecSZL2Orders(void)
{
}

int32_t wmdf::StopBitCodecSZL2Orders::SizeOfRecords( void* items,int count )
{
  return sizeof(SZSEL2Order)*count;
}
int StopBitCodecSZL2Orders::EncodeData(void* trans,int count)
{
	SZSEL2Order* orders = (SZSEL2Order* )trans;
	SZSEL2Order* order=NULL;
	fastCodec->InitEncoder();
	for (int i=0;i<count;i++)
	{
		order = &orders[i];
    fastCodec->Check_Msg_Avail ();
		//fastCodec->EncodeNewMsg(mainPMap,main_DefaultValue);
		fastCodec->EncodeInt32(FAST_OP_NONE,-1,order->set_no);
		fastCodec->EncodeInt32(FAST_OP_NONE,-1,order->rec_no);
		fastCodec->EncodeStr(FAST_OP_NONE,-1,(uint8_t*)order->wind_code,sizeof(order->wind_code));
		fastCodec->EncodeDouble(FAST_OP_NONE,-1,order->order_price,4);
		fastCodec->EncodeInt32(FAST_OP_NONE,-1,order->order_qty);
		fastCodec->EncodeStr(FAST_OP_NONE,-1,(uint8_t*)&order->order_kind,1);
		fastCodec->EncodeStr(FAST_OP_NONE,-1,(uint8_t*)&order->function_code,1);
		fastCodec->EncodeInt32(FAST_OP_NONE,-1,order->order_time);
		//fastCodec->EncodeEndMsg(NULL,NULL);
		Reset_PMap();
	}
	return fastCodec->buffer.Get_Buffer_Used();
}

void* StopBitCodecSZL2Orders::DecodeData(uint8_t* bytes,int length,int& count)
{
	fastCodec->InitDecoder(bytes,length);

	int tid=0,capicity=DECODE_ARRAY_CAPICITY;
	int size =0;

	SZSEL2Order* itemArray=new SZSEL2Order[capicity];
	while(fastCodec->buffer.Get_Buffer_Left()>0)
	{	
		if(size>=capicity)
		{
			SZSEL2Order* temp = new SZSEL2Order[capicity*2];
      memset(temp,0,sizeof(SZSEL2Order)*capicity*2);
			memcpy(temp,itemArray,sizeof(SZSEL2Order)*capicity);
			capicity=capicity*2;
			delete[] itemArray;
			itemArray = temp;
		}
		SZSEL2Order& currentItem=itemArray[size++];

		fastCodec->DecodeInt32(FAST_OP_NONE,-1,currentItem.set_no);
		fastCodec->DecodeInt32(FAST_OP_NONE,-1,currentItem.rec_no);
		fastCodec->DecodeStr(FAST_OP_NONE,-1,(uint8_t*)currentItem.wind_code,sizeof(currentItem.wind_code));
		fastCodec->DecodeDouble(FAST_OP_NONE,-1,currentItem.order_price,4);
		fastCodec->DecodeInt32(FAST_OP_NONE,-1,currentItem.order_qty);		
		fastCodec->DecodeStr(FAST_OP_NONE,-1,(uint8_t*)&currentItem.order_kind,1);
		fastCodec->DecodeStr(FAST_OP_NONE,-1,(uint8_t*)&currentItem.function_code,1);
		fastCodec->DecodeInt32(FAST_OP_NONE,-1,currentItem.order_time);


		//fastCodec->DecodeEndMsg(NULL,NULL);
	}
	count = size;
	return (void*)itemArray;
}
