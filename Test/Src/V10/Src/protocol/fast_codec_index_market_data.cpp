#include <iostream>
#include "fast_codec_index_market_data.h"
#include "../include/wmdf_structs.h"

using namespace wmdf;

FastCodecIndexMarketData::FastCodecIndexMarketData()
{
	mainPMap=(Fast_PMap_T*)malloc(sizeof(struct Fast_PMap_T)+INDEX_MARKETDATA_PMAP_BITS*sizeof(uint8_t));
	memset(mainPMap,0,sizeof(struct Fast_PMap_T)+INDEX_MARKETDATA_PMAP_BITS*sizeof(uint8_t));
	mainPMap->size=INDEX_MARKETDATA_PMAP_BITS;

  main_DefaultValue = (TValue*)calloc(sizeof(struct tap_TValue), INDEX_MARKETDATA_PMAP_BITS);
}

FastCodecIndexMarketData::~FastCodecIndexMarketData()
{
}


int FastCodecIndexMarketData::EncodeData(void* marketDatas,int count)
{
  IndexSnapshot* items = (IndexSnapshot*)marketDatas;
  fastCodec->InitEncoder();
	for (int i=0;i<count;i++)
	{
    fastCodec->Check_Msg_Avail ();
		fastCodec->EncodeNewMsg(mainPMap,main_DefaultValue);	
		fastCodec->EncodeInt32(FAST_OP_COPY,0,items[i].data_time_stamp);
		fastCodec->EncodeStr(FAST_OP_NONE,1,(uint8_t*)items[i].wind_code,sizeof(items[i].wind_code));
		fastCodec->EncodeDouble(FAST_OP_DEFAULT,2,items[i].pre_close,4);
		fastCodec->EncodeDouble(FAST_OP_DEFAULT,3,items[i].open_price,4);
		fastCodec->EncodeDouble(FAST_OP_DEFAULT,4,items[i].high_price,4);
		fastCodec->EncodeDouble(FAST_OP_DEFAULT,5,items[i].low_price,4);
		fastCodec->EncodeDouble(FAST_OP_DEFAULT,6,items[i].new_price,4);
		fastCodec->EncodeInt64(FAST_OP_DEFAULT,7,items[i].total_volume);
		fastCodec->EncodeDouble(FAST_OP_DEFAULT,8,items[i].total_amount,5);		
		fastCodec->EncodeEndMsg(NULL,NULL);
		Reset_PMap();
	}
  ResetValueDict();
	return fastCodec->buffer.Get_Buffer_Used();
}

void* FastCodecIndexMarketData::DecodeData(uint8_t* bytes,int length,int& count)
{
	int capicity=DECODE_ARRAY_CAPICITY;
	int size =0;
	fastCodec->InitDecoder(bytes,length);
	IndexSnapshot* items=new IndexSnapshot[capicity];
	while(fastCodec->DecodeNewMsg(mainPMap,main_DefaultValue)>0)
	{
		if(size>=capicity)
		{
			IndexSnapshot* temp = new IndexSnapshot[capicity*2];
      memset(temp,0,sizeof(IndexSnapshot)*capicity*2);
			memcpy(temp,items,sizeof(IndexSnapshot)*capicity);
			capicity=capicity*2;
			delete[] items;
			items = temp;
		}
		IndexSnapshot& currentItem=items[size++];

		fastCodec->DecodeInt32(FAST_OP_COPY,0,currentItem.data_time_stamp);
		fastCodec->DecodeStr(FAST_OP_NONE,1,(uint8_t*)currentItem.wind_code,sizeof(currentItem.wind_code));		
		fastCodec->DecodeDouble(FAST_OP_DEFAULT,2,currentItem.pre_close,4);		
		fastCodec->DecodeDouble(FAST_OP_DEFAULT,3,currentItem.open_price,4);		
		fastCodec->DecodeDouble(FAST_OP_DEFAULT,4,currentItem.high_price,4);		
		fastCodec->DecodeDouble(FAST_OP_DEFAULT,5,currentItem.low_price,4);		
		fastCodec->DecodeDouble(FAST_OP_DEFAULT,6,currentItem.new_price,4);
		fastCodec->DecodeInt64(FAST_OP_DEFAULT,7,currentItem.total_volume);		 
		fastCodec->DecodeDouble(FAST_OP_DEFAULT,8,currentItem.total_amount ,5);
	 
		fastCodec->DecodeEndMsg(NULL,NULL);    
	}
	count = size;
  ResetValueDict();
	return (void*)items;
}


int32_t wmdf::FastCodecIndexMarketData::SizeOfRecords( void* items,int count )
{
  return sizeof(FuturesMarketSnapshot)*count;
}

void wmdf::FastCodecIndexMarketData::ResetValueDict()
{
  memset(main_DefaultValue,0,sizeof(struct tap_TValue)*INDEX_MARKETDATA_PMAP_BITS);
}
