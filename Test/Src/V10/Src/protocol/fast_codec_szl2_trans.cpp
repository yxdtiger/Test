#include <iostream>
#include "../include/wmdf_structs.h"
#include "fast_codec_szl2_trans.h"


using namespace wmdf;

FastCodecSZL2Trans::FastCodecSZL2Trans(void)
{
	mainPMap=(Fast_PMap_T*)malloc(sizeof(struct Fast_PMap_T)+SZSEL2_TRANS_PMAP_BITS*sizeof(uint8_t));
	memset(mainPMap,0,sizeof(struct Fast_PMap_T)+SZSEL2_TRANS_PMAP_BITS*sizeof(uint8_t));
	mainPMap->size=SZSEL2_TRANS_PMAP_BITS;
	main_DefaultValue = (TValue*)calloc(sizeof(struct tap_TValue), SZSEL2_TRANS_PMAP_BITS);
}

FastCodecSZL2Trans::~FastCodecSZL2Trans(void)
{
 
}


int FastCodecSZL2Trans::EncodeData(void* trans,int count)
{
  L2Transaction* transactions = (L2Transaction* )trans;
  L2Transaction* transaction=NULL;
  fastCodec->InitEncoder();
	for (int i=0;i<count;i++)
	{
    transaction = &transactions[i];
    fastCodec->Check_Msg_Avail ();
		fastCodec->EncodeNewMsg(mainPMap,main_DefaultValue);
		fastCodec->EncodeInt32(FAST_OP_COPY,0,transaction->set_no);
		fastCodec->EncodeInt32(FAST_OP_INCR,1,transaction->rec_no);
		fastCodec->EncodeStr(FAST_OP_NONE,-1,(uint8_t*)transaction->wind_code,sizeof(transaction->wind_code));
		fastCodec->EncodeDouble(FAST_OP_DEFAULT,2,transaction->price,4);
		fastCodec->EncodeInt32(FAST_OP_DEFAULT,3,transaction->volume);
		fastCodec->EncodeStr(FAST_OP_COPY,4,(uint8_t*)&transaction->order_kind,1);
		fastCodec->EncodeStr(FAST_OP_COPY,5,(uint8_t*)&transaction->function_code,1);
		fastCodec->EncodeInt32(FAST_OP_COPY,6,transaction->trade_time);
    fastCodec->EncodeInt32(FAST_OP_COPY,7,transaction->buyOrderRecNo);
    fastCodec->EncodeInt32(FAST_OP_COPY,8,transaction->sellOrderRecNo);
		fastCodec->EncodeEndMsg(NULL,NULL);//2
		Reset_PMap();
	}
  ResetValueDict();
	return fastCodec->buffer.Get_Buffer_Used();
}

void* FastCodecSZL2Trans::DecodeData(uint8_t* bytes,int length,int& count)
{
	fastCodec->InitDecoder(bytes,length);

	int tid=0,capicity=DECODE_ARRAY_CAPICITY;
	int size =0;

	L2Transaction* itemArray=new L2Transaction[capicity];
	while(fastCodec->DecodeNewMsg(mainPMap,main_DefaultValue)>0)
	{	
		if(size>=capicity)
		{
			L2Transaction* temp = new L2Transaction[capicity*2];
      memset(temp,0,sizeof(L2Transaction)*capicity*2);
			memcpy(temp,itemArray,sizeof(L2Transaction)*capicity);
			capicity=capicity*2;
			delete[] itemArray;
			itemArray = temp;
		}
		L2Transaction& currentItem=itemArray[size++];

    currentItem.amount = WMDF_CONSTS_DOUBLE_NIL_VALUE;

		fastCodec->DecodeInt32(FAST_OP_COPY,0,currentItem.set_no);
		fastCodec->DecodeInt32(FAST_OP_INCR,1,currentItem.rec_no);
		fastCodec->DecodeStr(FAST_OP_NONE,-1,(uint8_t*)currentItem.wind_code,sizeof(currentItem.wind_code));
		fastCodec->DecodeDouble(FAST_OP_DEFAULT,2,currentItem.price,4);
		fastCodec->DecodeInt32(FAST_OP_DEFAULT,3,currentItem.volume);		
		fastCodec->DecodeStr(FAST_OP_COPY,4,(uint8_t*)&currentItem.order_kind,1);
    fastCodec->DecodeStr(FAST_OP_COPY,5,(uint8_t*)&currentItem.function_code,1);
    fastCodec->DecodeInt32(FAST_OP_COPY,6,currentItem.trade_time);
    fastCodec->DecodeInt32(FAST_OP_COPY,7,currentItem.buyOrderRecNo);
    fastCodec->DecodeInt32(FAST_OP_COPY,8,currentItem.sellOrderRecNo);
		fastCodec->DecodeEndMsg(NULL,NULL);    
	}
	count = size;
  ResetValueDict();
	return (void*)itemArray;
}




int32_t wmdf::FastCodecSZL2Trans::SizeOfRecords( void* items,int count )
{
  return count*sizeof(L2Transaction);
}

void wmdf::FastCodecSZL2Trans::ResetValueDict()
{
  memset(main_DefaultValue,0,sizeof(struct tap_TValue)*SZSEL2_TRANS_PMAP_BITS);
}
