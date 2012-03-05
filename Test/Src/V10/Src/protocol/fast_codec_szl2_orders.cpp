#include "../include/wmdf_structs.h"
#include "fast_codec_szl2_orders.h"

using namespace wmdf;
wmdf::FastCodecSZL2Orders::FastCodecSZL2Orders(void)
{
  mainPMap=(Fast_PMap_T*)malloc(sizeof(struct Fast_PMap_T)+SZL2_ORDERS_PMAP_BITS*sizeof(uint8_t));
  memset(mainPMap,0,sizeof(struct Fast_PMap_T)+SZL2_ORDERS_PMAP_BITS*sizeof(uint8_t));
  mainPMap->size=SZL2_ORDERS_PMAP_BITS;

  main_DefaultValue = (TValue*)calloc(sizeof(struct tap_TValue), SZL2_ORDERS_PMAP_BITS);
}

wmdf::FastCodecSZL2Orders::~FastCodecSZL2Orders(void)
{
}

int32_t wmdf::FastCodecSZL2Orders::SizeOfRecords( void* items,int count )
{
  return sizeof(SZSEL2Order)*count;
}

void wmdf::FastCodecSZL2Orders::ResetValueDict()
{
  memset(main_DefaultValue,0,sizeof(struct tap_TValue)*SZL2_ORDERS_PMAP_BITS);
}


int FastCodecSZL2Orders::EncodeData(void* trans,int count)
{
	SZSEL2Order* orders = (SZSEL2Order* )trans;
	SZSEL2Order* order=NULL;
	fastCodec->InitEncoder();
	for (int i=0;i<count;i++)
	{
		order = &orders[i];
    fastCodec->Check_Msg_Avail ();
		fastCodec->EncodeNewMsg(mainPMap,main_DefaultValue);
		fastCodec->EncodeInt32(FAST_OP_COPY,0,order->set_no);
		fastCodec->EncodeInt32(FAST_OP_INCR,1,order->rec_no);
		fastCodec->EncodeStr(FAST_OP_NONE,-1,(uint8_t*)order->wind_code,sizeof(order->wind_code));
		fastCodec->EncodeDouble(FAST_OP_DEFAULT,2,order->price,4);
		fastCodec->EncodeInt32(FAST_OP_DEFAULT,3,order->size);
		fastCodec->EncodeStr(FAST_OP_NONE,-1,(uint8_t*)&order->order_kind,1);
		fastCodec->EncodeStr(FAST_OP_NONE,-1,(uint8_t*)&order->function_code,1);
		fastCodec->EncodeInt32(FAST_OP_DEFAULT,4,order->order_time);
		fastCodec->EncodeEndMsg(NULL,NULL);
		Reset_PMap();
	}
  ResetValueDict();
	return fastCodec->buffer.Get_Buffer_Used();
}

void* FastCodecSZL2Orders::DecodeData(uint8_t* bytes,int length,int& count)
{
	fastCodec->InitDecoder(bytes,length);

	int tid=0,capicity=DECODE_ARRAY_CAPICITY;
	int size =0;

	SZSEL2Order* itemArray=new SZSEL2Order[capicity];
	while(fastCodec->DecodeNewMsg(mainPMap,main_DefaultValue)>0)
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

		fastCodec->DecodeInt32(FAST_OP_COPY,0,currentItem.set_no);
		fastCodec->DecodeInt32(FAST_OP_INCR,1,currentItem.rec_no);
		fastCodec->DecodeStr(FAST_OP_NONE,-1,(uint8_t*)currentItem.wind_code,sizeof(currentItem.wind_code));
		fastCodec->DecodeDouble(FAST_OP_DEFAULT,2,currentItem.price,4);
		fastCodec->DecodeInt32(FAST_OP_DEFAULT,3,currentItem.size);		
		fastCodec->DecodeStr(FAST_OP_NONE,-1,(uint8_t*)&currentItem.order_kind,1);
		fastCodec->DecodeStr(FAST_OP_NONE,-1,(uint8_t*)&currentItem.function_code,1);
		fastCodec->DecodeInt32(FAST_OP_DEFAULT,4,currentItem.order_time);


		fastCodec->DecodeEndMsg(NULL,NULL);
	}
	count = size;
  ResetValueDict();
	return (void*)itemArray;
}
