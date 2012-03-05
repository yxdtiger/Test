#include "sse_fast_indexdata_1113_template.h"
#include "../common/rawdata_object.h"


wmdf::SSEFastIndexData1113Template::SSEFastIndexData1113Template()
:FastTemplate(item_count_)
{
}

wmdf::SSEFastIndexData1113Template::~SSEFastIndexData1113Template()
{

}

wmdf::WindFastMessage* wmdf::SSEFastIndexData1113Template::DecodeRaw(uint8_t* raw_data,uint32_t len)
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
		assert(tid==SSE_INDEXDATA_1113_TYPE);
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
			int32_t openpx = fast_decoder_->DecodeInt32(4,DEFAULT);
      curr_index_data.OpenPx = Utils::Divide(openpx,WMDF_CONSTS_ONE_THOUSAND);

			int64_t total_amount = fast_decoder_->DecodeInt64(5,DEFAULT);
			curr_index_data.total_amount = Utils::Divide(total_amount,1);

			int32_t highpx = fast_decoder_->DecodeInt32(6,DEFAULT);
			curr_index_data.HighPx = Utils::Divide(highpx,WMDF_CONSTS_ONE_THOUSAND);	
		
			int32_t lowpx = fast_decoder_->DecodeInt32(7,DEFAULT);
			curr_index_data.LowPx = Utils::Divide(lowpx,WMDF_CONSTS_ONE_THOUSAND);

			int32_t lastpx = fast_decoder_->DecodeInt32(8,DEFAULT);
			curr_index_data.LastPx =Utils::Divide(lastpx,WMDF_CONSTS_ONE_THOUSAND);

			curr_index_data.volume = fast_decoder_->DecodeInt64(9,DEFAULT);
		}
		else
		{
			fast_decoder_->DecodeString(3,NONE,palce_str,sizeof(palce_str));
			fast_decoder_->DecodeInt32(4,DEFAULT);
			fast_decoder_->DecodeInt64(5,DEFAULT);
			fast_decoder_->DecodeInt32(6,DEFAULT);
			fast_decoder_->DecodeInt32(7,DEFAULT);
			fast_decoder_->DecodeInt32(8,DEFAULT);
      fast_decoder_->DecodeInt64(9,DEFAULT);
		}
	}
	
  if (size > 0)
  {
	  WindFastMessage* msg = new WindFastMessage();
	  msg->Size = size;
	  msg->Data = data_array;
	  msg->MsgType = SSE_INDEXDATA_1113_TYPE;
	  return msg;
  }
  else
  {
    delete data_array;
  }
  return NULL;
}
