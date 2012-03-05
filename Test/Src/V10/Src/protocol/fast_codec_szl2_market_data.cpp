#include <iostream>
#include <assert.h>
#include "../include/wmdf_structs.h"
#include "fast_codec_szl2_market_data.h"


using namespace wmdf;

FastCodecSZL2MarketData::FastCodecSZL2MarketData()
{
  mainPMap=(Fast_PMap_T*)malloc(sizeof(struct Fast_PMap_T)+SZL2_MARKETDATA_PMAP_BITS*sizeof(uint8_t));
  memset(mainPMap,0,sizeof(struct Fast_PMap_T)+SZL2_MARKETDATA_PMAP_BITS*sizeof(uint8_t));
  mainPMap->size=SZL2_MARKETDATA_PMAP_BITS;

  main_DefaultValue = (TValue*)calloc(sizeof(struct tap_TValue), SZL2_MARKETDATA_PMAP_BITS);
}

FastCodecSZL2MarketData::~FastCodecSZL2MarketData()
{
}


int FastCodecSZL2MarketData::EncodeData(void* items,int count)
{
  L2MarketSnapshot* item = (L2MarketSnapshot*)items;
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
    fastCodec->EncodeInt64(FAST_OP_DEFAULT,10,item->transactions_count);

    fastCodec->EncodeInt32(FAST_OP_DEFAULT,11,item->bid_size1);
    fastCodec->EncodeInt32(FAST_OP_DEFAULT,12,item->bid_size2);
    fastCodec->EncodeInt32(FAST_OP_DEFAULT,13,item->bid_size3);
    fastCodec->EncodeInt32(FAST_OP_DEFAULT,14,item->bid_size4);
    fastCodec->EncodeInt32(FAST_OP_DEFAULT,15,item->bid_size5);
    fastCodec->EncodeInt32(FAST_OP_DEFAULT,16,item->bid_size6);
    fastCodec->EncodeInt32(FAST_OP_DEFAULT,17,item->bid_size7);
    fastCodec->EncodeInt32(FAST_OP_DEFAULT,18,item->bid_size8);
    fastCodec->EncodeInt32(FAST_OP_DEFAULT,19,item->bid_size9);
    fastCodec->EncodeInt32(FAST_OP_DEFAULT,20,item->bid_size10);

    fastCodec->EncodeDouble(FAST_OP_DEFAULT,21,item->bid_price1		,4);      
    fastCodec->EncodeDouble(FAST_OP_DEFAULT,22,item->bid_price2		,4);
    fastCodec->EncodeDouble(FAST_OP_DEFAULT,23,item->bid_price3		,4);
    fastCodec->EncodeDouble(FAST_OP_DEFAULT,24,item->bid_price4		,4);
    fastCodec->EncodeDouble(FAST_OP_DEFAULT,25,item->bid_price5		,4);
    fastCodec->EncodeDouble(FAST_OP_DEFAULT,26,item->bid_price6		,4);
    fastCodec->EncodeDouble(FAST_OP_DEFAULT,27,item->bid_price7		,4);
    fastCodec->EncodeDouble(FAST_OP_DEFAULT,28,item->bid_price8		,4);
    fastCodec->EncodeDouble(FAST_OP_DEFAULT,29,item->bid_price9		,4);
    fastCodec->EncodeDouble(FAST_OP_DEFAULT,30,item->bid_price10		,4);

    fastCodec->EncodeInt32(FAST_OP_DEFAULT,31,item->ask_size1);
    fastCodec->EncodeInt32(FAST_OP_DEFAULT,32,item->ask_size2);
    fastCodec->EncodeInt32(FAST_OP_DEFAULT,33,item->ask_size3);
    fastCodec->EncodeInt32(FAST_OP_DEFAULT,34,item->ask_size4);
    fastCodec->EncodeInt32(FAST_OP_DEFAULT,35,item->ask_size5);
    fastCodec->EncodeInt32(FAST_OP_DEFAULT,36,item->ask_size6);
    fastCodec->EncodeInt32(FAST_OP_DEFAULT,37,item->ask_size7);
    fastCodec->EncodeInt32(FAST_OP_DEFAULT,38,item->ask_size8);
    fastCodec->EncodeInt32(FAST_OP_DEFAULT,39,item->ask_size9);
    fastCodec->EncodeInt32(FAST_OP_DEFAULT,40,item->ask_size10);

    fastCodec->EncodeDouble(FAST_OP_DEFAULT,41,item->ask_price1		,4);
    fastCodec->EncodeDouble(FAST_OP_DEFAULT,42,item->ask_price2	,4);
    fastCodec->EncodeDouble(FAST_OP_DEFAULT,43,item->ask_price3 	,4);
    fastCodec->EncodeDouble(FAST_OP_DEFAULT,44,item->ask_price4		,4);
    fastCodec->EncodeDouble(FAST_OP_DEFAULT,45,item->ask_price5		,4);
    fastCodec->EncodeDouble(FAST_OP_DEFAULT,46,item->ask_price6		,4);
    fastCodec->EncodeDouble(FAST_OP_DEFAULT,47,item->ask_price7	,4);
    fastCodec->EncodeDouble(FAST_OP_DEFAULT,48,item->ask_price8 	,4);
    fastCodec->EncodeDouble(FAST_OP_DEFAULT,49,item->ask_price9		,4);
    fastCodec->EncodeDouble(FAST_OP_DEFAULT,50,item->ask_price10	,4);

    fastCodec->EncodeEndMsg(NULL,NULL);
    Reset_PMap();
    item+=1;
  }
  ResetValueDict();
  return fastCodec->buffer.Get_Buffer_Used();
}

