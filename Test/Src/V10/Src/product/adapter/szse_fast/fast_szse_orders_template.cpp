#include "fast_szse_orders_template.h"
#include "fast_szse_template_object.h"
#include "../common/rawdata_object.h"

wmdf::FastSzseOrdersTemplate::FastSzseOrdersTemplate( void )
:FastTemplate(item_count_)
{
}

wmdf::FastSzseOrdersTemplate::~FastSzseOrdersTemplate( void )
{

}

wmdf::WindFastMessage* wmdf::FastSzseOrdersTemplate::DecodeRaw(uint8_t* raw_data,uint32_t len)
{
  int data_status=0;
  int tid=0;
  fast_decoder_->ReSet((uint8_t*)raw_data,len);
  //current_transcation.OrderEntryTime的默认值设为0；
  fast_decoder_->SetDefaultValue(8);
  int capacity=4;
  int size=0;
  stTemplate_Order_201* data_array = new stTemplate_Order_201[capacity];//做整体分配的方法	
	for(int32_t i=0;i!=capacity;++i)
	{
		memset(data_array[i].FunctionCode,0,sizeof(data_array[i].FunctionCode));
		data_array[i].OrderEntryTime = WMDF_CONSTS_32BIT_NIL_VALUE;
		memset(data_array[i].OrderKind,0,sizeof(data_array[i].OrderKind));
		data_array[i].OrderQty = WMDF_CONSTS_32BIT_NIL_VALUE;
		data_array[i].Price = WMDF_CONSTS_DOUBLE_NIL_VALUE;
		data_array[i].RecNo = WMDF_CONSTS_32BIT_NIL_VALUE;
		memset(data_array[i].SecurityID,0,sizeof(data_array[i].SecurityID));
		data_array[i].SetNo = WMDF_CONSTS_32BIT_NIL_VALUE;
	}
  while(!fast_decoder_->Empty())
  {
    fast_decoder_->BeginNewMessage();
    tid = fast_decoder_->DecodeUInt32(0,DEFAULT);
    assert(tid==201);
    // constant 变量不需要
    if(size>=capacity)
    {
      stTemplate_Order_201* temp = new stTemplate_Order_201[capacity*2];
      memcpy(temp,data_array,sizeof(stTemplate_Order_201)*capacity);
      capacity=capacity*2;
      delete[] data_array;
      data_array = temp;
			for(int32_t i=size;i!=capacity;++i)
			{
				memset(data_array[i].FunctionCode,0,sizeof(data_array[i].FunctionCode));
				data_array[i].OrderEntryTime = WMDF_CONSTS_32BIT_NIL_VALUE;
				memset(data_array[i].OrderKind,0,sizeof(data_array[i].OrderKind));
				data_array[i].OrderQty = WMDF_CONSTS_32BIT_NIL_VALUE;
				data_array[i].Price = WMDF_CONSTS_DOUBLE_NIL_VALUE;
				data_array[i].RecNo = WMDF_CONSTS_32BIT_NIL_VALUE;
				memset(data_array[i].SecurityID,0,sizeof(data_array[i].SecurityID));
				data_array[i].SetNo = WMDF_CONSTS_32BIT_NIL_VALUE;
			}
    }
    //读取数据
    stTemplate_Order_201& current_transcation=data_array[size++];
    current_transcation.SetNo=fast_decoder_->DecodeUInt32(1,COPY);
    current_transcation.RecNo = fast_decoder_->DecodeUInt32(2,INCREASE);
    fast_decoder_->DecodeString(3,NONE,current_transcation.SecurityID,sizeof(current_transcation.SecurityID));
    current_transcation.Price = Utils::Divide(fast_decoder_->DecodeUInt32(4,NONE),WMDF_CONSTS_ONE_THOUSAND);
    current_transcation.OrderQty = fast_decoder_->DecodeUInt32(5,NONE);
    fast_decoder_->DecodeString(6,NONE,current_transcation.OrderKind,sizeof(current_transcation.OrderKind));
    fast_decoder_->DecodeString(7,NONE,current_transcation.FunctionCode,sizeof(current_transcation.FunctionCode));
    current_transcation.OrderEntryTime = fast_decoder_->DecodeInt32(8,DELTA,PT_MANDATORY);
  }
  WindFastMessage* msg = new WindFastMessage();
  msg->MsgType= SZSE_ORDERS_TYPE;
  msg->Size=size;
  msg->Data = data_array;
  return msg;
}
