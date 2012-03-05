#include "sse_fast_indexdata_3113_template.h"
#include "../common/rawdata_object.h"


wmdf::SSEFastIndexData3113Template::SSEFastIndexData3113Template()
:FastTemplate(item_count_)
{

}

wmdf::SSEFastIndexData3113Template::~SSEFastIndexData3113Template()
{

}

wmdf::WindFastMessage* wmdf::SSEFastIndexData3113Template::DecodeRaw( uint8_t* raw_data,uint32_t len )
{
	int data_status=0,data_time_stamp=0;
	int tid=0;
	fast_decoder_->ReSet(raw_data,len);
	int capacity=128;
	int size=0;
	uint8_t palce_str[20];
	uint64_t temp=0;
	IndexData* data_array = new IndexData[capacity];
	for(int32_t i=0;i!=capacity;++i)
	{
		data_array[i].DataStatus = WMDF_CONSTS_32BIT_NIL_VALUE;
		data_array[i].DataTimeStamp = WMDF_CONSTS_32BIT_NIL_VALUE;
		data_array[i].HighPx = WMDF_CONSTS_DOUBLE_NIL_VALUE;
		data_array[i].LastPx = WMDF_CONSTS_DOUBLE_NIL_VALUE;
		data_array[i].LowPx = WMDF_CONSTS_DOUBLE_NIL_VALUE;
		data_array[i].OpenPx = WMDF_CONSTS_DOUBLE_NIL_VALUE;
		data_array[i].PreClosePx = WMDF_CONSTS_DOUBLE_NIL_VALUE;
		memset(data_array[i].SecurityID,0,sizeof(data_array->SecurityID));
		data_array[i].total_amount = WMDF_CONSTS_DOUBLE_NIL_VALUE;
		data_array[i].transactions_count = WMDF_CONSTS_64BIT_NIL_VALUE;
		data_array[i].volume = WMDF_CONSTS_64BIT_NIL_VALUE;
	}
	while(!fast_decoder_->Empty())
	{
		fast_decoder_->BeginNewMessage();
		tid = fast_decoder_->DecodeUInt32(0,DEFAULT);
		assert(tid==SSE_INDEXDATA_3113_TYPE);
		data_time_stamp = fast_decoder_->DecodeInt32(1,COPY,PT_MANDATORY);
		data_status = fast_decoder_->DecodeInt32(2,DEFAULT);
		if(data_status<=0)
		{
			if(size>=capacity)
			{
				IndexData* temp = new IndexData[capacity*2];
				memcpy(temp,data_array,sizeof(IndexData)*capacity);
				capacity = capacity*2;
				delete[] data_array;
				data_array = temp;
				for(int32_t i=size;i!=capacity;++i)
				{
					data_array[i].DataStatus = WMDF_CONSTS_32BIT_NIL_VALUE;
					data_array[i].DataTimeStamp = WMDF_CONSTS_32BIT_NIL_VALUE;
					data_array[i].HighPx = WMDF_CONSTS_DOUBLE_NIL_VALUE;
					data_array[i].LastPx = WMDF_CONSTS_DOUBLE_NIL_VALUE;
					data_array[i].LowPx = WMDF_CONSTS_DOUBLE_NIL_VALUE;
					data_array[i].OpenPx = WMDF_CONSTS_DOUBLE_NIL_VALUE;
					data_array[i].PreClosePx = WMDF_CONSTS_DOUBLE_NIL_VALUE;
					memset(data_array[i].SecurityID,0,sizeof(data_array->SecurityID));
					data_array[i].total_amount = WMDF_CONSTS_DOUBLE_NIL_VALUE;
					data_array[i].transactions_count = WMDF_CONSTS_64BIT_NIL_VALUE;
					data_array[i].volume = WMDF_CONSTS_64BIT_NIL_VALUE;
				}
			}

			IndexData& curr_index_data = data_array[size++];
			curr_index_data.DataTimeStamp = data_time_stamp;
			fast_decoder_->DecodeString(3,NONE,(uint8_t*)curr_index_data.SecurityID,sizeof(curr_index_data.SecurityID));
			int64_t preclose = fast_decoder_->DecodeInt64(4,DEFAULT);
      curr_index_data.PreClosePx = Utils::Divide(preclose,WMDF_CONSTS_HUNDRED_THOUSAND);

			int64_t openpx = fast_decoder_->DecodeInt64(5,DEFAULT);
			curr_index_data.OpenPx = Utils::Divide(openpx,WMDF_CONSTS_HUNDRED_THOUSAND);

			int64_t total_amount = fast_decoder_->DecodeInt64(6,DEFAULT);
			curr_index_data.total_amount = Utils::Divide(total_amount,WMDF_CONSTS_TEN);

			int64_t highpx = fast_decoder_->DecodeInt64(7,DEFAULT);
			curr_index_data.HighPx = Utils::Divide(highpx,WMDF_CONSTS_HUNDRED_THOUSAND);
			
			int64_t lowpx = fast_decoder_->DecodeInt64(8,DEFAULT);
			curr_index_data.LowPx = Utils::Divide(lowpx,WMDF_CONSTS_HUNDRED_THOUSAND);

			int64_t lastpx = fast_decoder_->DecodeInt64(9,DEFAULT);
			curr_index_data.LastPx = Utils::Divide(lastpx,WMDF_CONSTS_HUNDRED_THOUSAND);

			int32_t tradetime = fast_decoder_->DecodeInt32(10,DEFAULT);

      int64_t volume = fast_decoder_->DecodeInt64(11,DEFAULT);
			curr_index_data.volume = Utils::DivideEx(volume,WMDF_CONSTS_HUNDRED_THOUSAND);

			int64_t close_index = fast_decoder_->DecodeInt64(12,DEFAULT);
			if(close_index!=WMDF_CONSTS_64BIT_NIL_VALUE && curr_index_data.LastPx==WMDF_CONSTS_DOUBLE_NIL_VALUE)
			{
				curr_index_data.LastPx = (double)close_index;
			}
		}
		else
		{
			fast_decoder_->DecodeString(3,NONE,palce_str,sizeof(palce_str));
			fast_decoder_->DecodeInt64(4,DEFAULT);
			fast_decoder_->DecodeInt64(5,DEFAULT);
			fast_decoder_->DecodeInt64(6,DEFAULT);
			fast_decoder_->DecodeInt64(7,DEFAULT);
			fast_decoder_->DecodeInt64(8,DEFAULT);
			fast_decoder_->DecodeInt64(9,DEFAULT);
			fast_decoder_->DecodeInt32(10,DEFAULT);
			fast_decoder_->DecodeInt64(11,DEFAULT);
		}
	}
  if (size > 0)
  {
    WindFastMessage* msg = new WindFastMessage();
    msg->MsgType = SSE_INDEXDATA_3113_TYPE;
    msg->Size=size;
    msg->Data = data_array;
    return	msg;
  }
  else
  {
    delete data_array;
  }
  return NULL;
}
