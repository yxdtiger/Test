#include "szse_step_orders_template.h"

wmdf::SZSEStepOrdersTemplate::SZSEStepOrdersTemplate()
{

}

wmdf::SZSEStepOrdersTemplate::~SZSEStepOrdersTemplate()
{

}

wmdf::WindStepMessage* wmdf::SZSEStepOrdersTemplate::DecodeRaw( uint8_t* message_start,uint8_t* message_end )
{
  int32_t ret=0;
	step_decoder_->ReSet((char*)message_start,(char*)message_end);

	Orders* data_array = new Orders();
	memset(data_array->wind_code,0,sizeof(data_array->wind_code));
	data_array->function_code = WMDF_CONSTS_8BIT_NIL_VALUE;
	data_array->order_kind = WMDF_CONSTS_8BIT_NIL_VALUE;
	data_array->order_time = WMDF_CONSTS_32BIT_NIL_VALUE;
	data_array->price = WMDF_CONSTS_DOUBLE_NIL_VALUE;
	data_array->rec_no = WMDF_CONSTS_32BIT_NIL_VALUE;
	data_array->set_no = WMDF_CONSTS_32BIT_NIL_VALUE;
	data_array->size = WMDF_CONSTS_32BIT_NIL_VALUE;

	if(FIND_TAG("10115"))
		data_array->set_no = step_decoder_->DecodeInt32();
	if(FIND_TAG("8926"))
		data_array->rec_no = step_decoder_->DecodeInt32();
	if(FIND_TAG("48"))
		step_decoder_->DecodeString(data_array->wind_code,sizeof(data_array->wind_code));
	if(FIND_TAG("44"))
		data_array->price = step_decoder_->DecodeDouble();
	if(FIND_TAG("38"))
		data_array->size = step_decoder_->DecodeInt32();
	if(FIND_TAG("8930"))
		data_array->order_kind=step_decoder_->DecodeChar();
	if(FIND_TAG("8931"))
		data_array->function_code=step_decoder_->DecodeChar();
	if(FIND_TAG("8500"))
		data_array->order_time = step_decoder_->DecodeSZSETimeStamp();

	WindStepMessage* msg = new WindStepMessage();
	msg->MsgType = TEMPLATE_SZSE_ORDERS_201_TYPE;
	msg->Data = data_array;
	return msg;
}

