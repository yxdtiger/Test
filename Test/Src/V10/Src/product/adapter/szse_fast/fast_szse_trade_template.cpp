#include "fast_szse_trade_template.h"
#include "fast_szse_template_object.h"
#include "../common/rawdata_object.h"

wmdf::FastSzseTradeTemplate::FastSzseTradeTemplate( void )
:FastTemplate(item_count_)
{
 
}

wmdf::FastSzseTradeTemplate::~FastSzseTradeTemplate( void )
{
 
}

wmdf::WindFastMessage* wmdf::FastSzseTradeTemplate::DecodeRaw(uint8_t* raw_data,uint32_t len)
{
  int data_status=0;
  int tid=0;
  fast_decoder_->ReSet((uint8_t*)raw_data,len);
  fast_decoder_->SetDefaultValue(10);
  int capacity=4;
  int size=0;
  stTemplate_Trade_202* data_array = new stTemplate_Trade_202[capacity];//做整体分配的方法	
 	for(int32_t i=0;i!=capacity;++i)
 	{
 		data_array[i].BuyOrderRecNo = WMDF_CONSTS_32BIT_NIL_VALUE;
 		memset(data_array[i].FunctionCode,0,sizeof(data_array[i].FunctionCode));
 		memset(data_array[i].OrderKind,0,sizeof(data_array[i].OrderKind));
 		data_array[i].Price = WMDF_CONSTS_DOUBLE_NIL_VALUE;
 		data_array[i].RecNo = WMDF_CONSTS_32BIT_NIL_VALUE;
 		memset(data_array[i].SecurityID,0,sizeof(data_array[i].SecurityID));
 		data_array[i].SellOrderRecNo = WMDF_CONSTS_32BIT_NIL_VALUE;
 		data_array[i].SetNo = WMDF_CONSTS_32BIT_NIL_VALUE;
 		data_array[i].TradeQty = WMDF_CONSTS_32BIT_NIL_VALUE;
 		data_array[i].TradeTime = WMDF_CONSTS_32BIT_NIL_VALUE;
 	}
  while(!fast_decoder_->Empty())
  {
    fast_decoder_->BeginNewMessage();
    tid = fast_decoder_->DecodeUInt32(0,DEFAULT);
    assert(tid==202);
    // constant 变量不需要
    if(size>=capacity)
    {
      stTemplate_Trade_202* temp = new stTemplate_Trade_202[capacity*2];
      memcpy(temp,data_array,sizeof(stTemplate_Trade_202)*capacity);
      capacity=capacity*2;
      delete[] data_array;
      data_array = temp;
 			for(int32_t i=size;i!=capacity;++i)
 			{
 				data_array[i].BuyOrderRecNo = WMDF_CONSTS_32BIT_NIL_VALUE;
 				memset(data_array[i].FunctionCode,0,sizeof(data_array[i].FunctionCode));
 				memset(data_array[i].OrderKind,0,sizeof(data_array[i].OrderKind));
 				data_array[i].Price = WMDF_CONSTS_DOUBLE_NIL_VALUE;
 				data_array[i].RecNo = WMDF_CONSTS_32BIT_NIL_VALUE;
 				memset(data_array[i].SecurityID,0,sizeof(data_array[i].SecurityID));
 				data_array[i].SellOrderRecNo = WMDF_CONSTS_32BIT_NIL_VALUE;
 				data_array[i].SetNo = WMDF_CONSTS_32BIT_NIL_VALUE;
 				data_array[i].TradeQty = WMDF_CONSTS_32BIT_NIL_VALUE;
 				data_array[i].TradeTime = WMDF_CONSTS_32BIT_NIL_VALUE;
 			}
    }
    //读取数据
    stTemplate_Trade_202& current_transcation=data_array[size++];

    current_transcation.SetNo=fast_decoder_->DecodeUInt32(1,COPY);
    current_transcation.RecNo = fast_decoder_->DecodeUInt32(2,INCREASE);
    current_transcation.BuyOrderRecNo = fast_decoder_->DecodeUInt32(3,NONE);
    current_transcation.SellOrderRecNo = fast_decoder_->DecodeUInt32(4,NONE);
    fast_decoder_->DecodeString(5,NONE,current_transcation.SecurityID,sizeof(current_transcation.SecurityID));
    current_transcation.Price = Utils::Divide(fast_decoder_->DecodeUInt32(6,NONE),WMDF_CONSTS_ONE_THOUSAND);
    current_transcation.TradeQty = fast_decoder_->DecodeUInt32(7,NONE);
    fast_decoder_->DecodeString(8,NONE,current_transcation.OrderKind,sizeof(current_transcation.OrderKind));
    fast_decoder_->DecodeString(9,NONE,current_transcation.FunctionCode,sizeof(current_transcation.FunctionCode));
    current_transcation.TradeTime = fast_decoder_->DecodeInt32(10,DELTA,PT_MANDATORY);
  }
  WindFastMessage* msg = new WindFastMessage();
  msg->MsgType = SZSE_TRANSACTION_TYPE;
  msg->Size=size;
  msg->Data = data_array;
  return msg;

}
