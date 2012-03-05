#include "sse_fast_transaction_3101_template.h"
#include "../common/rawdata_object.h"



wmdf::SSEFastTransaction3101Template::SSEFastTransaction3101Template()
:FastTemplate(item_count_)
{

}

wmdf::SSEFastTransaction3101Template::~SSEFastTransaction3101Template()
{

}

wmdf::WindFastMessage* wmdf::SSEFastTransaction3101Template::DecodeRaw(uint8_t* raw_data,uint32_t len)
{
	int data_status=0;
	int tid=0;
	fast_decoder_->ReSet(raw_data,len);
	int capacity=64;
	int size=0;
	uint8_t palce_str[20];
	Transaction* data_array = new Transaction[capacity];//做整体分配的方法	
	for(int32_t i=0;i!=capacity;++i)
	{
		data_array[i].buyOrderRecNo = WMDF_CONSTS_32BIT_NIL_VALUE;
		data_array[i].function_code = WMDF_CONSTS_8BIT_NIL_VALUE;
		data_array[i].order_kind = WMDF_CONSTS_8BIT_NIL_VALUE;
		data_array[i].price = WMDF_CONSTS_DOUBLE_NIL_VALUE;
		data_array[i].rec_no = WMDF_CONSTS_32BIT_NIL_VALUE;
		data_array[i].sellOrderRecNo = WMDF_CONSTS_32BIT_NIL_VALUE;
		data_array[i].set_no = WMDF_CONSTS_32BIT_NIL_VALUE;
		data_array[i].trade_money = WMDF_CONSTS_DOUBLE_NIL_VALUE;
		data_array[i].trade_qty = WMDF_CONSTS_32BIT_NIL_VALUE;
		data_array[i].trade_time = WMDF_CONSTS_32BIT_NIL_VALUE;
		memset(data_array[i].wind_code,0,sizeof(data_array->wind_code));
	}
	while(!fast_decoder_->Empty())
	{

		fast_decoder_->BeginNewMessage();
		tid = fast_decoder_->DecodeUInt32(0,DEFAULT);
		assert(tid==SSE_TRANSACTION_3101_TYPE);
		data_status=fast_decoder_->DecodeInt32(1,DEFAULT);
		if(data_status<=0)//数据有效
		{
			// constant 变量不需要
			if(size>=capacity)
			{
				Transaction* temp = new Transaction[capacity*2];
				memcpy(temp,data_array,sizeof(Transaction)*capacity);
				capacity=capacity*2;
				delete[] data_array;
				data_array = temp;
				for(int32_t i=size;i!=capacity;++i)
				{
					data_array[i].buyOrderRecNo = WMDF_CONSTS_32BIT_NIL_VALUE;
					data_array[i].function_code = WMDF_CONSTS_8BIT_NIL_VALUE;
					data_array[i].order_kind = WMDF_CONSTS_8BIT_NIL_VALUE;
					data_array[i].price = WMDF_CONSTS_DOUBLE_NIL_VALUE;
					data_array[i].rec_no = WMDF_CONSTS_32BIT_NIL_VALUE;
					data_array[i].sellOrderRecNo = WMDF_CONSTS_32BIT_NIL_VALUE;
					data_array[i].set_no = WMDF_CONSTS_32BIT_NIL_VALUE;
					data_array[i].trade_money = WMDF_CONSTS_DOUBLE_NIL_VALUE;
					data_array[i].trade_qty = WMDF_CONSTS_32BIT_NIL_VALUE;
					data_array[i].trade_time = WMDF_CONSTS_32BIT_NIL_VALUE;
					memset(data_array[i].wind_code,0,sizeof(data_array->wind_code));
				}
			}
			//读取数据
			Transaction& current_transcation=data_array[size++];
			current_transcation.rec_no=fast_decoder_->DecodeInt32(2,INCREASE,PT_MANDATORY);
			current_transcation.set_no=fast_decoder_->DecodeInt32(3,COPY);//上交所trade_channel并入set_no
			fast_decoder_->DecodeString(4,COPY,(uint8_t*)current_transcation.wind_code,sizeof(current_transcation.wind_code));
			
			int32_t tradetime = fast_decoder_->DecodeInt32(5,COPY);
      current_transcation.trade_time = Utils::Multiply(tradetime,10);

			int32_t price = fast_decoder_->DecodeInt32(6,DEFAULT);
      current_transcation.price = Utils::Divide(price,WMDF_CONSTS_ONE_THOUSAND);

			int64_t tradeqty = fast_decoder_->DecodeInt64(7,DEFAULT);
			current_transcation.trade_qty = Utils::DivideEx2(tradeqty,WMDF_CONSTS_ONE_THOUSAND);

			int64_t trademoney = fast_decoder_->DecodeInt64(8,DEFAULT);
			current_transcation.trade_money = Utils::Divide(trademoney,WMDF_CONSTS_HUNDRED_THOUSAND);
		}
		else//数据无效
		{
			fast_decoder_->DecodeInt32(2,INCREASE);
			fast_decoder_->DecodeInt32(3,DEFAULT);
			fast_decoder_->DecodeString(4,DEFAULT,palce_str,sizeof(palce_str));
			fast_decoder_->DecodeInt32(5,DEFAULT);
			fast_decoder_->DecodeInt32(6,DEFAULT);
			fast_decoder_->DecodeInt64(7,DEFAULT);
			fast_decoder_->DecodeInt64(8,DEFAULT);
		}
	}

  if (size > 0)
  {
    WindFastMessage* msg = new WindFastMessage();
    msg->MsgType = SSE_TRANSACTION_3101_TYPE;
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
