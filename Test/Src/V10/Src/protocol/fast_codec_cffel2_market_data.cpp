#include "fast_codec_cffel2_market_data.h"
#include "../include/wmdf_structs.h"

using namespace wmdf;

FastCodecCFFEL2MarketData::FastCodecCFFEL2MarketData()
{
	mainPMap=(Fast_PMap_T*)malloc(sizeof(struct Fast_PMap_T)+CFFEL2_MARKETDATA_PMAP_BITS*sizeof(uint8_t));
	memset(mainPMap,0,sizeof(struct Fast_PMap_T)+CFFEL2_MARKETDATA_PMAP_BITS*sizeof(uint8_t));
	mainPMap->size=CFFEL2_MARKETDATA_PMAP_BITS;

  main_DefaultValue = (TValue*)calloc(sizeof(struct tap_TValue), CFFEL2_MARKETDATA_PMAP_BITS);
}

FastCodecCFFEL2MarketData::~FastCodecCFFEL2MarketData()
{
}


int FastCodecCFFEL2MarketData::EncodeData(void* marketDatas,int count)
{
  CFEL2MarketSnapshot* item = (CFEL2MarketSnapshot*)marketDatas;
  fastCodec->InitEncoder();

	for (int i=0;i<count;i++)
	{
    fastCodec->Check_Msg_Avail ();
		fastCodec->EncodeNewMsg(mainPMap,main_DefaultValue);		
		fastCodec->EncodeInt32(FAST_OP_COPY,0,item->trade_date);
		fastCodec->EncodeInt32(FAST_OP_COPY,1,item->data_time_stamp);
		fastCodec->EncodeStr(FAST_OP_NONE,2,(uint8_t*)item->wind_code,sizeof(item->wind_code));
		fastCodec->EncodeDouble(FAST_OP_DEFAULT,3,item->pre_close,4);
		fastCodec->EncodeDouble(FAST_OP_DEFAULT,4,item->open_price,4);
		fastCodec->EncodeDouble(FAST_OP_DEFAULT,5,item->high_price,4);
		fastCodec->EncodeDouble(FAST_OP_DEFAULT,6,item->low_price,4);
		fastCodec->EncodeDouble(FAST_OP_DEFAULT,7,item->new_price,4);
		fastCodec->EncodeInt64(FAST_OP_DEFAULT,8,item->total_volume);
		fastCodec->EncodeDouble(FAST_OP_DEFAULT,9,item->total_amount,5);		
		
    if(item->bid_size1!=WMDF_CONSTS_32BIT_NIL_VALUE)//要么都有值，要么都没值
    {
      fastCodec->EncodeInt32(FAST_OP_NONE,-1,5);
		  fastCodec->EncodeInt32(FAST_OP_NONE,-1,item->bid_size1);
		  fastCodec->EncodeInt32(FAST_OP_NONE,-1,item->bid_size2);
		  fastCodec->EncodeInt32(FAST_OP_NONE,-1,item->bid_size3);
		  fastCodec->EncodeInt32(FAST_OP_NONE,-1,item->bid_size4);
		  fastCodec->EncodeInt32(FAST_OP_NONE,-1,item->bid_size5);
		  fastCodec->EncodeDouble(FAST_OP_NONE,-1,item->bid_price1		,4);
		  fastCodec->EncodeDouble(FAST_OP_NONE,-1,item->bid_price2		,4);
		  fastCodec->EncodeDouble(FAST_OP_NONE,-1,item->bid_price3		,4);
		  fastCodec->EncodeDouble(FAST_OP_NONE,-1,item->bid_price4		,4);
		  fastCodec->EncodeDouble(FAST_OP_NONE,-1,item->bid_price5		,4);
    }
    else
    {
      fastCodec->EncodeInt32(FAST_OP_NONE,-1,0);
    }

    if(item->bid_size1!=WMDF_CONSTS_32BIT_NIL_VALUE)//要么都有值，要么都没值
    {
      fastCodec->EncodeInt32(FAST_OP_NONE,-1,5);
		  fastCodec->EncodeInt32(FAST_OP_NONE,-1,item->ask_size1);
		  fastCodec->EncodeInt32(FAST_OP_NONE,-1,item->ask_size2);
		  fastCodec->EncodeInt32(FAST_OP_NONE,-1,item->ask_size3);
		  fastCodec->EncodeInt32(FAST_OP_NONE,-1,item->ask_size4);
		  fastCodec->EncodeInt32(FAST_OP_NONE,-1,item->ask_size5);
		  fastCodec->EncodeDouble(FAST_OP_NONE,-1,item->ask_price1		,4);
		  fastCodec->EncodeDouble(FAST_OP_NONE,-1,item->ask_price2		,4);
		  fastCodec->EncodeDouble(FAST_OP_NONE,-1,item->ask_price3		,4);
		  fastCodec->EncodeDouble(FAST_OP_NONE,-1,item->ask_price4		,4);
		  fastCodec->EncodeDouble(FAST_OP_NONE,-1,item->ask_price5		,4);
    }
    else
    {
      fastCodec->EncodeInt32(FAST_OP_NONE,-1,0);
    }

		fastCodec->EncodeDouble(FAST_OP_DEFAULT,10,item->pre_settle		,4);
		fastCodec->EncodeInt64(FAST_OP_DEFAULT,11,item->position);
		fastCodec->EncodeInt64(FAST_OP_DEFAULT,12,item->pre_position);
		fastCodec->EncodeDouble(FAST_OP_DEFAULT,13,item->high_limit		,4);
		fastCodec->EncodeDouble(FAST_OP_DEFAULT,14,item->low_limit		,4);
		
		fastCodec->EncodeEndMsg(NULL,NULL);
		Reset_PMap();
		item+=1;
	}
	return fastCodec->buffer.Get_Buffer_Used();
}

