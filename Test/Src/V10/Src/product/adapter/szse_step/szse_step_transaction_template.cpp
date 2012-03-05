#include "szse_step_transaction_template.h"

wmdf::SZSEStepTransactionTemplate::SZSEStepTransactionTemplate()
{

}


wmdf::SZSEStepTransactionTemplate::~SZSEStepTransactionTemplate()
{
}



wmdf::WindStepMessage* wmdf::SZSEStepTransactionTemplate::DecodeRaw( uint8_t* message_start,uint8_t* message_end )
{
  int ret=0;
	step_decoder_->ReSet((char*)message_start,(char*)message_end);

	Transaction* data_array = new Transaction();

	if(FIND_TAG("10115"))
		data_array->set_no = step_decoder_->DecodeInt32();
	if(FIND_TAG("8926"))
		data_array->rec_no = step_decoder_->DecodeInt32();

  if(FIND_TAG("10116"))
    data_array->buyOrderRecNo = step_decoder_->DecodeInt32();
  if(FIND_TAG("10117"))
    data_array->buyOrderRecNo = step_decoder_->DecodeInt32();

	if(FIND_TAG("48"))
		step_decoder_->DecodeString(data_array->wind_code,sizeof(data_array->wind_code));
	if(FIND_TAG("44"))
		data_array->price = step_decoder_->DecodeDouble();
	if(FIND_TAG("10015"))
		data_array->trade_qty=step_decoder_->DecodeInt32();
	if(FIND_TAG("8930"))
		data_array->order_kind=step_decoder_->DecodeChar();
	if(FIND_TAG("8931"))
		data_array->function_code=step_decoder_->DecodeChar();
	if(FIND_TAG("10013"))
		data_array->trade_time=step_decoder_->DecodeSZSETimeStamp();

	WindStepMessage* msg = new WindStepMessage();
	msg->MsgType = SZSE_TRANSACTION_202_TYPE;
	msg->Data = data_array;
	return msg;
}


