#include <iostream>
#include "../include/wmdf_structs.h"
#include "fast_codec_shl2_trans.h"


using namespace wmdf;

FastCodecSHL2Trans::FastCodecSHL2Trans(void)
{
	mainPMap=(Fast_PMap_T*)malloc(sizeof(struct Fast_PMap_T)+SSEL2_TRANS_PMAP_BITS*sizeof(uint8_t));
	memset(mainPMap,0,sizeof(struct Fast_PMap_T)+SSEL2_TRANS_PMAP_BITS*sizeof(uint8_t));
	mainPMap->size=SSEL2_TRANS_PMAP_BITS;
	main_DefaultValue = (TValue*)calloc(sizeof(struct tap_TValue), SSEL2_TRANS_PMAP_BITS);
}

FastCodecSHL2Trans::~FastCodecSHL2Trans(void)
{
 
}


int FastCodecSHL2Trans::EncodeData(void* trans,int count)
{
  L2Transaction* item = (L2Transaction* )trans;
  fastCodec->InitEncoder();
	for (int i=0;i<count;i++)
	{
    fastCodec->Check_Msg_Avail ();
		fastCodec->EncodeNewMsg(mainPMap,main_DefaultValue);
		
		fastCodec->EncodeInt32(FAST_OP_INCR,0,item->rec_no);
		fastCodec->EncodeInt32(FAST_OP_COPY,1,item->set_no);    
		fastCodec->EncodeStr(FAST_OP_COPY,2,(uint8_t*)item->wind_code,sizeof(item->wind_code));
		fastCodec->EncodeInt32(FAST_OP_COPY,3,item->trade_time);
    fastCodec->EncodeDouble(FAST_OP_DEFAULT,4,item->price,4);
		fastCodec->EncodeInt32(FAST_OP_DEFAULT,5,item->volume);
		fastCodec->EncodeDouble(FAST_OP_DEFAULT,6,item->amount,4);
		fastCodec->EncodeEndMsg(NULL,NULL);
		Reset_PMap();
		item+=1;
	}
  ResetValueDict();
	return fastCodec->buffer.Get_Buffer_Used();
}

void* FastCodecSHL2Trans::DecodeData(uint8_t* bytes,int length,int& count)
{
	fastCodec->InitDecoder(bytes,length);

	int tid=0,capicity=DECODE_ARRAY_CAPICITY;
	int size =0;
	int int32_value=WMDF_CONSTS_32BIT_NIL_VALUE;
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
    
    currentItem.set_no = WMDF_CONSTS_32BIT_NIL_VALUE;
    currentItem.buyOrderRecNo=WMDF_CONSTS_32BIT_NIL_VALUE;
    currentItem.sellOrderRecNo = WMDF_CONSTS_32BIT_NIL_VALUE;
    currentItem.order_kind = '\0';
    currentItem.function_code = '\0';
		fastCodec->DecodeInt32(FAST_OP_INCR,0,currentItem.rec_no);
		fastCodec->DecodeInt32(FAST_OP_COPY,1,currentItem.set_no);
		fastCodec->DecodeStr(FAST_OP_COPY,2,(uint8_t*)currentItem.wind_code,sizeof(currentItem.wind_code));
    fastCodec->DecodeInt32(FAST_OP_COPY,3,currentItem.trade_time);
    fastCodec->DecodeDouble(FAST_OP_DEFAULT,4,currentItem.price,4);
		fastCodec->DecodeInt32(FAST_OP_DEFAULT,5,currentItem.volume);		
		fastCodec->DecodeDouble(FAST_OP_DEFAULT,6,currentItem.amount,4);
		fastCodec->DecodeEndMsg(NULL,NULL);    
	}
	count = size;
  ResetValueDict();
	return (void*)itemArray;
}




int32_t wmdf::FastCodecSHL2Trans::SizeOfRecords( void* items,int count )
{
  return count*sizeof(L2Transaction);
}

void wmdf::FastCodecSHL2Trans::ResetValueDict()
{
  memset(main_DefaultValue,0,sizeof(struct tap_TValue)*SSEL2_TRANS_PMAP_BITS);
}
