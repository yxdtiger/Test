#include "fast_szse_snapshot_template.h"
#include "../common/rawdata_object.h"
#include "fast_szse_template_object.h"
#include <iostream>

void Reset( stTemplate_Snapshot_103& snapshotdata )
{
  snapshotdata.DataTimeStamp = WMDF_CONSTS_32BIT_NIL_VALUE;
  memset(snapshotdata.EndOfDayMaker,0,sizeof(snapshotdata.EndOfDayMaker));
  snapshotdata.HighPx = WMDF_CONSTS_DOUBLE_NIL_VALUE;
  snapshotdata.LastPx = WMDF_CONSTS_DOUBLE_NIL_VALUE;
  snapshotdata.LowPx = WMDF_CONSTS_DOUBLE_NIL_VALUE;
  snapshotdata.nBidPriceLevelNum = WMDF_CONSTS_32BIT_NIL_VALUE;
  snapshotdata.nOfferPriceLevelNum = WMDF_CONSTS_32BIT_NIL_VALUE;
  snapshotdata.NumTrades = WMDF_CONSTS_64BIT_NIL_VALUE;
  snapshotdata.OpenPx = WMDF_CONSTS_DOUBLE_NIL_VALUE;
  snapshotdata.pBidPriceLevel = NULL;
  snapshotdata.PERatio1 = WMDF_CONSTS_DOUBLE_NIL_VALUE;
  snapshotdata.PERatio2 = WMDF_CONSTS_DOUBLE_NIL_VALUE;
  snapshotdata.pOfferPriceLevel = NULL;
  snapshotdata.PreClosePx = WMDF_CONSTS_DOUBLE_NIL_VALUE;
  memset(snapshotdata.SecurityID,0,sizeof(snapshotdata.SecurityID));
  snapshotdata.TotalBidQty = WMDF_CONSTS_64BIT_NIL_VALUE;
  snapshotdata.TotalLongPosition = WMDF_CONSTS_64BIT_NIL_VALUE;
  snapshotdata.TotalOfferQty = WMDF_CONSTS_64BIT_NIL_VALUE;
  snapshotdata.TotalValueTrade = WMDF_CONSTS_DOUBLE_NIL_VALUE;
  snapshotdata.TotalVolumeTrade = WMDF_CONSTS_64BIT_NIL_VALUE;
  snapshotdata.WeightedAvgBidPx = WMDF_CONSTS_DOUBLE_NIL_VALUE;
  snapshotdata.WeightedAvgOfferPx = WMDF_CONSTS_DOUBLE_NIL_VALUE;
  snapshotdata.pBidPriceLevel = NULL;
  snapshotdata.pOfferPriceLevel = NULL;
}

void ReleasePriceLevel( ptrBidOfferPriceLevel_103 price_level,int32_t count )
{
  if (NULL != price_level)
  {
    for (int32_t index=0; index<count; index++)
    {
      if(price_level[index].nSequenceLength>0)
        delete[] price_level[index].pChildSequence;
    }
    delete[] price_level;
  }
}

wmdf::FastSzseSnapshotTemplate::FastSzseSnapshotTemplate( void )
:FastTemplate(item_count_)
{
}

wmdf::FastSzseSnapshotTemplate::~FastSzseSnapshotTemplate( void )
{
	for(map<string,CompareStruct*>::iterator iter=compare_map_.begin();iter!=compare_map_.end();++iter)
	{
		CompareStruct *temp = iter->second;
		delete temp;
		temp = NULL;
	}
	compare_map_.clear();
}

