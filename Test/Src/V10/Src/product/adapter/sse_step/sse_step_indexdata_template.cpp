#include "sse_step_indexdata_template.h"

wmdf::SSEStepIndexDataTemplate::SSEStepIndexDataTemplate()
{

}

wmdf::SSEStepIndexDataTemplate::~SSEStepIndexDataTemplate()
{

}


wmdf::WindStepMessage* wmdf::SSEStepIndexDataTemplate::DecodeRaw( uint8_t* message_start,uint8_t* message_end )
{
  int ret=0;
  step_decoder_->ReSet((char*)message_start,(char*)message_end);
  IndexData* data_array = new IndexData();
  memset(data_array->SecurityID,0,sizeof(data_array->SecurityID));
  data_array->DataStatus = WMDF_CONSTS_32BIT_NIL_VALUE;
  data_array->DataTimeStamp = WMDF_CONSTS_32BIT_NIL_VALUE;
  data_array->HighPx = WMDF_CONSTS_DOUBLE_NIL_VALUE;
  data_array->LastPx = WMDF_CONSTS_DOUBLE_NIL_VALUE;
  data_array->LowPx = WMDF_CONSTS_DOUBLE_NIL_VALUE;
  data_array->OpenPx = WMDF_CONSTS_DOUBLE_NIL_VALUE;
  data_array->PreClosePx = WMDF_CONSTS_DOUBLE_NIL_VALUE;
  data_array->total_amount = WMDF_CONSTS_DOUBLE_NIL_VALUE;
  data_array->transactions_count = WMDF_CONSTS_64BIT_NIL_VALUE;
  data_array->volume = WMDF_CONSTS_64BIT_NIL_VALUE;

  if(FIND_TAG("10072"))
    step_decoder_->DecodeInt32();

  if(FIND_TAG("10178"))
    data_array->DataTimeStamp = step_decoder_->DecodeInt32();

  if(FIND_TAG("10121"))
  {
    delete data_array;
    return NULL;
  }  

  if(FIND_TAG("48"))
    step_decoder_->DecodeString(data_array->SecurityID,sizeof(data_array->SecurityID));
  if(FIND_TAG("10006"))
  {
    data_array->OpenPx = step_decoder_->DecodeDouble();
    if(data_array->OpenPx == 0)
      data_array->OpenPx = WMDF_CONSTS_DOUBLE_NIL_VALUE;
  }
  if(FIND_TAG("10118"))
  {
    data_array->total_amount = step_decoder_->DecodeInt32();
    if(data_array->total_amount == 0)
      data_array->total_amount = WMDF_CONSTS_DOUBLE_NIL_VALUE;
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
    if(data_array->LowPx == 0)
      data_array->LowPx = WMDF_CONSTS_DOUBLE_NIL_VALUE;
  }

  if(FIND_TAG("10008"))
  {
    data_array->LastPx = step_decoder_->DecodeDouble();
    if(data_array->LastPx == 0)
      data_array->LastPx = WMDF_CONSTS_DOUBLE_NIL_VALUE;
  }
  if(FIND_TAG("387"))
  {
    data_array->volume = (int64_t)step_decoder_->DecodeDouble();
    if(data_array->volume == 0)
    {
      data_array->volume = WMDF_CONSTS_64BIT_NIL_VALUE;
    }
  }
  WindStepMessage* msg = new WindStepMessage();
  msg->MsgType = SSE_INDEXDATA_1113_TYPE;
  msg->Data = data_array;
  return msg;
}