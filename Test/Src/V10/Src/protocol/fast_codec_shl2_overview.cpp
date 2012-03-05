#include <iostream>
#include "../include/wmdf_structs.h"
#include "fast_codec_shl2_overview.h"


using namespace wmdf;

FastCodecSHL2Overview::FastCodecSHL2Overview(void)
{
	mainPMap=(Fast_PMap_T*)malloc(sizeof(struct Fast_PMap_T)+SSEL2_OVERVIEW_PMAP_BITS*sizeof(uint8_t));
	memset(mainPMap,0,sizeof(struct Fast_PMap_T)+SSEL2_OVERVIEW_PMAP_BITS*sizeof(uint8_t));
	mainPMap->size=SSEL2_OVERVIEW_PMAP_BITS;
	main_DefaultValue = (TValue*)calloc(sizeof(struct tap_TValue), SSEL2_OVERVIEW_PMAP_BITS);
}

FastCodecSHL2Overview::~FastCodecSHL2Overview(void)
{
 
}


int FastCodecSHL2Overview::EncodeData(void* trans,int count)
{
  MarketOverView* item = (MarketOverView* )trans;
  fastCodec->InitEncoder();
	for (int i=0;i<count;i++)
	{
    fastCodec->Check_Msg_Avail ();
		fastCodec->EncodeNewMsg(mainPMap,main_DefaultValue);
		  
		fastCodec->EncodeStr(FAST_OP_COPY,0,(uint8_t*)item->wind_code,sizeof(item->wind_code));
		fastCodec->EncodeInt32(FAST_OP_COPY,1,item->date_time_stamp);
    fastCodec->EncodeInt32(FAST_OP_DEFAULT,2,item->origtime);
    fastCodec->EncodeInt32(FAST_OP_DEFAULT,3,item->origdate);
		fastCodec->EncodeEndMsg(NULL,NULL);
		Reset_PMap();
		item+=1;
	}
  ResetValueDict();
	return fastCodec->buffer.Get_Buffer_Used();
}

void* FastCodecSHL2Overview::DecodeData(uint8_t* bytes,int length,int& count)
{
	fastCodec->InitDecoder(bytes,length);

	int tid=0,capicity=DECODE_ARRAY_CAPICITY;
	int size =0;
	int int32_value=WMDF_CONSTS_32BIT_NIL_VALUE;
	MarketOverView* itemArray=new MarketOverView[capicity];
	while(fastCodec->DecodeNewMsg(mainPMap,main_DefaultValue)>0)
	{	
		if(size>=capicity)
		{
			MarketOverView* temp = new MarketOverView[capicity*2];
      memset(temp,0,sizeof(MarketOverView)*capicity*2);
			memcpy(temp,itemArray,sizeof(MarketOverView)*capicity);
			capicity=capicity*2;
			delete[] itemArray;
			itemArray = temp;
		}
		MarketOverView& currentItem=itemArray[size++];

		fastCodec->DecodeStr(FAST_OP_COPY,0,(uint8_t*)currentItem.wind_code,sizeof(currentItem.wind_code));
    fastCodec->DecodeInt32(FAST_OP_COPY,1,currentItem.date_time_stamp);
    fastCodec->DecodeInt32(FAST_OP_DEFAULT,2,currentItem.origtime);
		fastCodec->DecodeInt32(FAST_OP_DEFAULT,3,currentItem.origdate);		
		fastCodec->DecodeEndMsg(NULL,NULL);    
	}
	count = size;
  ResetValueDict();
	return (void*)itemArray;
}




int32_t wmdf::FastCodecSHL2Overview::SizeOfRecords( void* items,int count )
{
  return count*sizeof(MarketOverView);
}

void wmdf::FastCodecSHL2Overview::ResetValueDict()
{
  memset(main_DefaultValue,0,sizeof(struct tap_TValue)*SSEL2_OVERVIEW_PMAP_BITS);
}