void* FastCodecCFFEL2MarketData::DecodeData(uint8_t* bytes,int length,int& count)
{
	int capicity=DECODE_ARRAY_CAPICITY;
	int size =0;
	int int32_value=WMDF_CONSTS_32BIT_NIL_VALUE;

	fastCodec->InitDecoder(bytes,length);
	CFEL2MarketSnapshot* itemArray=new CFEL2MarketSnapshot[capicity];
	while(fastCodec->DecodeNewMsg(mainPMap,main_DefaultValue)>0)
	{
		if(size>=capicity)
		{
			CFEL2MarketSnapshot* temp = new CFEL2MarketSnapshot[capicity*2];
      memset(temp,0,sizeof(CFEL2MarketSnapshot)*capicity*2);
			memcpy(temp,itemArray,sizeof(CFEL2MarketSnapshot)*capicity);
			capicity=capicity*2;
			delete[] itemArray;
			itemArray = temp;
		}
		CFEL2MarketSnapshot& currentItem=itemArray[size++];
		
		fastCodec->DecodeInt32(FAST_OP_COPY,0,currentItem.trade_date);
		fastCodec->DecodeInt32(FAST_OP_COPY,1,currentItem.data_time_stamp);
		fastCodec->DecodeStr(FAST_OP_NONE,2,(uint8_t*)currentItem.wind_code,sizeof(currentItem.wind_code));
		
		fastCodec->DecodeDouble(FAST_OP_DEFAULT,3,currentItem.pre_close,4);		
		fastCodec->DecodeDouble(FAST_OP_DEFAULT,4,currentItem.open_price,4);		
		fastCodec->DecodeDouble(FAST_OP_DEFAULT,5,currentItem.high_price,4);		
		fastCodec->DecodeDouble(FAST_OP_DEFAULT,6,currentItem.low_price,4);		
		fastCodec->DecodeDouble(FAST_OP_DEFAULT,7,currentItem.new_price,4);
		fastCodec->DecodeInt64(FAST_OP_DEFAULT,8,currentItem.total_volume);		 
		fastCodec->DecodeDouble(FAST_OP_DEFAULT,9,currentItem.total_amount ,5);		
    
    fastCodec->DecodeInt32(FAST_OP_NONE,-1,int32_value);
    if(int32_value==5)
    {
		  fastCodec->DecodeInt32(FAST_OP_NONE,-1,currentItem.bid_size1);
		  fastCodec->DecodeInt32(FAST_OP_NONE,-1,currentItem.bid_size2);
		  fastCodec->DecodeInt32(FAST_OP_NONE,-1,currentItem.bid_size3);
		  fastCodec->DecodeInt32(FAST_OP_NONE,-1,currentItem.bid_size4);
		  fastCodec->DecodeInt32(FAST_OP_NONE,-1,currentItem.bid_size5);
		  fastCodec->DecodeDouble(FAST_OP_NONE,-1,currentItem.bid_price1	,4);		
		  fastCodec->DecodeDouble(FAST_OP_NONE,-1,currentItem.bid_price2	,4);		
		  fastCodec->DecodeDouble(FAST_OP_NONE,-1,currentItem.bid_price3	,4);		
		  fastCodec->DecodeDouble(FAST_OP_NONE,-1,currentItem.bid_price4	,4);		
		  fastCodec->DecodeDouble(FAST_OP_NONE,-1,currentItem.bid_price5	,4);
    }
    fastCodec->DecodeInt32(FAST_OP_NONE,-1,int32_value);
    if(int32_value==5)
    {  
		  fastCodec->DecodeInt32(FAST_OP_NONE,-1,currentItem.ask_size1);		
		  fastCodec->DecodeInt32(FAST_OP_NONE,-1,currentItem.ask_size2);		
		  fastCodec->DecodeInt32(FAST_OP_NONE,-1,currentItem.ask_size3);		
		  fastCodec->DecodeInt32(FAST_OP_NONE,-1,currentItem.ask_size4);		
		  fastCodec->DecodeInt32(FAST_OP_NONE,-1,currentItem.ask_size5);
		  fastCodec->DecodeDouble(FAST_OP_NONE,-1,currentItem.ask_price1	,4);		
		  fastCodec->DecodeDouble(FAST_OP_NONE,-1,currentItem.ask_price2	,4);		
		  fastCodec->DecodeDouble(FAST_OP_NONE,-1,currentItem.ask_price3	,4);		
		  fastCodec->DecodeDouble(FAST_OP_NONE,-1,currentItem.ask_price4	,4);		
		  fastCodec->DecodeDouble(FAST_OP_NONE,-1,currentItem.ask_price5	,4);	
    }
		fastCodec->DecodeDouble(FAST_OP_DEFAULT,10,currentItem.pre_settle ,4);		
		fastCodec->DecodeInt64(FAST_OP_DEFAULT,11,currentItem.position);		 
		fastCodec->DecodeInt64(FAST_OP_DEFAULT,12,currentItem.pre_position);
		fastCodec->DecodeDouble(FAST_OP_DEFAULT,13,currentItem.high_limit ,4);	
		fastCodec->DecodeDouble(FAST_OP_DEFAULT,14,currentItem.low_limit ,4);		

		fastCodec->DecodeEndMsg(NULL,NULL);    
	}
	count = size;
	return (void*)itemArray;
}




int32_t wmdf::FastCodecCFFEL2MarketData::SizeOfRecords( void* items,int count )
{
  return sizeof(CFEL2MarketSnapshot)*count;
}

void wmdf::FastCodecCFFEL2MarketData::ResetValueDict()
{
  memset(main_DefaultValue,0,sizeof(struct tap_TValue)*CFFEL2_MARKETDATA_PMAP_BITS);
}
