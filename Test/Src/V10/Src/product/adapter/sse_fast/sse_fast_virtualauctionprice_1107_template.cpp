#include "sse_fast_virtualauctionprice_1107_template.h"
#include "../common/rawdata_object.h"


wmdf::SSEFastVirtualAuctionPrice1107Template::SSEFastVirtualAuctionPrice1107Template()
:FastTemplate(item_count_)
{

}

wmdf::SSEFastVirtualAuctionPrice1107Template::~SSEFastVirtualAuctionPrice1107Template()
{

}

wmdf::WindFastMessage* wmdf::SSEFastVirtualAuctionPrice1107Template::DecodeRaw( uint8_t* raw_data,uint32_t len )
{
	int data_status = 0,data_time_stamp = 0;
	int tid=0;
	fast_decoder_->ReSet(raw_data,len);
	int capacity = 64;
	int size=0;
	uint8_t palce_str[20];
	uint64_t temp=0;
	VirtualAuctionPrice* data_array = new VirtualAuctionPrice[capacity];
	for(int32_t i=0;i!=capacity;++i)
	{
		memset(data_array[i].windcode,0,sizeof(data_array->windcode));
		data_array[i].askleaveqty = WMDF_CONSTS_32BIT_NIL_VALUE;
		data_array[i].bidleaveqty = WMDF_CONSTS_32BIT_NIL_VALUE;
		data_array[i].date_time_stamp = WMDF_CONSTS_32BIT_NIL_VALUE;
		data_array[i].price = WMDF_CONSTS_DOUBLE_NIL_VALUE;
		data_array[i].virtualauctionqty = WMDF_CONSTS_32BIT_NIL_VALUE;
	}
	while(!fast_decoder_->Empty())
	{
		fast_decoder_->BeginNewMessage();
		tid = fast_decoder_->DecodeUInt32(0,DEFAULT);
		assert(tid == SSE_VIRTUALAUCTIONPRICE_1107_TYPE);
		data_time_stamp = fast_decoder_->DecodeInt32(1,COPY,PT_MANDATORY);
		data_status = fast_decoder_->DecodeInt32(2,DEFAULT);
		if(data_status<=0)
		{
			if(size>=capacity)
			{
				VirtualAuctionPrice* temp = new VirtualAuctionPrice[capacity*2];
				memcpy(temp,data_array,sizeof(VirtualAuctionPrice)*capacity);
				capacity = capacity*2;
				delete [] data_array;
				data_array = temp;
				//for(int32_t i=size;i!=capacity;++i)
				//{
				//	memset(data_array[i].windcode,0,sizeof(data_array->windcode));
				//	data_array[i].askleaveqty = WMDF_CONSTS_32BIT_NIL_VALUE;
				//	data_array[i].bidleaveqty = WMDF_CONSTS_32BIT_NIL_VALUE;
				//	data_array[i].date_time_stamp = WMDF_CONSTS_32BIT_NIL_VALUE;
				//	data_array[i].price = WMDF_CONSTS_DOUBLE_NIL_VALUE;
				//	data_array[i].virtualauctionqty = WMDF_CONSTS_32BIT_NIL_VALUE;
				//}
			}
			VirtualAuctionPrice& curr_data = data_array[size++];
      curr_data.date_time_stamp = Utils::Multiply(data_time_stamp,1000);
			fast_decoder_->DecodeString(3,NONE,(uint8_t*)curr_data.windcode,sizeof(curr_data.windcode));
      curr_data.price = Utils::Divide(fast_decoder_->DecodeInt32(4,NONE,PT_MANDATORY),WMDF_CONSTS_ONE_THOUSAND);
			curr_data.virtualauctionqty = Utils::ConvertToInt32(fast_decoder_->DecodeInt64(5,NONE,PT_MANDATORY));
			curr_data.bidleaveqty = Utils::ConvertToInt32(fast_decoder_->DecodeInt64(6,NONE,PT_MANDATORY));
			curr_data.askleaveqty = Utils::ConvertToInt32(fast_decoder_->DecodeInt64(7,NONE,PT_MANDATORY));
		}
		else
		{
			fast_decoder_->DecodeString(3,NONE,palce_str,sizeof(palce_str));
			fast_decoder_->DecodeInt32(4,NONE);
			fast_decoder_->DecodeInt64(5,NONE);
			fast_decoder_->DecodeInt64(6,NONE);
			fast_decoder_->DecodeInt64(7,NONE);
		}
	}

  if (size>0)
  {
	  WindFastMessage* msg = new WindFastMessage();
	  msg->MsgType = SSE_VIRTUALAUCTIONPRICE_1107_TYPE;
    msg->Size = size;
	  msg->Data = data_array;
	  return msg;
  }
  else
  {
    delete data_array;
  }
  return NULL;
}
