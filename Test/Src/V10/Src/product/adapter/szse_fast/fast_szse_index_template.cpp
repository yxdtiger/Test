#include "fast_szse_index_template.h"
#include "fast_szse_template_object.h"
#include "../common/rawdata_object.h"

void Reset( SZSEL2_Index_104& indexdata )
{
  indexdata.CloseIndex = WMDF_CONSTS_DOUBLE_NIL_VALUE;
  indexdata.DataTimeStamp = WMDF_CONSTS_32BIT_NIL_VALUE;
  memset(indexdata.EndOfDayMaker,0,sizeof(indexdata.EndOfDayMaker));
  indexdata.HighIndex = WMDF_CONSTS_DOUBLE_NIL_VALUE;
  indexdata.LastIndex = WMDF_CONSTS_DOUBLE_NIL_VALUE;
  indexdata.LowIndex = WMDF_CONSTS_DOUBLE_NIL_VALUE;
  indexdata.NumTrades = WMDF_CONSTS_64BIT_NIL_VALUE;
  indexdata.OpenIndex = WMDF_CONSTS_DOUBLE_NIL_VALUE;
  memset(indexdata.SecurityID,0,sizeof(indexdata.SecurityID));
  indexdata.TotalVolumeTraded = WMDF_CONSTS_64BIT_NIL_VALUE;
  indexdata.TurnOver = WMDF_CONSTS_DOUBLE_NIL_VALUE;
}

wmdf::FastSzseIndexTemplate::FastSzseIndexTemplate( void )
:FastTemplate(item_count_)
{
}

wmdf::FastSzseIndexTemplate::~FastSzseIndexTemplate( void )
{
  
}
wmdf::WindFastMessage* wmdf::FastSzseIndexTemplate::DecodeRaw(uint8_t* raw_data,uint32_t len)
{
  int data_status=0;
  int tid=0;
  fast_decoder_->ReSet((uint8_t*)raw_data,len);
  //current_index.DataTimeStamp
  fast_decoder_->SetDefaultValue(1);
  int capacity=4;
  int size=0;
  SZSEL2_Index_104* data_array = new SZSEL2_Index_104[capacity];//做整体分配的方法
	for(int32_t i=0;i!=capacity;++i)
	{
		Reset(data_array[i]);
	}
  while(!fast_decoder_->Empty())
  {
    fast_decoder_->BeginNewMessage();
    tid = fast_decoder_->DecodeUInt32(0,DEFAULT);
    assert(tid==104);
    // constant 变量不需要
    if(size>=capacity)
    {
      SZSEL2_Index_104* temp = new SZSEL2_Index_104[capacity*2];
      memcpy(temp,data_array,sizeof(SZSEL2_Index_104)*capacity);
      capacity=capacity*2;
      delete[] data_array;
      data_array = temp;
			for(int32_t i=size;i!=capacity;++i)
			{
				Reset(data_array[i]);
			}
    }
    //读取数据
    SZSEL2_Index_104& current_index=data_array[size++];

    current_index.DataTimeStamp=fast_decoder_->DecodeInt32(1,DELTA,PT_MANDATORY);
    fast_decoder_->DecodeString(2,NONE,current_index.SecurityID,sizeof(current_index.SecurityID));
    current_index.CloseIndex = Utils::Divide(fast_decoder_->DecodeUInt64(3,NONE),WMDF_CONSTS_TEN_THOUSAND);
    current_index.OpenIndex = Utils::Divide(fast_decoder_->DecodeUInt64(4,NONE),WMDF_CONSTS_TEN_THOUSAND);
    current_index.HighIndex = Utils::Divide(fast_decoder_->DecodeUInt64(5,NONE),WMDF_CONSTS_TEN_THOUSAND);
    current_index.LowIndex = Utils::Divide(fast_decoder_->DecodeUInt64(6,NONE),WMDF_CONSTS_TEN_THOUSAND);
    current_index.LastIndex = Utils::Divide(fast_decoder_->DecodeUInt64(7,NONE),WMDF_CONSTS_TEN_THOUSAND);
    current_index.TurnOver = Utils::Divide(fast_decoder_->DecodeUInt64(8,NONE),WMDF_CONSTS_ONE_THOUSAND);
    current_index.NumTrades = fast_decoder_->DecodeUInt64(9,NONE);
    current_index.TotalVolumeTraded = fast_decoder_->DecodeUInt64(10,NONE);
    fast_decoder_->DecodeString(11,COPY,current_index.EndOfDayMaker,sizeof(current_index.EndOfDayMaker));


    std::string windcode((char*)current_index.SecurityID); 
    std::map<std::string,int32_t>::iterator iter = time_map_.find(windcode);
    if(iter == time_map_.end())
    {
      time_map_.insert(make_pair(windcode,current_index.DataTimeStamp));
    }
    else
    {
      //特殊处理 925之前  及 11：31 到 13：00时段的数据
      if((iter->second == current_index.DataTimeStamp) || (current_index.DataTimeStamp<92500000))  
      {
        Reset(current_index);
        size--;
      }
      else if (current_index.DataTimeStamp >=113100000 && current_index.DataTimeStamp<130000000) 
      {
        Reset(current_index);
        size--;
      }
      else
      {
        iter->second=current_index.DataTimeStamp;
      }
    }
  }
  if (size > 0)
  {
    WindFastMessage* msg = new WindFastMessage();
    msg->MsgType = SZSE_INDEX_TYPE;
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

