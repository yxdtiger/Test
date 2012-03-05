#include "sse_step_transaction_template.h"

wmdf::SSEStepTransactionTemplate::SSEStepTransactionTemplate()
{

}

wmdf::SSEStepTransactionTemplate::~SSEStepTransactionTemplate()
{

}

wmdf::WindStepMessage* wmdf::SSEStepTransactionTemplate::DecodeRaw( uint8_t* message_start,uint8_t* message_end )
{
	int ret=0;
	step_decoder_->ReSet((char*)message_start,(char*)message_end);
	Transaction* data_array = new Transaction();
	data_array->buyOrderRecNo = WMDF_CONSTS_32BIT_NIL_VALUE;
	data_array->function_code = WMDF_CONSTS_8BIT_NIL_VALUE;
	data_array->order_kind = WMDF_CONSTS_8BIT_NIL_VALUE;
	data_array->price = WMDF_CONSTS_DOUBLE_NIL_VALUE;
	data_array->rec_no = WMDF_CONSTS_32BIT_NIL_VALUE;
	data_array->sellOrderRecNo = WMDF_CONSTS_32BIT_NIL_VALUE;
	data_array->set_no = WMDF_CONSTS_32BIT_NIL_VALUE;
	data_array->trade_money = WMDF_CONSTS_DOUBLE_NIL_VALUE;
	data_array->trade_qty = WMDF_CONSTS_32BIT_NIL_VALUE;
	data_array->trade_time = WMDF_CONSTS_32BIT_NIL_VALUE;
	memset(data_array->wind_code,0,sizeof(data_array->wind_code));


	if(FIND_TAG("10072"))
		step_decoder_->DecodeInt32();
	if(FIND_TAG("10011"))
		data_array->rec_no = step_decoder_->DecodeInt32();
	if(FIND_TAG("10115"))
		data_array->set_no = step_decoder_->DecodeInt32();
	if(FIND_TAG("10121"))
	{
		delete data_array;
		return NULL;
	}
	
	if(FIND_TAG("48"))
		step_decoder_->DecodeString(data_array->wind_code,sizeof(data_array->wind_code));
	if(FIND_TAG("10012"))
		step_decoder_->DecodeInt32();
	if(FIND_TAG("10013"))
		data_array->trade_time = step_decoder_->DecodeSSETimeStamp();
	if(FIND_TAG("10014"))
		data_array->price = step_decoder_->DecodeDouble();
	if(FIND_TAG("10015"))
		data_array->trade_qty = step_decoder_->DecodeInt32();
	if(FIND_TAG("10016"))
		data_array->trade_money = step_decoder_->DecodeDouble();


	WindStepMessage* msg = new WindStepMessage();
	msg->MsgType = SSE_TRANSACTION_1101_TYPE;
	msg->Data = data_array;
  return msg;
}

