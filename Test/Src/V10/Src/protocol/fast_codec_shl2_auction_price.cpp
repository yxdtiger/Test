#include <iostream>
#include "../include/wmdf_structs.h"
#include "fast_codec_shl2_auction_price.h"


using namespace wmdf;

FastCodecSHL2AuctionPrice::FastCodecSHL2AuctionPrice(void)
{
	mainPMap=(Fast_PMap_T*)malloc(sizeof(struct Fast_PMap_T)+SSEL2_AuctionPrice_PMAP_BITS*sizeof(uint8_t));
	memset(mainPMap,0,sizeof(struct Fast_PMap_T)+SSEL2_AuctionPrice_PMAP_BITS*sizeof(uint8_t));
	mainPMap->size=SSEL2_AuctionPrice_PMAP_BITS;
	main_DefaultValue = (TValue*)calloc(sizeof(struct tap_TValue), SSEL2_AuctionPrice_PMAP_BITS);
}

FastCodecSHL2AuctionPrice::~FastCodecSHL2AuctionPrice(void)
{
 
}


int FastCodecSHL2AuctionPrice::EncodeData(void* trans,int count)
{
  VirtualAuctionPrice* item = (VirtualAuctionPrice* )trans;
  fastCodec->InitEncoder();
	for (int i=0;i<count;i++)
	{
    fastCodec->Check_Msg_Avail ();
		fastCodec->EncodeNewMsg(mainPMap,main_DefaultValue);
		  
		fastCodec->EncodeStr(FAST_OP_COPY,0,(uint8_t*)item->wind_code,sizeof(item->wind_code));
		fastCodec->EncodeInt32(FAST_OP_COPY,1,item->date_time_stamp);
    fastCodec->EncodeDouble(FAST_OP_DEFAULT,2,item->price,4);
    fastCodec->EncodeInt32(FAST_OP_DEFAULT,3,item->virtual_auction_qty);
    fastCodec->EncodeInt32(FAST_OP_DEFAULT,4,item->leave_qty);
    fastCodec->EncodeInt32(FAST_OP_DEFAULT,5,item->side);
		fastCodec->EncodeEndMsg(NULL,NULL);
		Reset_PMap();
		item+=1;
	}
  ResetValueDict();
	return fastCodec->buffer.Get_Buffer_Used();
}

void* FastCodecSHL2AuctionPrice::DecodeData(uint8_t* bytes,int length,int& count)
{
	fastCodec->InitDecoder(bytes,length);

	int tid=0,capicity=DECODE_ARRAY_CAPICITY;
	int size =0;
	int int32_value=WMDF_CONSTS_32BIT_NIL_VALUE;
	VirtualAuctionPrice* itemArray=new VirtualAuctionPrice[capicity];
	while(fastCodec->DecodeNewMsg(mainPMap,main_DefaultValue)>0)
	{	
		if(size>=capicity)
		{
			VirtualAuctionPrice* temp = new VirtualAuctionPrice[capicity*2];
      memset(temp,0,sizeof(VirtualAuctionPrice)*capicity*2);
			memcpy(temp,itemArray,sizeof(VirtualAuctionPrice)*capicity);
			capicity=capicity*2;
			delete[] itemArray;
			itemArray = temp;
		}
		VirtualAuctionPrice& currentItem=itemArray[size++];

		fastCodec->DecodeStr(FAST_OP_COPY,0,(uint8_t*)currentItem.wind_code,sizeof(currentItem.wind_code));
    fastCodec->DecodeInt32(FAST_OP_COPY,1,currentItem.date_time_stamp);
    fastCodec->DecodeDouble(FAST_OP_DEFAULT,2,currentItem.price,4);
		fastCodec->DecodeInt32(FAST_OP_DEFAULT,3,currentItem.virtual_auction_qty);		
    fastCodec->DecodeInt32(FAST_OP_DEFAULT,4,currentItem.leave_qty);
    fastCodec->DecodeInt32(FAST_OP_DEFAULT,5,int32_value);		
    currentItem.side = (uint8_t)int32_value;
		fastCodec->DecodeEndMsg(NULL,NULL);    
	}
	count = size;
  ResetValueDict();
	return (void*)itemArray;
}




int32_t wmdf::FastCodecSHL2AuctionPrice::SizeOfRecords( void* items,int count )
{
  return count*sizeof(VirtualAuctionPrice);
}

void wmdf::FastCodecSHL2AuctionPrice::ResetValueDict()
{
  memset(main_DefaultValue,0,sizeof(struct tap_TValue)*SSEL2_AuctionPrice_PMAP_BITS);
}
