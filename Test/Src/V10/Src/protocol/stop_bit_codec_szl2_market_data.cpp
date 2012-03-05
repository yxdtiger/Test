#include <iostream>
#include "../include/wmdf_structs.h"
#include "stop_bit_codec_szl2_market_data.h"


using namespace wmdf;

StopBitCodecSZL2MarketData::StopBitCodecSZL2MarketData()
{
  mainPMap=(Fast_PMap_T*)malloc(sizeof(struct Fast_PMap_T)+STOPBIT_SZL2_MARKETDATA_PMAP_BITS*sizeof(uint8_t));
  memset(mainPMap,0,sizeof(struct Fast_PMap_T)+STOPBIT_SZL2_MARKETDATA_PMAP_BITS*sizeof(uint8_t));
  mainPMap->size=STOPBIT_SZL2_MARKETDATA_PMAP_BITS;
  main_DefaultValue = (TValue*)calloc(sizeof(struct tap_TValue), STOPBIT_SZL2_MARKETDATA_PMAP_BITS);
}

StopBitCodecSZL2MarketData::~StopBitCodecSZL2MarketData()
{
}


int StopBitCodecSZL2MarketData::EncodeData(void* items,int count)
{
  L2MarketSnapshot* item = (L2MarketSnapshot*)items;
  fastCodec->InitEncoder();
  for (int i=0;i<count;i++)
  {
    fastCodec->Check_Msg_Avail ();
    //fastCodec->EncodeNewMsg(mainPMap,main_DefaultValue);

    fastCodec->EncodeInt32(FAST_OP_NONE,-1,item->trade_date);
    fastCodec->EncodeInt32(FAST_OP_NONE,-1,item->trade_time);
    fastCodec->EncodeStr(FAST_OP_NONE,-1,(uint8_t*)item->wind_code,sizeof(item->wind_code));
    fastCodec->EncodeDouble(FAST_OP_NONE,-1,item->prev_close,4);
    fastCodec->EncodeDouble(FAST_OP_NONE,-1,item->today_open,4);
    fastCodec->EncodeDouble(FAST_OP_NONE,-1,item->today_high,4);
    fastCodec->EncodeDouble(FAST_OP_NONE,-1,item->today_low,4);
    fastCodec->EncodeDouble(FAST_OP_NONE,-1,item->new_price,4);

    fastCodec->EncodeInt64(FAST_OP_NONE,-1,item->volume);
    fastCodec->EncodeDouble(FAST_OP_NONE,-1,item->total_amount,5);		
    fastCodec->EncodeInt64(FAST_OP_NONE,-1,item->transactions_count);
//    fastCodec->EncodeInt64(FAST_OP_NONE,-1,item->trading_phase_code);
   // fastCodec->EncodeStr(FAST_OP_NONE,-1,(uint8_t*)item->security_pre_name,4);
    //fastCodec->EncodeDouble(FAST_OP_NONE,-1,item->iopv,4);

    fastCodec->EncodeInt32(FAST_OP_NONE,-1,item->bid_volume1);
    fastCodec->EncodeDouble(FAST_OP_NONE,-1,item->bid_price1		,4);
    fastCodec->EncodeInt32(FAST_OP_NONE,-1,item->bid_volume2);
    fastCodec->EncodeDouble(FAST_OP_NONE,-1,item->bid_price2		,4);
    fastCodec->EncodeInt32(FAST_OP_NONE,-1,item->bid_volume3);
    fastCodec->EncodeDouble(FAST_OP_NONE,-1,item->bid_price3		,4);
    fastCodec->EncodeInt32(FAST_OP_NONE,-1,item->bid_volume4);
    fastCodec->EncodeDouble(FAST_OP_NONE,-1,item->bid_price4		,4);
    fastCodec->EncodeInt32(FAST_OP_NONE,-1,item->bid_volume5);
    fastCodec->EncodeDouble(FAST_OP_NONE,-1,item->bid_price5		,4);
    fastCodec->EncodeInt32(FAST_OP_NONE,-1,item->bid_volume6);
    fastCodec->EncodeDouble(FAST_OP_NONE,-1,item->bid_price6		,4);
    fastCodec->EncodeInt32(FAST_OP_NONE,-1,item->bid_volume7);
    fastCodec->EncodeDouble(FAST_OP_NONE,-1,item->bid_price7		,4);
    fastCodec->EncodeInt32(FAST_OP_NONE,-1,item->bid_volume8);
    fastCodec->EncodeDouble(FAST_OP_NONE,-1,item->bid_price8		,4);
    fastCodec->EncodeInt32(FAST_OP_NONE,-1,item->bid_volume9);
    fastCodec->EncodeDouble(FAST_OP_NONE,-1,item->bid_price9		,4);
    fastCodec->EncodeInt32(FAST_OP_NONE,-1,item->bid_volume10);
    fastCodec->EncodeDouble(FAST_OP_NONE,-1,item->bid_price10		,4);	


    fastCodec->EncodeInt32(FAST_OP_NONE,-1,item->ask_volume1);
    fastCodec->EncodeDouble(FAST_OP_NONE,-1,item->ask_price1		,4);
    fastCodec->EncodeInt32(FAST_OP_NONE,-1,item->ask_volume2);
    fastCodec->EncodeDouble(FAST_OP_NONE,-1,item->ask_price2	,4);
    fastCodec->EncodeInt32(FAST_OP_NONE,-1,item->ask_volume3);
    fastCodec->EncodeDouble(FAST_OP_NONE,-1,item->ask_price3 	,4);
    fastCodec->EncodeInt32(FAST_OP_NONE,-1,item->ask_volume4);
    fastCodec->EncodeDouble(FAST_OP_NONE,-1,item->ask_price4		,4);
    fastCodec->EncodeInt32(FAST_OP_NONE,-1,item->ask_volume5);
    fastCodec->EncodeDouble(FAST_OP_NONE,-1,item->ask_price5		,4);
    fastCodec->EncodeInt32(FAST_OP_NONE,-1,item->ask_volume6);
    fastCodec->EncodeDouble(FAST_OP_NONE,-1,item->ask_price6		,4);
    fastCodec->EncodeInt32(FAST_OP_NONE,-1,item->ask_volume7);
    fastCodec->EncodeDouble(FAST_OP_NONE,-1,item->ask_price7	,4);
    fastCodec->EncodeInt32(FAST_OP_NONE,-1,item->ask_volume8);
    fastCodec->EncodeDouble(FAST_OP_NONE,-1,item->ask_price8 	,4);
    fastCodec->EncodeInt32(FAST_OP_NONE,-1,item->ask_volume9);
    fastCodec->EncodeDouble(FAST_OP_NONE,-1,item->ask_price9		,4);
    fastCodec->EncodeInt32(FAST_OP_NONE,-1,item->ask_volume10);
    fastCodec->EncodeDouble(FAST_OP_NONE,-1,item->ask_price10	,4);			

    //fastCodec->EncodeEndMsg(NULL,NULL);
    //Reset_PMap();
    item+=1;
  }
  return fastCodec->buffer.Get_Buffer_Used();
}

