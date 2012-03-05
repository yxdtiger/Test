#include <iostream>
#include "../include/wmdf_structs.h"
#include "fast_codec_shszl1_market_data.h"


using namespace wmdf;

FastCodecSHSZL1MarketData::FastCodecSHSZL1MarketData()
{
	mainPMap=(Fast_PMap_T*)malloc(sizeof(struct Fast_PMap_T)+SHSZL1_MARKETDATA_PMAP_BITS*sizeof(uint8_t));
	memset(mainPMap,0,sizeof(struct Fast_PMap_T)+SHSZL1_MARKETDATA_PMAP_BITS*sizeof(uint8_t));
	mainPMap->size=SHSZL1_MARKETDATA_PMAP_BITS;

	main_DefaultValue = (TValue*)calloc(sizeof(struct tap_TValue), SHSZL1_MARKETDATA_PMAP_BITS);
}

FastCodecSHSZL1MarketData::~FastCodecSHSZL1MarketData()
{
}


int FastCodecSHSZL1MarketData::EncodeData(void* marketDatas,int count)
{
  L1MarketSnapshot* item = (L1MarketSnapshot*)marketDatas;
  fastCodec->InitEncoder();

	for (int i=0;i<count;i++)
	{
    fastCodec->Check_Msg_Avail ();
		fastCodec->EncodeNewMsg(mainPMap,main_DefaultValue);
		//fastCodec->EncodeInt32(FAST_OP_COPY,0,item->trade_date);
		fastCodec->EncodeInt32(FAST_OP_COPY,1,item->data_time_stamp);
		fastCodec->EncodeStr(FAST_OP_NONE,2,(uint8_t*)item->wind_code,sizeof(item->wind_code));
		fastCodec->EncodeDouble(FAST_OP_DEFAULT,3,item->pre_close,4);
		fastCodec->EncodeDouble(FAST_OP_DEFAULT,4,item->open_price,4);
		fastCodec->EncodeDouble(FAST_OP_DEFAULT,5,item->high_price,4);
		fastCodec->EncodeDouble(FAST_OP_DEFAULT,6,item->low_price,4);
		fastCodec->EncodeDouble(FAST_OP_DEFAULT,7,item->new_price,4);
		fastCodec->EncodeInt64(FAST_OP_DEFAULT,8,item->total_volume);
		fastCodec->EncodeDouble(FAST_OP_DEFAULT,9,item->total_amount,5);		
		
		
		if(item->bid_size1!=WMDF_CONSTS_32BIT_NIL_VALUE)
		{	
			fastCodec->EncodeInt32(FAST_OP_NONE,-1,5);
			fastCodec->EncodeInt32(FAST_OP_NONE,-1,item->bid_size1);
			fastCodec->EncodeDouble(FAST_OP_NONE,-1,item->bid_price1		,4);
			fastCodec->EncodeInt32(FAST_OP_NONE,-1,item->bid_size2);
			fastCodec->EncodeDouble(FAST_OP_NONE,-1,item->bid_price2		,4);
			fastCodec->EncodeInt32(FAST_OP_NONE,-1,item->bid_size3);
			fastCodec->EncodeDouble(FAST_OP_NONE,-1,item->bid_price3		,4);
			fastCodec->EncodeInt32(FAST_OP_NONE,-1,item->bid_size4);
			fastCodec->EncodeDouble(FAST_OP_NONE,-1,item->bid_price4		,4);
			fastCodec->EncodeInt32(FAST_OP_NONE,-1,item->bid_size5);
			fastCodec->EncodeDouble(FAST_OP_NONE,-1,item->bid_price5		,4);			
		}
		else
		{
			fastCodec->EncodeInt32(FAST_OP_NONE,-1,0);
		}

		if(item->ask_size1!=WMDF_CONSTS_32BIT_NIL_VALUE)
		{	
			fastCodec->EncodeInt32(FAST_OP_NONE,-1,5);
			fastCodec->EncodeInt32(FAST_OP_NONE,-1,item->ask_size1);
			fastCodec->EncodeDouble(FAST_OP_NONE,-1,item->ask_price1		,4);
			fastCodec->EncodeInt32(FAST_OP_NONE,-1,item->ask_size2);
			fastCodec->EncodeDouble(FAST_OP_NONE,-1,item->ask_price2	,4);
			fastCodec->EncodeInt32(FAST_OP_NONE,-1,item->ask_size3);
			fastCodec->EncodeDouble(FAST_OP_NONE,-1,item->ask_price3 	,4);
			fastCodec->EncodeInt32(FAST_OP_NONE,-1,item->ask_size4);
			fastCodec->EncodeDouble(FAST_OP_NONE,-1,item->ask_price4		,4);
			fastCodec->EncodeInt32(FAST_OP_NONE,-1,item->ask_size5);
			fastCodec->EncodeDouble(FAST_OP_NONE,-1,item->ask_price5		,4);			
		}
		else
		{
			fastCodec->EncodeInt32(FAST_OP_NONE,-1,0);
		}
		fastCodec->EncodeEndMsg(NULL,NULL);
		Reset_PMap();
		item+=1;
	}
  ResetValueDict();
	return fastCodec->buffer.Get_Buffer_Used();
}

