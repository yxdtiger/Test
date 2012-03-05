#include "szse_step_indexdata_template.h"


using namespace std;
wmdf::SZSEStepIndexDataTemplate::SZSEStepIndexDataTemplate()
{
	has_before925_ = false;
}

wmdf::SZSEStepIndexDataTemplate::~SZSEStepIndexDataTemplate()
{

}

wmdf::WindStepMessage* wmdf::SZSEStepIndexDataTemplate::DecodeRaw( uint8_t* message_start,uint8_t* message_end )
{
  int ret=0;
	step_decoder_->ReSet((char*)message_start,(char*)message_end);
	char palce_str[20];
	IndexData* data_array = new IndexData();
	data_array->DataStatus = WMDF_CONSTS_32BIT_NIL_VALUE;
	data_array->DataTimeStamp = WMDF_CONSTS_32BIT_NIL_VALUE;
	data_array->HighPx = WMDF_CONSTS_DOUBLE_NIL_VALUE;
	data_array->LastPx = WMDF_CONSTS_DOUBLE_NIL_VALUE;
	data_array->LowPx = WMDF_CONSTS_DOUBLE_NIL_VALUE;
	data_array->OpenPx = WMDF_CONSTS_DOUBLE_NIL_VALUE;
	data_array->PreClosePx = WMDF_CONSTS_DOUBLE_NIL_VALUE;
	memset(data_array->SecurityID,0,sizeof(data_array->SecurityID));
	data_array->total_amount = WMDF_CONSTS_DOUBLE_NIL_VALUE;
	data_array->transactions_count = WMDF_CONSTS_64BIT_NIL_VALUE;
	data_array->volume = WMDF_CONSTS_64BIT_NIL_VALUE;


	if(FIND_TAG("10178"))
		data_array->DataTimeStamp = step_decoder_->DecodeSZSETimeStamp();
	if(FIND_TAG("48"))
		step_decoder_->DecodeString(data_array->SecurityID,sizeof(data_array->SecurityID));
	if(FIND_TAG("10007"))
	{
		data_array->PreClosePx = step_decoder_->DecodeDouble();
		if(data_array->PreClosePx ==0)
			data_array->PreClosePx = WMDF_CONSTS_DOUBLE_NIL_VALUE;
	}
	if(FIND_TAG("10006"))
	{
		data_array->OpenPx = step_decoder_->DecodeDouble();
		if(data_array->OpenPx ==0)
			data_array->OpenPx = WMDF_CONSTS_DOUBLE_NIL_VALUE;
	}
	if(FIND_TAG("10009"))
	{
		data_array->HighPx = step_decoder_->DecodeDouble();
		if(data_array->HighPx == 0)
			data_array->HighPx = WMDF_CONSTS_DOUBLE_NIL_VALUE;
	}
	if(FIND_TAG("10010"))
	{
		data_array->LowPx = step_decoder_->DecodeDouble();
		if(data_array->LowPx ==0)
			data_array->LowPx = WMDF_CONSTS_DOUBLE_NIL_VALUE;
	}
	if(FIND_TAG("10008"))
	{
		data_array->LastPx = step_decoder_->DecodeDouble();
		if(data_array->LastPx == 0)
			data_array->LastPx = WMDF_CONSTS_DOUBLE_NIL_VALUE;
	}

	if(FIND_TAG("10118"))
		data_array->total_amount = step_decoder_->DecodeDouble();

	if(FIND_TAG("8503"))
		data_array->transactions_count = step_decoder_->DecodeInt64();

	if(FIND_TAG("387"))
		data_array->volume = step_decoder_->DecodeInt64();

	if(FIND_TAG("10004"))
    std::string endofdaymaker = step_decoder_->DecodeString((char*)palce_str,20);

	if(has_before925_ && data_array->DataTimeStamp<92500000)
	{
		delete data_array;
		return NULL;
	}

	has_before925_ = true;

	if(data_array->DataTimeStamp >=113100000 && data_array->DataTimeStamp<130000000)
	{
		delete data_array;
		return NULL;
	}


	map<string,int32_t>::iterator iter = time_map_.find(data_array->SecurityID);
	if(iter == time_map_.end())
	{
		time_map_.insert(make_pair(data_array->SecurityID,data_array->DataTimeStamp));
	}
	else
	{
		if(iter->second == data_array->DataTimeStamp)
		{
			delete data_array;
			return NULL;
		}
    else
    {
      iter->second=data_array->DataTimeStamp;
    }
	}

	WindStepMessage* msg = new WindStepMessage();
	msg->MsgType = SZSE_INDEXDATA_104_TYPE;
	msg->Data = data_array;
	return msg;
}

