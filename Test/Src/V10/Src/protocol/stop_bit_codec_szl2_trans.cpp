#include <iostream>
#include "../include/wmdf_structs.h"
#include "stop_bit_codec_szl2_trans.h"


using namespace wmdf;

StopBitCodecSZL2Trans::StopBitCodecSZL2Trans(void)
{
	mainPMap=(Fast_PMap_T*)malloc(sizeof(struct Fast_PMap_T)+STOP_BIT_SZSEL2_TRANS_PMAP_BITS*sizeof(uint8_t));
	memset(mainPMap,0,sizeof(struct Fast_PMap_T)+STOP_BIT_SZSEL2_TRANS_PMAP_BITS*sizeof(uint8_t));
	mainPMap->size=STOP_BIT_SZSEL2_TRANS_PMAP_BITS;
	main_DefaultValue = (TValue*)calloc(sizeof(struct tap_TValue), STOP_BIT_SZSEL2_TRANS_PMAP_BITS);
}

StopBitCodecSZL2Trans::~StopBitCodecSZL2Trans(void)
{
 
}


int StopBitCodecSZL2Trans::EncodeData(void* trans,int count)
{
  L2Transaction* transactions = (L2Transaction* )trans;
  L2Transaction* transaction=NULL;
  fastCodec->InitEncoder();
	for (int i=0;i<count;i++)
	{
    transaction = &transactions[i];
    fastCodec->Check_Msg_Avail ();
		//fastCodec->EncodeNewMsg(mainPMap,main_DefaultValue);
		fastCodec->EncodeInt32(FAST_OP_NONE,-1,transaction->set_no);
		fastCodec->EncodeInt32(FAST_OP_NONE,-1,transaction->rec_no);
		fastCodec->EncodeStr(FAST_OP_NONE,-1,(uint8_t*)transaction->wind_code,sizeof(transaction->wind_code));
		fastCodec->EncodeDouble(FAST_OP_NONE,-1,transaction->price,4);
		fastCodec->EncodeInt32(FAST_OP_NONE,-1,transaction->trade_qty);
		fastCodec->EncodeStr(FAST_OP_NONE,-1,(uint8_t*)&transaction->order_kind,1);
		fastCodec->EncodeStr(FAST_OP_NONE,-1,(uint8_t*)&transaction->function_code,1);
		fastCodec->EncodeInt32(FAST_OP_NONE,-1,transaction->trade_time);
    fastCodec->EncodeInt32(FAST_OP_NONE,-1,transaction->buyOrderRecNo);
    fastCodec->EncodeInt32(FAST_OP_NONE,-1,transaction->sellOrderRecNo);
		//fastCodec->EncodeEndMsg(NULL,NULL);//2
		//Reset_PMap();
	}
	return fastCodec->buffer.Get_Buffer_Used();
}

void* StopBitCodecSZL2Trans::DecodeData(uint8_t* bytes,int length,int& count)
{
	fastCodec->InitDecoder(bytes,length);

	int tid=0,capicity=DECODE_ARRAY_CAPICITY;
	int size =0;

	L2Transaction* itemArray=new L2Transaction[capicity];
	while(fastCodec->buffer.Get_Buffer_Left()>0)
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

		fastCodec->DecodeInt32(FAST_OP_NONE,-1,currentItem.set_no);
		fastCodec->DecodeInt32(FAST_OP_NONE,-1,currentItem.rec_no);
		fastCodec->DecodeStr(FAST_OP_NONE,-1,(uint8_t*)currentItem.wind_code,sizeof(currentItem.wind_code));
		fastCodec->DecodeDouble(FAST_OP_NONE,-1,currentItem.price,4);
		fastCodec->DecodeInt32(FAST_OP_NONE,-1,currentItem.trade_qty);		
		fastCodec->DecodeStr(FAST_OP_NONE,-1,(uint8_t*)&currentItem.order_kind,1);
    fastCodec->DecodeStr(FAST_OP_NONE,-1,(uint8_t*)&currentItem.function_code,1);
    fastCodec->DecodeInt32(FAST_OP_NONE,-1,currentItem.trade_time);
    fastCodec->DecodeInt32(FAST_OP_NONE,-1,currentItem.buyOrderRecNo);
    fastCodec->DecodeInt32(FAST_OP_NONE,-1,currentItem.sellOrderRecNo);
		//fastCodec->DecodeEndMsg(NULL,NULL);    
	}
	count = size;
	return (void*)itemArray;
}




int32_t wmdf::StopBitCodecSZL2Trans::SizeOfRecords( void* items,int count )
{
  return sizeof(L2Transaction)*count;
}