void* StopBitCodecSZL2MarketData::DecodeData(uint8_t* bytes,int length,int& count)
{
  int tid=0,capicity=DECODE_ARRAY_CAPICITY;
  int size =0;
  int int32_value=WMDF_CONSTS_32BIT_NIL_VALUE;
  fastCodec->InitDecoder(bytes,length);
  L2MarketSnapshot* itemArray=new L2MarketSnapshot[capicity];
  memset(itemArray,0,sizeof(L2MarketSnapshot)*capicity);
  while(fastCodec->buffer.Get_Buffer_Left()>0)
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

    fastCodec->DecodeInt32(FAST_OP_NONE,-1,currentItem.trade_date);
    fastCodec->DecodeInt32(FAST_OP_NONE,-1,currentItem.trade_time);
    fastCodec->DecodeStr(FAST_OP_NONE,-1,(uint8_t*)currentItem.wind_code,sizeof(currentItem.wind_code));
    fastCodec->DecodeDouble(FAST_OP_NONE,-1,currentItem.prev_close,4);
    fastCodec->DecodeDouble(FAST_OP_NONE,-1,currentItem.today_open,4);    
    fastCodec->DecodeDouble(FAST_OP_NONE,-1,currentItem.today_high,4);
    fastCodec->DecodeDouble(FAST_OP_NONE,-1,currentItem.today_low,4);
    fastCodec->DecodeDouble(FAST_OP_NONE,-1,currentItem.new_price,4);
    fastCodec->DecodeInt64(FAST_OP_NONE,-1,currentItem.volume);
    fastCodec->DecodeDouble(FAST_OP_NONE,-1,currentItem.total_amount,5);
    fastCodec->DecodeInt64(FAST_OP_NONE,-1,currentItem.transactions_count);
   // fastCodec->DecodeInt64(FAST_OP_NONE,-1,currentItem.trading_phase_code);
    //fastCodec->DecodeStr(FAST_OP_NONE,-1,(uint8_t*)currentItem.security_pre_name,4);
    //fastCodec->DecodeDouble(FAST_OP_NONE,-1,currentItem.iopv,4);

    fastCodec->DecodeInt32(FAST_OP_NONE,-1,currentItem.bid_volume1);
    fastCodec->DecodeDouble(FAST_OP_NONE,-1,currentItem.bid_price1,4);
    fastCodec->DecodeInt32(FAST_OP_NONE,-1,currentItem.bid_volume2	);
    fastCodec->DecodeDouble(FAST_OP_NONE,-1,currentItem.bid_price2,4);
    fastCodec->DecodeInt32(FAST_OP_NONE,-1,currentItem.bid_volume3);
    fastCodec->DecodeDouble(FAST_OP_NONE,-1,currentItem.bid_price3,4);
    fastCodec->DecodeInt32(FAST_OP_NONE,-1,currentItem.bid_volume4);
    fastCodec->DecodeDouble(FAST_OP_NONE,-1,currentItem.bid_price4,4);
    fastCodec->DecodeInt32(FAST_OP_NONE,-1,currentItem.bid_volume5);
    fastCodec->DecodeDouble(FAST_OP_NONE,-1,currentItem.bid_price5,4);
    fastCodec->DecodeInt32(FAST_OP_NONE,-1,currentItem.bid_volume6);
    fastCodec->DecodeDouble(FAST_OP_NONE,-1,currentItem.bid_price6,4);
    fastCodec->DecodeInt32(FAST_OP_NONE,-1,currentItem.bid_volume7);
    fastCodec->DecodeDouble(FAST_OP_NONE,-1,currentItem.bid_price7,4);
    fastCodec->DecodeInt32(FAST_OP_NONE,-1,currentItem.bid_volume8);
    fastCodec->DecodeDouble(FAST_OP_NONE,-1,currentItem.bid_price8,4);
    fastCodec->DecodeInt32(FAST_OP_NONE,-1,currentItem.bid_volume9);
    fastCodec->DecodeDouble(FAST_OP_NONE,-1,currentItem.bid_price9,4);
    fastCodec->DecodeInt32(FAST_OP_NONE,-1,currentItem.bid_volume10);
    fastCodec->DecodeDouble(FAST_OP_NONE,-1,currentItem.bid_price10,4);	

    fastCodec->DecodeInt32(FAST_OP_NONE,-1,currentItem.ask_volume1);
    fastCodec->DecodeDouble(FAST_OP_NONE,-1,currentItem.ask_price1,4);
    fastCodec->DecodeInt32(FAST_OP_NONE,-1,currentItem.ask_volume2);
    fastCodec->DecodeDouble(FAST_OP_NONE,-1,currentItem.ask_price2,4);
    fastCodec->DecodeInt32(FAST_OP_NONE,-1,currentItem.ask_volume3);
    fastCodec->DecodeDouble(FAST_OP_NONE,-1,currentItem.ask_price3,4);
    fastCodec->DecodeInt32(FAST_OP_NONE,-1,currentItem.ask_volume4);
    fastCodec->DecodeDouble(FAST_OP_NONE,-1,currentItem.ask_price4,4);
    fastCodec->DecodeInt32(FAST_OP_NONE,-1,currentItem.ask_volume5);
    fastCodec->DecodeDouble(FAST_OP_NONE,-1,currentItem.ask_price5,4);	
    fastCodec->DecodeInt32(FAST_OP_NONE,-1,currentItem.ask_volume6);
    fastCodec->DecodeDouble(FAST_OP_NONE,-1,currentItem.ask_price6,4);
    fastCodec->DecodeInt32(FAST_OP_NONE,-1,currentItem.ask_volume7);
    fastCodec->DecodeDouble(FAST_OP_NONE,-1,currentItem.ask_price7,4);
    fastCodec->DecodeInt32(FAST_OP_NONE,-1,currentItem.ask_volume8);
    fastCodec->DecodeDouble(FAST_OP_NONE,-1,currentItem.ask_price8,4);
    fastCodec->DecodeInt32(FAST_OP_NONE,-1,currentItem.ask_volume9);
    fastCodec->DecodeDouble(FAST_OP_NONE,-1,currentItem.ask_price9,4);
    fastCodec->DecodeInt32(FAST_OP_NONE,-1,currentItem.ask_volume10);
    fastCodec->DecodeDouble(FAST_OP_NONE,-1,currentItem.ask_price10,4);	

    //fastCodec->DecodeEndMsg(NULL,NULL);
  }
  count = size;
  return (void*)itemArray;
}




int32_t wmdf::StopBitCodecSZL2MarketData::SizeOfRecords( void* items,int count )
{
  return sizeof(L2MarketSnapshot)*count;
}
