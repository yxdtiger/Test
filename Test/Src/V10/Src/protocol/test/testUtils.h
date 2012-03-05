#ifndef TEST_UTILS_H
#define TEST_UTILS_H
#include "../../include/wmdf_structs.h"
#include "../../environment/utils.h"

namespace wmdf{
class TestUtils
{
public:
  static bool isValueEqual(double d1,double d2);
  static void DestroySSEL2OrderQueues(L2OrderQueue* items,int& count);
  static void TolerateOrderQueue(L2OrderQueue* orderQueue);
  static bool CompareLoginRequest(LoginRequest request1,LoginRequest request2);
  static bool CompareLoginResponse(LoginResponse* response1,LoginResponse* response2);
  static bool CompareLogout(Logout* logout1,Logout* logout2);
  static bool CompareMarketData(L1MarketSnapshot marketData1,L1MarketSnapshot marketData2);
  static bool CompareMarketDataL2(L2MarketSnapshot marketData1,L2MarketSnapshot marketData2);
  static bool CompareTransaction(L2Transaction trans1,L2Transaction trans2);
  static bool CompareOrderQueue(L2OrderQueue* order1,L2OrderQueue* order2);
  static bool CompareOrders(SZSEL2Order* order1,SZSEL2Order* order2);
  static bool CompareCFFEL1MarketData(FuturesMarketSnapshot marketData1,FuturesMarketSnapshot marketData2);
};

bool TestUtils::isValueEqual(double d1,double d2)
{
  return (d1-d2)<0.01&&(d1-d2)>-0.01;
}

void TestUtils::DestroySSEL2OrderQueues(L2OrderQueue* items,int& count)
{
  for(int i=0;i<count;i++)
  {
    for(int j=0;j<items[i].price_level_count;j++)
    {
      delete[] items[i].order_queue_levels[j].order_queue_items;
    }
    delete[] items[i].order_queue_levels;
  }
  delete[] items;
  items=NULL;
}

void TestUtils::TolerateOrderQueue(L2OrderQueue* orderQueue)
{
  for(int i=0;i<orderQueue->price_level_count;i++)
  {
    if(orderQueue->image_status==1)//full image
    {
      orderQueue->order_queue_levels[i].price_level_operator=WMDF_CONSTS_32BIT_NIL_VALUE;//absent
    }
    if(orderQueue->order_queue_levels[i].price_level_operator==3)//delete
    {
      orderQueue->order_queue_levels[i].orders_count=0;
    }
    for(int j=0;j<orderQueue->order_queue_levels[i].orders_count;j++)
    {
      if(orderQueue->order_queue_levels[i].price_level_operator==1||orderQueue->order_queue_levels[i].price_level_operator==WMDF_CONSTS_32BIT_NIL_VALUE)//add
      {
        orderQueue->order_queue_levels[i].order_queue_items[j].order_queue_operator=WMDF_CONSTS_32BIT_NIL_VALUE;
        orderQueue->order_queue_levels[i].order_queue_items[j].order_queue_operator_entry_id=WMDF_CONSTS_32BIT_NIL_VALUE;
      }
      else if(orderQueue->order_queue_levels[i].price_level_operator==2)
      {
        if(orderQueue->order_queue_levels[i].order_queue_items[j].order_queue_operator==1)
        {
          orderQueue->order_queue_levels[i].order_queue_items[j].order_queue_operator_entry_id=WMDF_CONSTS_32BIT_NIL_VALUE;
        }
        else if(orderQueue->order_queue_levels[i].order_queue_items[j].order_queue_operator==3)
        {
          orderQueue->order_queue_levels[i].order_queue_items[j].order_qty=WMDF_CONSTS_32BIT_NIL_VALUE;
        }
      }
    }
  }
}

bool TestUtils::CompareOrderQueue(L2OrderQueue* order1,L2OrderQueue* order2)
{
  if(order1->date_time_stamp==order2->date_time_stamp)
  {
    if(order1->image_status==order2->image_status)
      if(order1->side==order2->side)
        if(memcmp((uint8_t*)order1->wind_code ,(uint8_t*)order2->wind_code,10)==0)
          if(order1->price_level_count==order2->price_level_count)
          {
            for(int i = 0;i<order1->price_level_count;i++)
            {
              if(order1->order_queue_levels[i].orders_total == order2->order_queue_levels[i].orders_total&&
                order1->order_queue_levels[i].price == order2->order_queue_levels[i].price&&
                order1->order_queue_levels[i].price_level_operator == order2->order_queue_levels[i].price_level_operator)
              {
                for(int j=0;j<order1->order_queue_levels[i].orders_count;j++)
                {
                  if(order1->order_queue_levels[i].order_queue_items[j].order_qty == order2->order_queue_levels[i].order_queue_items[j].order_qty
                    &&order1->order_queue_levels[i].order_queue_items[j].order_queue_operator == order2->order_queue_levels[i].order_queue_items[j].order_queue_operator
                    &&order1->order_queue_levels[i].order_queue_items[j].order_queue_operator_entry_id == order2->order_queue_levels[i].order_queue_items[j].order_queue_operator_entry_id)
                    continue;
                  else
                    return false;
                }
              }
            }
            return true;
          }
  }
  return false;
}

bool TestUtils::CompareOrders(SZSEL2Order* order1,SZSEL2Order* order2)
{
  return order1->function_code==order2->function_code
    && order1->order_kind == order2->order_kind
    && isValueEqual(order1->order_price,order2->order_price)
    && order1->order_qty==order2->order_qty
    && order1->order_time==order2->order_time
    && order1->rec_no == order2->rec_no
    && order1->set_no == order2->set_no
    && (memcmp((uint8_t*)order1->wind_code ,(uint8_t*)order1->wind_code,10)==0);
}

bool TestUtils::CompareMarketData(L1MarketSnapshot marketData1,L1MarketSnapshot marketData2)
{
  return isValueEqual(marketData1.ask_price1,marketData2.ask_price1)
    && isValueEqual(marketData1.ask_price2,marketData2.ask_price2)
    && isValueEqual(marketData1.ask_price3,marketData2.ask_price3)
    && isValueEqual(marketData1.ask_price4,marketData2.ask_price4)
    && isValueEqual(marketData1.ask_price5,marketData2.ask_price5)
    && marketData1.ask_volume1==marketData2.ask_volume1
    && marketData1.ask_volume2==marketData2.ask_volume2
    && marketData1.ask_volume3==marketData2.ask_volume3
    && marketData1.ask_volume4==marketData2.ask_volume4
    && marketData1.ask_volume5==marketData2.ask_volume5
    && isValueEqual(marketData1.bid_price1,marketData2.bid_price1)
    && isValueEqual(marketData1.bid_price2,marketData2.bid_price2)
    && isValueEqual(marketData1.bid_price3,marketData2.bid_price3)
    && isValueEqual(marketData1.bid_price4,marketData2.bid_price4)
    && isValueEqual(marketData1.bid_price5,marketData2.bid_price5)
    && marketData1.bid_volume1==marketData2.bid_volume1
    && marketData1.bid_volume2==marketData2.bid_volume2
    && marketData1.bid_volume3==marketData2.bid_volume3
    && marketData1.bid_volume4==marketData2.bid_volume4
    && marketData1.bid_volume5==marketData2.bid_volume5
    && isValueEqual(marketData1.new_price,marketData2.new_price)
    && isValueEqual(marketData1.prev_close,marketData2.prev_close)
    && isValueEqual(marketData1.today_high,marketData2.today_high)
    && isValueEqual(marketData1.today_low,marketData2.today_low)
    && isValueEqual(marketData1.today_open,marketData2.today_open)
    && isValueEqual(marketData1.total_amount,marketData2.total_amount)
    && marketData1.trade_date==marketData2.trade_date
    && marketData1.trade_time==marketData2.trade_time
    && marketData1.volume==marketData2.volume
    && (memcmp((uint8_t*)marketData1.wind_code ,(uint8_t*)marketData2.wind_code,10)==0);
}

bool TestUtils::CompareMarketDataL2(L2MarketSnapshot marketData1,L2MarketSnapshot marketData2)
{
  return isValueEqual(marketData1.ask_price1,marketData2.ask_price1)
    && isValueEqual(marketData1.ask_price2,marketData2.ask_price2)
    && isValueEqual(marketData1.ask_price3,marketData2.ask_price3)
    && isValueEqual(marketData1.ask_price4,marketData2.ask_price4)
    && isValueEqual(marketData1.ask_price5,marketData2.ask_price5)
    && isValueEqual(marketData1.ask_price6,marketData2.ask_price6)
    && isValueEqual(marketData1.ask_price7,marketData2.ask_price7)
    && isValueEqual(marketData1.ask_price8,marketData2.ask_price8)
    && isValueEqual(marketData1.ask_price9,marketData2.ask_price9)
    && isValueEqual(marketData1.ask_price10,marketData2.ask_price10)
    && marketData1.ask_volume1==marketData2.ask_volume1
    && marketData1.ask_volume2==marketData2.ask_volume2
    && marketData1.ask_volume3==marketData2.ask_volume3
    && marketData1.ask_volume4==marketData2.ask_volume4
    && marketData1.ask_volume5==marketData2.ask_volume5
    && marketData1.ask_volume6==marketData2.ask_volume6
    && marketData1.ask_volume7==marketData2.ask_volume7
    && marketData1.ask_volume8==marketData2.ask_volume8
    && marketData1.ask_volume9==marketData2.ask_volume9
    && marketData1.ask_volume10==marketData2.ask_volume10
    && isValueEqual(marketData1.bid_price1,marketData2.bid_price1)
    && isValueEqual(marketData1.bid_price2,marketData2.bid_price2)
    && isValueEqual(marketData1.bid_price3,marketData2.bid_price3)
    && isValueEqual(marketData1.bid_price4,marketData2.bid_price4)
    && isValueEqual(marketData1.bid_price5,marketData2.bid_price5)
    && isValueEqual(marketData1.bid_price1,marketData2.bid_price6)
    && isValueEqual(marketData1.bid_price2,marketData2.bid_price7)
    && isValueEqual(marketData1.bid_price3,marketData2.bid_price8)
    && isValueEqual(marketData1.bid_price4,marketData2.bid_price9)
    && isValueEqual(marketData1.bid_price5,marketData2.bid_price10)
    && marketData1.bid_volume1==marketData2.bid_volume1
    && marketData1.bid_volume2==marketData2.bid_volume2
    && marketData1.bid_volume3==marketData2.bid_volume3
    && marketData1.bid_volume4==marketData2.bid_volume4
    && marketData1.bid_volume5==marketData2.bid_volume5
    && marketData1.bid_volume6==marketData2.bid_volume6
    && marketData1.bid_volume7==marketData2.bid_volume7
    && marketData1.bid_volume8==marketData2.bid_volume8
    && marketData1.bid_volume9==marketData2.bid_volume9
    && marketData1.bid_volume10==marketData2.bid_volume10
    && marketData1.volume==marketData2.volume
    && isValueEqual(marketData1.new_price,marketData2.new_price)
    && isValueEqual(marketData1.prev_close,marketData2.prev_close)
    && isValueEqual(marketData1.today_high,marketData2.today_high)
    && isValueEqual(marketData1.today_low,marketData2.today_low)
    && isValueEqual(marketData1.today_open,marketData2.today_open)

    && isValueEqual(marketData1.total_amount,marketData2.total_amount)
    && isValueEqual(marketData1.weighted_average_best_bid,marketData2.weighted_average_best_bid)
    && isValueEqual(marketData1.weighted_average_best_ask,marketData2.weighted_average_best_ask)

    && marketData1.trade_date==marketData2.trade_date
    && marketData1.trade_time==marketData2.trade_time
    && marketData1.transactions_count==marketData2.transactions_count
//    && marketData1.trading_phase_code==marketData2.trading_phase_code
    //&&(memcmp((uint8_t*)marketData1.security_pre_name ,(uint8_t*)marketData2.security_pre_name,4)==0)
    && marketData1.l2_total_bid_qty==marketData2.l2_total_bid_qty
    && marketData1.l2_total_offer_qty==marketData2.l2_total_offer_qty
    && (memcmp((uint8_t*)marketData1.wind_code ,(uint8_t*)marketData2.wind_code,10)==0);
}

bool TestUtils::CompareTransaction(L2Transaction trans1,L2Transaction trans2)
{
  return trans1.trade_channel==trans2.trade_channel
    && trans1.rec_no==trans2.rec_no
    && trans1.set_no==trans2.set_no
    && trans1.buyOrderRecNo==trans2.buyOrderRecNo
    && trans1.sellOrderRecNo==trans2.sellOrderRecNo
    && trans1.function_code==trans2.function_code
    && trans1.order_kind==trans2.order_kind
    && trans1.trade_money==trans2.trade_money
    && trans1.trade_qty==trans2.trade_qty
    && trans1.trade_time==trans2.trade_time
    && isValueEqual(trans1.price,trans2.price)
    && (memcmp((uint8_t*)trans1.wind_code ,(uint8_t*)trans2.wind_code,10)==0);
}

bool TestUtils::CompareLoginRequest(LoginRequest request1,LoginRequest request2)
{
  if(request1.product_count==request2.product_count
    &&(memcmp((uint8_t*)request1.user_name ,(uint8_t*)request2.user_name,32)==0)
    &&(memcmp((uint8_t*)request1.password ,(uint8_t*)request2.password,16)==0))
  {
    for(int i=0;i<request1.product_count;i++)
    {
      if(request1.product[i].codec==request2.product[i].codec
        &&request1.product[i].major_version==request2.product[i].major_version
        &&request1.product[i].minor_version==request2.product[i].minor_version
        &&request1.product[i].product_id==request2.product[i].product_id)
      {
        continue;
      }
      else
      {
        return false;
      }
    }
    return true;
  }
  else return false;
}

bool TestUtils::CompareLoginResponse(LoginResponse* response1,LoginResponse* response2)
{
  if(response1->server_count==response2->server_count
    && response1->user_id==response2->user_id
    && response1->product_count==response2->product_count
    && response1->ack_type==response2->ack_type)
  {
    for(int i=0; i<response1->product_count;i++)
    {
      if(response1->product_status[i].codec==response2->product_status[i].codec
        &&response1->product_status[i].product_id==response2->product_status[i].product_id
        &&response1->product_status[i].product_status==response2->product_status[i].product_status
        &&response1->product_status[i].major_version==response2->product_status[i].major_version
        &&response1->product_status[i].minor_version==response2->product_status[i].minor_version
        &&response1->product_status[i].message_count==response2->product_status[i].message_count
        )
       {
          for(int j=0;j<response1->product_status[i].message_count;j++)
          {
            if(response1->product_status[i].message_info[j].message_id==response2->product_status[i].message_info[j].message_id
              &&response1->product_status[i].message_info[j].message_status==response2->product_status[i].message_info[j].message_status)
            {
              continue;
            }
            else
              return false;
          }
        }
      else
        return false;
    }
    for(int i=0;i<response1->server_count;i++)
    {
      if((memcmp((uint8_t*)response1->serverinfo[i].server_addr ,(uint8_t*)response1->serverinfo[i].server_addr,15)==0)
        &&response1->serverinfo[i].server_port==response2->serverinfo[i].server_port)
      {
        continue;
      }
      else
      {
        return false;
      }
    }
    return true;
  }
  else
  {
    return false;
  }
}


bool TestUtils::CompareLogout(Logout* logout1,Logout* logout2)
{
  return logout1->logout_type==logout2->logout_type;
}
bool TestUtils::CompareCFFEL1MarketData(FuturesMarketSnapshot marketData1,FuturesMarketSnapshot marketData2)
{
  return isValueEqual(marketData1.ask_price1,marketData2.ask_price1)
    && marketData1.ask_volume1==marketData2.ask_volume1
    && isValueEqual(marketData1.bid_price1,marketData2.bid_price1)
    && marketData1.bid_volume1==marketData2.bid_volume1
    && isValueEqual(marketData1.high_limit,marketData2.high_limit)
    && isValueEqual(marketData1.low_limit,marketData2.low_limit)
    && isValueEqual(marketData1.new_price,marketData2.new_price)
    && isValueEqual(marketData1.prev_close,marketData2.prev_close)
    && marketData1.prev_position==marketData2.prev_position
    && isValueEqual(marketData1.prev_settle,marketData2.prev_settle)
    && isValueEqual(marketData1.today_high,marketData2.today_high)
    && isValueEqual(marketData1.today_low,marketData2.today_low)
    && isValueEqual(marketData1.today_open,marketData2.today_open)
    && isValueEqual(marketData1.amount,marketData2.amount)
    && marketData1.trade_date==marketData2.trade_date
    && marketData1.trade_time==marketData2.trade_time
    && marketData1.volume==marketData2.volume
    && (memcmp((uint8_t*)marketData1.wind_code ,(uint8_t*)marketData2.wind_code,10)==0);
}
}
#endif
