#include <iostream>
#include "fast_codec_dcel1_market_data.h"
#include "../include/wmdf_structs.h"

using namespace wmdf;

FastCodecDCEL1MarketData::FastCodecDCEL1MarketData()
{
	mainPMap=(Fast_PMap_T*)malloc(sizeof(struct Fast_PMap_T)+DCEL1_MARKETDATA_PMAP_BITS*sizeof(uint8_t));
	memset(mainPMap,0,sizeof(struct Fast_PMap_T)+DCEL1_MARKETDATA_PMAP_BITS*sizeof(uint8_t));
	mainPMap->size=DCEL1_MARKETDATA_PMAP_BITS;

  main_DefaultValue = (TValue*)calloc(sizeof(struct tap_TValue), DCEL1_MARKETDATA_PMAP_BITS);
}

FastCodecDCEL1MarketData::~FastCodecDCEL1MarketData()
{
}


int FastCodecDCEL1MarketData::EncodeData(void* marketDatas,int length)
{
  FuturesMarketSnapshot* item = (FuturesMarketSnapshot*)marketDatas;
  fastCodec->InitEncoder();

	for (int i=0;i<length;i++)
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

		fastCodec->EncodeInt32(FAST_OP_DEFAULT,10,item->bid_size1);
		fastCodec->EncodeDouble(FAST_OP_DEFAULT,11,item->bid_price1		,4);
		fastCodec->EncodeInt32(FAST_OP_DEFAULT,12,item->ask_size1);
		fastCodec->EncodeDouble(FAST_OP_DEFAULT,13,item->ask_price1		,4);    
    fastCodec->EncodeDouble(FAST_OP_DEFAULT,14,item->high_limit		,4);
    fastCodec->EncodeDouble(FAST_OP_DEFAULT,15,item->low_limit		,4);
		
		fastCodec->EncodeEndMsg(NULL,NULL);
		Reset_PMap();
		item+=1;
	}
  ResetValueDict();
	return fastCodec->buffer.Get_Buffer_Used();
}

void* FastCodecDCEL1MarketData::DecodeData(uint8_t* bytes,int length,int& count)
{
	int tid=0,capicity=DECODE_ARRAY_CAPICITY;
	int size =0;
	fastCodec->InitDecoder(bytes,length);
	FuturesMarketSnapshot* itemArray=new FuturesMarketSnapshot[capicity];
	while(fastCodec->DecodeNewMsg(mainPMap,main_DefaultValue)>0)
	{
		if(size>=capicity)
		{
			FuturesMarketSnapshot* temp = new FuturesMarketSnapshot[capicity*2];
      memset(temp,0,sizeof(FuturesMarketSnapshot)*capicity*2);
			memcpy(temp,itemArray,sizeof(FuturesMarketSnapshot)*capicity);
			capicity=capicity*2;
			delete[] itemArray;
			itemArray = temp;
		}
		FuturesMarketSnapshot& currentItem=itemArray[size++];
		currentItem.settle =      WMDF_CONSTS_DOUBLE_NIL_VALUE;
    currentItem.pre_settle = WMDF_CONSTS_DOUBLE_NIL_VALUE;
    currentItem.position =    WMDF_CONSTS_64BIT_NIL_VALUE;
    currentItem.pre_position = WMDF_CONSTS_64BIT_NIL_VALUE;
    currentItem.average_price = WMDF_CONSTS_DOUBLE_NIL_VALUE;

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
		fastCodec->DecodeInt32(FAST_OP_DEFAULT,10,currentItem.bid_size1);
		fastCodec->DecodeDouble(FAST_OP_DEFAULT,11,currentItem.bid_price1	,4);		
		fastCodec->DecodeInt32(FAST_OP_DEFAULT,12,currentItem.ask_size1);		
		fastCodec->DecodeDouble(FAST_OP_DEFAULT,13,currentItem.ask_price1	,4);    
    fastCodec->DecodeDouble(FAST_OP_DEFAULT,14,currentItem.high_limit ,4);  
    fastCodec->DecodeDouble(FAST_OP_DEFAULT,15,currentItem.low_limit ,4); 
  
		fastCodec->DecodeEndMsg(NULL,NULL);
	}
	count = size;
  ResetValueDict();
	return (void*)itemArray;
}




int32_t wmdf::FastCodecDCEL1MarketData::SizeOfRecords( void* items,int count )
{
  return sizeof(FuturesMarketSnapshot)*count;
}

void wmdf::FastCodecDCEL1MarketData::ResetValueDict()
{
  memset(main_DefaultValue,0,sizeof(struct tap_TValue)*DCEL1_MARKETDATA_PMAP_BITS);
}
