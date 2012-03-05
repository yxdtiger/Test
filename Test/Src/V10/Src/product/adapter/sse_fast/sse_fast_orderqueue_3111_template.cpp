#include "sse_fast_orderqueue_3111_template.h"
#include "../common/rawdata_object.h"

wmdf::SSEFastOrderQueue3111Template::SSEFastOrderQueue3111Template()
:FastTemplate(item_count_)
{

}

wmdf::SSEFastOrderQueue3111Template::~SSEFastOrderQueue3111Template()
{

}

wmdf::WindFastMessage* wmdf::SSEFastOrderQueue3111Template::DecodeRaw( uint8_t* raw_data,uint32_t len )
{
	int data_status=0,data_time_stamp=0;
	int tid=0;
	fast_decoder_->ReSet(raw_data,len);
	int capacity=64;
	int size=0;

	uint8_t priceLevelMap[56];
	uint8_t orderItemMap[56];
	uint64_t temp=0;
	OrderQueueImage* data_array = new OrderQueueImage[capacity];//做整体分配的方法	
	for(int i=0;i!=capacity;++i)
	{
		data_array[i].date_time_stamp = WMDF_CONSTS_32BIT_NIL_VALUE;
		data_array[i].image_status = WMDF_CONSTS_32BIT_NIL_VALUE;
		data_array[i].order_queue_levels = NULL;
		data_array[i].price_level_count = WMDF_CONSTS_32BIT_NIL_VALUE;
		memset(data_array[i].side,0,sizeof(data_array[i].side));
	}
	while(!fast_decoder_->Empty())
	{
		fast_decoder_->BeginNewMessage();
		tid = fast_decoder_->DecodeUInt32(0,COPY);
		//printf("%d\n",tid);
		assert(tid==SSE_ORDERQUEUE_3111_TYPE);
		data_time_stamp = fast_decoder_->DecodeInt32(1,COPY,PT_MANDATORY);
		data_status=fast_decoder_->DecodeInt32(2,DEFAULT);
		if(data_status<=0)//数据有效
		{
			// constant 变量不需要
			if(size>=capacity)
			{
				OrderQueueImage* temp = new OrderQueueImage[capacity*2];
				memcpy(temp,data_array,sizeof(OrderQueueImage)*capacity);
				capacity=capacity*2;
				delete[] data_array;
				data_array = temp;
				for(int i=size;i!=capacity;++i)
				{
					data_array[i].date_time_stamp = WMDF_CONSTS_32BIT_NIL_VALUE;
					data_array[i].image_status = WMDF_CONSTS_32BIT_NIL_VALUE;
					data_array[i].order_queue_levels = NULL;
					data_array[i].price_level_count = WMDF_CONSTS_32BIT_NIL_VALUE;
					memset(data_array[i].side,0,sizeof(data_array[i].side));
				}
			}
			//读取数据
			OrderQueueImage& current_Orderqueue=data_array[size++];
			current_Orderqueue.date_time_stamp = data_time_stamp*1000;
			fast_decoder_->DecodeString(3,COPY,&current_Orderqueue.side[0],sizeof(current_Orderqueue.side));
			fast_decoder_->DecodeString(4,NONE,(uint8_t*)current_Orderqueue.wind_code,sizeof(current_Orderqueue.wind_code));
			current_Orderqueue.image_status = fast_decoder_->DecodeInt32(5,COPY);
			current_Orderqueue.price_level_count=fast_decoder_->DecodeInt32(6,NONE);
			if(current_Orderqueue.price_level_count>0)
			{
				current_Orderqueue.order_queue_levels = new OrderQueuePriceLevel[current_Orderqueue.price_level_count];
				for (int i=0;i<current_Orderqueue.price_level_count;i++)
				{
					OrderQueuePriceLevel& priceLevel = current_Orderqueue.order_queue_levels[i];
					fast_decoder_->BeginSqueuce(priceLevelMap,sizeof(priceLevelMap));

					priceLevel.price_level_operator = fast_decoder_->DecodeInt32(1,DEFAULT);
					priceLevel.price = Utils::Divide(fast_decoder_->DecodeInt32(2,NONE,PT_MANDATORY),WMDF_CONSTS_ONE_THOUSAND);
					priceLevel.orders_total = fast_decoder_->DecodeInt32(3,NONE,PT_MANDATORY);
					priceLevel.orders_count = fast_decoder_->DecodeInt32(4,NONE);
					priceLevel.order_queue_items = NULL;
					fast_decoder_->EndSqueuce();

					if(priceLevel.orders_count>0)
					{
						priceLevel.order_queue_items = new OrderItem[priceLevel.orders_count];
						for (int j=0;j<priceLevel.orders_count;j++)
						{
							OrderItem& orderitem = priceLevel.order_queue_items[j];
							fast_decoder_->BeginSqueuce(orderItemMap,sizeof(orderItemMap));

							orderitem.order_queue_operator = fast_decoder_->DecodeInt32(1,DEFAULT);
							orderitem.order_queue_operator_entry_id  = fast_decoder_->DecodeInt32(2,DEFAULT);

							int64_t orderqty = fast_decoder_->DecodeInt64(3,DEFAULT);
              orderitem.order_qty = Utils::DivideEx2(orderqty,WMDF_CONSTS_ONE_THOUSAND);

							fast_decoder_->EndSqueuce();
						}
					}
					else
					{
						priceLevel.orders_count = 0;
						priceLevel.order_queue_items = NULL;
					}
				}
			}
			else
			{
				current_Orderqueue.price_level_count = 0;
				current_Orderqueue.order_queue_levels=NULL;
			}

		}else//数据无效
		{
			//do nothing;
			assert(0);
		}
	}

  if (size > 0)
  {
    WindFastMessage* msg = new WindFastMessage();
    msg->MsgType = SSE_ORDERQUEUE_3111_TYPE;
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