void* FastCodecSZL2MarketData::DecodeData(uint8_t* bytes,int length,int& count)
{
  int capicity=DECODE_ARRAY_CAPICITY;
  int size =0;
  int int32_value=WMDF_CONSTS_32BIT_NIL_VALUE;
  fastCodec->InitDecoder(bytes,length);
  L2MarketSnapshot* itemArray=new L2MarketSnapshot[capicity];
  memset(itemArray,0,sizeof(L2MarketSnapshot)*capicity);
  while(fastCodec->DecodeNewMsg(mainPMap,main_DefaultValue)>0)
  {
    if(size>=capicity)
    {
      L2MarketSnapshot* temp = new L2MarketSnapshot[capicity*2];
      memset(temp,0,sizeof(L2MarketSnapshot)*capicity*2);
      memcpy(temp,itemArray,sizeof(L2MarketSnapshot)*capicity);
      capicity=capicity*2;
      delete[] itemArray;
      itemArray = temp;
    }
    L2MarketSnapshot& currentItem=itemArray[size++];
    currentItem.weighted_average_ask_price = WMDF_CONSTS_DOUBLE_NIL_VALUE;
    currentItem.weighted_average_bid_price = WMDF_CONSTS_DOUBLE_NIL_VALUE;
    currentItem.total_bid_size = WMDF_CONSTS_64BIT_NIL_VALUE;
    currentItem.total_ask_size = WMDF_CONSTS_64BIT_NIL_VALUE;

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
    fastCodec->DecodeInt64(FAST_OP_DEFAULT,10,currentItem.transactions_count);  

    fastCodec->DecodeInt32(FAST_OP_DEFAULT,11,currentItem.bid_size1);
    fastCodec->DecodeInt32(FAST_OP_DEFAULT,12,currentItem.bid_size2	);
    fastCodec->DecodeInt32(FAST_OP_DEFAULT,13,currentItem.bid_size3);
    fastCodec->DecodeInt32(FAST_OP_DEFAULT,14,currentItem.bid_size4);
    fastCodec->DecodeInt32(FAST_OP_DEFAULT,15,currentItem.bid_size5);
    fastCodec->DecodeInt32(FAST_OP_DEFAULT,16,currentItem.bid_size6);
    fastCodec->DecodeInt32(FAST_OP_DEFAULT,17,currentItem.bid_size7);
    fastCodec->DecodeInt32(FAST_OP_DEFAULT,18,currentItem.bid_size8);
    fastCodec->DecodeInt32(FAST_OP_DEFAULT,19,currentItem.bid_size9);
    fastCodec->DecodeInt32(FAST_OP_DEFAULT,20,currentItem.bid_size10);

    fastCodec->DecodeDouble(FAST_OP_DEFAULT,21,currentItem.bid_price1,4);      
    fastCodec->DecodeDouble(FAST_OP_DEFAULT,22,currentItem.bid_price2,4);      
    fastCodec->DecodeDouble(FAST_OP_DEFAULT,23,currentItem.bid_price3,4);
    fastCodec->DecodeDouble(FAST_OP_DEFAULT,24,currentItem.bid_price4,4);      
    fastCodec->DecodeDouble(FAST_OP_DEFAULT,25,currentItem.bid_price5,4);      
    fastCodec->DecodeDouble(FAST_OP_DEFAULT,26,currentItem.bid_price6,4);      
    fastCodec->DecodeDouble(FAST_OP_DEFAULT,27,currentItem.bid_price7,4);      
    fastCodec->DecodeDouble(FAST_OP_DEFAULT,28,currentItem.bid_price8,4);      
    fastCodec->DecodeDouble(FAST_OP_DEFAULT,29,currentItem.bid_price9,4);      
    fastCodec->DecodeDouble(FAST_OP_DEFAULT,30,currentItem.bid_price10,4);	

    fastCodec->DecodeInt32(FAST_OP_DEFAULT,31,currentItem.ask_size1);
    fastCodec->DecodeInt32(FAST_OP_DEFAULT,32,currentItem.ask_size2);
    fastCodec->DecodeInt32(FAST_OP_DEFAULT,33,currentItem.ask_size3);
    fastCodec->DecodeInt32(FAST_OP_DEFAULT,34,currentItem.ask_size4);
    fastCodec->DecodeInt32(FAST_OP_DEFAULT,35,currentItem.ask_size5);
    fastCodec->DecodeInt32(FAST_OP_DEFAULT,36,currentItem.ask_size6);
    fastCodec->DecodeInt32(FAST_OP_DEFAULT,37,currentItem.ask_size7);
    fastCodec->DecodeInt32(FAST_OP_DEFAULT,38,currentItem.ask_size8);
    fastCodec->DecodeInt32(FAST_OP_DEFAULT,39,currentItem.ask_size9);
    fastCodec->DecodeInt32(FAST_OP_DEFAULT,40,currentItem.ask_size10);

    fastCodec->DecodeDouble(FAST_OP_DEFAULT,41,currentItem.ask_price1,4);      
    fastCodec->DecodeDouble(FAST_OP_DEFAULT,42,currentItem.ask_price2,4);      
    fastCodec->DecodeDouble(FAST_OP_DEFAULT,43,currentItem.ask_price3,4);      
    fastCodec->DecodeDouble(FAST_OP_DEFAULT,44,currentItem.ask_price4,4);
    fastCodec->DecodeDouble(FAST_OP_DEFAULT,45,currentItem.ask_price5,4);	
    fastCodec->DecodeDouble(FAST_OP_DEFAULT,46,currentItem.ask_price6,4);
    fastCodec->DecodeDouble(FAST_OP_DEFAULT,47,currentItem.ask_price7,4);
    fastCodec->DecodeDouble(FAST_OP_DEFAULT,48,currentItem.ask_price8,4);
    fastCodec->DecodeDouble(FAST_OP_DEFAULT,49,currentItem.ask_price9,4);
    fastCodec->DecodeDouble(FAST_OP_DEFAULT,50,currentItem.ask_price10,4);	


    fastCodec->DecodeEndMsg(NULL,NULL);
  }
  count = size;
  ResetValueDict();
  return (void*)itemArray;
}




int32_t wmdf::FastCodecSZL2MarketData::SizeOfRecords( void* items,int count )
{
  return sizeof(L2MarketSnapshot)*count;
}

void wmdf::FastCodecSZL2MarketData::ResetValueDict()
{
  memset(main_DefaultValue,0,sizeof(struct tap_TValue)*SZL2_MARKETDATA_PMAP_BITS);
}