void* FastCodecSHSZL1MarketData::DecodeData(uint8_t* bytes,int length,int& count)
{
	int capicity=DECODE_ARRAY_CAPICITY;
	int size =0;
	int int32_value=WMDF_CONSTS_32BIT_NIL_VALUE;
	int64_t int64_value = WMDF_CONSTS_64BIT_NIL_VALUE;

	fastCodec->InitDecoder(bytes,length);
	L1MarketSnapshot* itemArray=new L1MarketSnapshot[capicity];
	while(fastCodec->DecodeNewMsg(mainPMap,main_DefaultValue)>0)
	{
		if(size>=capicity)
		{
			L1MarketSnapshot* temp = new L1MarketSnapshot[capicity*2];
      memset(temp,0,sizeof(L1MarketSnapshot)*capicity*2);
			memcpy(temp,itemArray,sizeof(L1MarketSnapshot)*capicity);
			capicity=capicity*2;
			delete[] itemArray;
			itemArray = temp;
		}
		L1MarketSnapshot& currentItem=itemArray[size++];

	
		//fastCodec->DecodeInt32(FAST_OP_COPY,0,currentItem.trade_date);
		fastCodec->DecodeInt32(FAST_OP_COPY,1,currentItem.data_time_stamp);
		fastCodec->DecodeStr(FAST_OP_NONE,2,(uint8_t*)currentItem.wind_code,sizeof(currentItem.wind_code));
		
		fastCodec->DecodeDouble(FAST_OP_DEFAULT,3,currentItem.pre_close,4);
		fastCodec->DecodeDouble(FAST_OP_DEFAULT,4,currentItem.open_price,4);
		fastCodec->DecodeDouble(FAST_OP_DEFAULT,5,currentItem.high_price,4);
		fastCodec->DecodeDouble(FAST_OP_DEFAULT,6,currentItem.low_price,4);
		fastCodec->DecodeDouble(FAST_OP_DEFAULT,7,currentItem.new_price,4);
		fastCodec->DecodeInt64(FAST_OP_DEFAULT,8,currentItem.total_volume);		 
		fastCodec->DecodeDouble(FAST_OP_DEFAULT,9,currentItem.total_amount,5);
		

		fastCodec->DecodeInt32(FAST_OP_NONE,-1,int32_value);
		if(5==int32_value)
		{
			fastCodec->DecodeInt32(FAST_OP_NONE,-1,currentItem.bid_size1);
			fastCodec->DecodeDouble(FAST_OP_NONE,-1,currentItem.bid_price1,4);
			fastCodec->DecodeInt32(FAST_OP_NONE,-1,currentItem.bid_size2	);
			fastCodec->DecodeDouble(FAST_OP_NONE,-1,currentItem.bid_price2	,4);	
			fastCodec->DecodeInt32(FAST_OP_NONE,-1,currentItem.bid_size3);
			fastCodec->DecodeDouble(FAST_OP_NONE,-1,currentItem.bid_price3	,4);	
			fastCodec->DecodeInt32(FAST_OP_NONE,-1,currentItem.bid_size4);
			fastCodec->DecodeDouble(FAST_OP_NONE,-1,currentItem.bid_price4	,4);	
			fastCodec->DecodeInt32(FAST_OP_NONE,-1,currentItem.bid_size5);
			fastCodec->DecodeDouble(FAST_OP_NONE,-1,currentItem.bid_price5	,4);				
		}
    else
    {
      currentItem.bid_size1 = WMDF_CONSTS_32BIT_NIL_VALUE;
      currentItem.bid_size2 = WMDF_CONSTS_32BIT_NIL_VALUE;
      currentItem.bid_size3 = WMDF_CONSTS_32BIT_NIL_VALUE;
      currentItem.bid_size4 = WMDF_CONSTS_32BIT_NIL_VALUE;
      currentItem.bid_size5 = WMDF_CONSTS_32BIT_NIL_VALUE;
      currentItem.bid_price1 = WMDF_CONSTS_DOUBLE_NIL_VALUE;
      currentItem.bid_price2 = WMDF_CONSTS_DOUBLE_NIL_VALUE;
      currentItem.bid_price3 = WMDF_CONSTS_DOUBLE_NIL_VALUE;
      currentItem.bid_price4 = WMDF_CONSTS_DOUBLE_NIL_VALUE;
      currentItem.bid_price5 = WMDF_CONSTS_DOUBLE_NIL_VALUE;
    }

		fastCodec->DecodeInt32(FAST_OP_NONE,-1,int32_value);
		if(5==int32_value)
		{
			fastCodec->DecodeInt32(FAST_OP_NONE,-1,currentItem.ask_size1);
			fastCodec->DecodeDouble(FAST_OP_NONE,-1,currentItem.ask_price1,4);
			fastCodec->DecodeInt32(FAST_OP_NONE,-1,currentItem.ask_size2);
			fastCodec->DecodeDouble(FAST_OP_NONE,-1,currentItem.ask_price2,4);
			fastCodec->DecodeInt32(FAST_OP_NONE,-1,currentItem.ask_size3);
			fastCodec->DecodeDouble(FAST_OP_NONE,-1,currentItem.ask_price3	,4);
			fastCodec->DecodeInt32(FAST_OP_NONE,-1,currentItem.ask_size4);
			fastCodec->DecodeDouble(FAST_OP_NONE,-1,currentItem.ask_price4	,4);
			fastCodec->DecodeInt32(FAST_OP_NONE,-1,currentItem.ask_size5);
			fastCodec->DecodeDouble(FAST_OP_NONE,-1,currentItem.ask_price5	,4);				
		}
    else
    {
      currentItem.ask_size1 = WMDF_CONSTS_32BIT_NIL_VALUE;
      currentItem.ask_size2 = WMDF_CONSTS_32BIT_NIL_VALUE;
      currentItem.ask_size3 = WMDF_CONSTS_32BIT_NIL_VALUE;
      currentItem.ask_size4 = WMDF_CONSTS_32BIT_NIL_VALUE;
      currentItem.ask_size5 = WMDF_CONSTS_32BIT_NIL_VALUE;
      currentItem.ask_price1 = WMDF_CONSTS_DOUBLE_NIL_VALUE;
      currentItem.ask_price2 = WMDF_CONSTS_DOUBLE_NIL_VALUE;
      currentItem.ask_price3 = WMDF_CONSTS_DOUBLE_NIL_VALUE;
      currentItem.ask_price4 = WMDF_CONSTS_DOUBLE_NIL_VALUE;
      currentItem.ask_price5 = WMDF_CONSTS_DOUBLE_NIL_VALUE;
    }

		fastCodec->DecodeEndMsg(NULL,NULL);
	}
	count = size;
  ResetValueDict();
	return (void*)itemArray;
}




int32_t wmdf::FastCodecSHSZL1MarketData::SizeOfRecords( void* items,int count )
{
  return sizeof(L1MarketSnapshot)*count;
}

void wmdf::FastCodecSHSZL1MarketData::ResetValueDict()
{
  memset(main_DefaultValue,0,sizeof(struct tap_TValue)*SHSZL1_MARKETDATA_PMAP_BITS);
}