wmdf::WindFastMessage* wmdf::FastSzseSnapshotTemplate::DecodeRaw(uint8_t* raw_data,uint32_t len)
{
  int32_t data_status=0;
  int32_t tid=0;
  fast_decoder_->ReSet((uint8_t*)raw_data,len);
  //current_snapshot.DataTimeStamp
  fast_decoder_->SetDefaultValue(1);
  int32_t capacity=4;
  int32_t size=0;
  uint32_t  tmp_squene_count=0;
  stTemplate_Snapshot_103* data_array = new stTemplate_Snapshot_103[capacity];//做整体分配的方法	
	for(int32_t i=0;i!=capacity;++i)
	{
		Reset(data_array[i]);
	}
  while(!fast_decoder_->Empty())
  {
    fast_decoder_->BeginNewMessage();
    tid = fast_decoder_->DecodeUInt32(0,DEFAULT);
    assert(tid==103);
    // constant 变量不需要
    if(size>=capacity)
    {
      stTemplate_Snapshot_103* temp = new stTemplate_Snapshot_103[capacity*2];
      memcpy(temp,data_array,sizeof(stTemplate_Snapshot_103)*capacity);
      capacity=capacity*2;
      delete[] data_array;
      data_array = temp;
			for(int32_t i=size;i!=capacity;++i)
			{
				Reset(data_array[i]);
			}
    }
    //读取数据
    stTemplate_Snapshot_103& current_snapshot=data_array[size++];
    current_snapshot.DataTimeStamp = fast_decoder_->DecodeInt32(1,DELTA,PT_MANDATORY);
    
    fast_decoder_->DecodeString(2,NONE,current_snapshot.SecurityID,sizeof(current_snapshot.SecurityID));

    uint32_t preclose_px = fast_decoder_->DecodeUInt32(3,NONE);
    if (preclose_px > 0)
    {
      current_snapshot.PreClosePx= Utils::Divide(preclose_px,WMDF_CONSTS_ONE_THOUSAND);
    }

    uint32_t open_px = fast_decoder_->DecodeUInt32(4,NONE);
    if (open_px > 0)
    {
      current_snapshot.OpenPx= Utils::Divide(open_px,WMDF_CONSTS_ONE_THOUSAND);
    }
    uint32_t high_px = fast_decoder_->DecodeUInt32(5,NONE);
    if (high_px > 0)
    {
      current_snapshot.HighPx= Utils::Divide(high_px,WMDF_CONSTS_ONE_THOUSAND);
    }
    uint32_t low_px = fast_decoder_->DecodeUInt32(6,NONE);


    if (low_px != 999999999 )
    {
      current_snapshot.LowPx= Utils::Divide(low_px,WMDF_CONSTS_ONE_THOUSAND);
    }
    
    uint32_t last_px = fast_decoder_->DecodeUInt32(7,NONE);
    if (last_px > 0)
    {
      current_snapshot.LastPx= Utils::Divide(last_px,WMDF_CONSTS_ONE_THOUSAND);
    }
    current_snapshot.NumTrades= fast_decoder_->DecodeUInt64(8,NONE);
    current_snapshot.TotalVolumeTrade= fast_decoder_->DecodeUInt64(9,NONE);
    current_snapshot.TotalValueTrade= Utils::Divide(fast_decoder_->DecodeUInt64(10,NONE),WMDF_CONSTS_ONE_THOUSAND);
    current_snapshot.TotalLongPosition= fast_decoder_->DecodeUInt64(11,NONE);
    current_snapshot.PERatio1= Utils::Divide(fast_decoder_->DecodeUInt32(12,NONE),WMDF_CONSTS_ONE_HUNDRED);
    current_snapshot.PERatio2= Utils::Divide(fast_decoder_->DecodeUInt32(13,NONE),WMDF_CONSTS_ONE_HUNDRED);
    fast_decoder_->DecodeString(14,COPY,current_snapshot.EndOfDayMaker,sizeof(current_snapshot.EndOfDayMaker));
    current_snapshot.TotalOfferQty= fast_decoder_->DecodeUInt64(15,NONE);
    current_snapshot.WeightedAvgOfferPx= Utils::Divide(fast_decoder_->DecodeUInt64(16,NONE),WMDF_CONSTS_ONE_THOUSAND);




    tmp_squene_count = fast_decoder_->DecodeInt32(17,NONE);
    current_snapshot.nOfferPriceLevelNum= tmp_squene_count;
    if(tmp_squene_count>0)
    {
      current_snapshot.pOfferPriceLevel=new stBidOfferPriceLevel_103[tmp_squene_count];
      for(uint32_t i=0;i<tmp_squene_count;i++)
      {
        ptrBidOfferPriceLevel_103 tmp_price_level = &current_snapshot.pOfferPriceLevel[i];
        tmp_price_level->BidorOfferPx = Utils::Divide(fast_decoder_->DecodeUInt32(0,NONE),WMDF_CONSTS_ONE_THOUSAND);
        tmp_price_level->BidorOfferSize = fast_decoder_->DecodeUInt64(1,NONE);
        tmp_price_level->NumOrders = fast_decoder_->DecodeUInt32(2,NONE);
        uint32_t tmp_squene_count2 = fast_decoder_->DecodeInt32(3,NONE);
        tmp_price_level->nSequenceLength = tmp_squene_count2;
        if(tmp_squene_count2>0)
        {
          tmp_price_level->pChildSequence=new stOrders_103[tmp_squene_count2];
          for(uint32_t j=0;j<tmp_squene_count2;j++)
            tmp_price_level->pChildSequence[j].OrderQty = (int32_t)fast_decoder_->DecodeUInt64(0,NONE);
        }
      }
    }

    current_snapshot.TotalBidQty= fast_decoder_->DecodeUInt64(18,NONE);
    current_snapshot.WeightedAvgBidPx= Utils::Divide(fast_decoder_->DecodeUInt64(19,NONE),WMDF_CONSTS_ONE_THOUSAND);
    tmp_squene_count = fast_decoder_->DecodeInt32(20,NONE);
    current_snapshot.nBidPriceLevelNum= tmp_squene_count;
    if(tmp_squene_count>0)
    {
      current_snapshot.pBidPriceLevel=new stBidOfferPriceLevel_103[tmp_squene_count];
      for(uint32_t i=0;i<tmp_squene_count;i++)
      {
        ptrBidOfferPriceLevel_103 tmp_price_level = &current_snapshot.pBidPriceLevel[i];
        tmp_price_level->BidorOfferPx = Utils::Divide(fast_decoder_->DecodeUInt32(0,NONE),WMDF_CONSTS_ONE_THOUSAND);
        tmp_price_level->BidorOfferSize = fast_decoder_->DecodeUInt64(1,NONE);
        tmp_price_level->NumOrders = fast_decoder_->DecodeUInt32(2,NONE);
        uint32_t tmp_squene_count2 = fast_decoder_->DecodeInt32(3,NONE);
        tmp_price_level->nSequenceLength = tmp_squene_count2;
        if(tmp_squene_count2>0)
        {
          tmp_price_level->pChildSequence=new stOrders_103[tmp_squene_count2];
          for(uint32_t j=0;j<tmp_squene_count2;j++)
            tmp_price_level->pChildSequence[j].OrderQty = (int32_t)fast_decoder_->DecodeUInt64(0,NONE);
        }
      }
    }

		string windcode((char*)current_snapshot.SecurityID);
		map<string,CompareStruct*>::iterator iter = compare_map_.find(windcode);
		if(iter == compare_map_.end())
		{
			CompareStruct *curr_data = new CompareStruct();
			curr_data->trade_time = current_snapshot.DataTimeStamp;
			curr_data->new_price = current_snapshot.LastPx;
			curr_data->volume = current_snapshot.TotalVolumeTrade;
			curr_data->peratio1 = current_snapshot.PERatio1;
			curr_data->peratio2 = current_snapshot.PERatio2;
			compare_map_.insert(make_pair(windcode,curr_data));
		}
		else
		{
			CompareStruct *last_data = iter->second;
			if(current_snapshot.DataTimeStamp != last_data->trade_time
				||  current_snapshot.LastPx != last_data->new_price
				|| current_snapshot.TotalVolumeTrade != last_data->volume
				||  current_snapshot.PERatio1 != last_data->peratio1
				|| current_snapshot.PERatio2 != last_data->peratio2)
			{
				last_data->trade_time = current_snapshot.DataTimeStamp;
				last_data->new_price = current_snapshot.LastPx;
				last_data->volume = current_snapshot.TotalVolumeTrade;
				last_data->peratio1 = current_snapshot.PERatio1;
				last_data->peratio2 = current_snapshot.PERatio2;
			}
			else
			{
				ReleasePriceLevel(current_snapshot.pBidPriceLevel,current_snapshot.nBidPriceLevelNum);
				ReleasePriceLevel(current_snapshot.pOfferPriceLevel,current_snapshot.nOfferPriceLevelNum);
				Reset(current_snapshot);
				size--;
			}
		}

  }
  if (size > 0)
  {
    WindFastMessage* msg = new WindFastMessage();
    msg->MsgType = SZSE_SNAPSHOT_TYPE;
    msg->Size=size;
    msg->Data = data_array;
    return msg;
  }
  else
  {
    delete data_array;
  }
  return NULL;
}
