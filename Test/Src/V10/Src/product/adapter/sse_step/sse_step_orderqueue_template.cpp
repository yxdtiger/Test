#include "sse_step_orderqueue_template.h"

wmdf::SSEStepOrderQueueTemplate::SSEStepOrderQueueTemplate()
{

}

wmdf::SSEStepOrderQueueTemplate::~SSEStepOrderQueueTemplate()
{

}

wmdf::WindStepMessage* wmdf::SSEStepOrderQueueTemplate::DecodeRaw( uint8_t* message_start,uint8_t* message_end )
{
  int ret=0;
	step_decoder_->ReSet((char*)message_start,(char*)message_end);
	OrderQueueImage* data_array = new OrderQueueImage();
	data_array->date_time_stamp = WMDF_CONSTS_32BIT_NIL_VALUE;
	data_array->image_status = WMDF_CONSTS_32BIT_NIL_VALUE;
	data_array->order_queue_levels = NULL;
	data_array->price_level_count = WMDF_CONSTS_32BIT_NIL_VALUE;
	memset(data_array->side,0,sizeof(data_array->side));
	memset(data_array->wind_code,0,sizeof(data_array->wind_code));

	if(FIND_TAG("10072"))
		step_decoder_->DecodeInt32();
	if(FIND_TAG("10178"))
		data_array->date_time_stamp = step_decoder_->DecodeInt32();
  if(FIND_TAG("10121"))
  {
    delete data_array;
    return NULL;
  }  
	if(FIND_TAG("54"))
		data_array->side[0] = step_decoder_->DecodeChar();
	if(FIND_TAG("48"))
		step_decoder_->DecodeString(data_array->wind_code,sizeof(data_array->wind_code));
	if(FIND_TAG("10063"))
		data_array->price_level_count = step_decoder_->DecodeInt32();

	if ((data_array->price_level_count > 0) && (data_array->price_level_count != WMDF_CONSTS_32BIT_NIL_VALUE))
  {
		data_array->order_queue_levels = new OrderQueuePriceLevel[data_array->price_level_count];
    for(size_t i=0; i!=data_array->price_level_count; ++i)
    {
      if(FIND_TAG("44"))
        data_array->order_queue_levels[i].price =  step_decoder_->DecodeDouble();
      if(FIND_TAG("10067"))
        data_array->order_queue_levels[i].orders_total = step_decoder_->DecodeInt32();
      if(FIND_TAG("73"))
        data_array->order_queue_levels[i].orders_count = step_decoder_->DecodeInt32();

      if(data_array->order_queue_levels[i].orders_count == 0)
        data_array->order_queue_levels[i].order_queue_items = NULL;
      else
      {
        data_array->order_queue_levels[i].order_queue_items = new OrderItem[data_array->order_queue_levels->orders_count];
        for(size_t j=0; j!=data_array->order_queue_levels[i].orders_count; ++j)
        {
          if(FIND_TAG("38"))
            data_array->order_queue_levels[i].order_queue_items[j].order_queue_operator = WMDF_CONSTS_32BIT_NIL_VALUE;
          data_array->order_queue_levels[i].order_queue_items[j].order_queue_operator_entry_id = WMDF_CONSTS_32BIT_NIL_VALUE;
            data_array->order_queue_levels[i].order_queue_items[j].order_qty = step_decoder_->DecodeInt32();
        }      
      }
    }
  }
	else
	{
		delete data_array;
		return NULL;
	}

	WindStepMessage* msg = new WindStepMessage();
	msg->MsgType = SSE_ORDERQUEUE_1111_TYPE;
	msg->Data = data_array;
	return msg;
}

