#include "index_transfer.h"
#include "indicatorDefs.h"
#include "cmath"


wmdf::IndexTransfer::IndexTransfer(void)
{
}

wmdf::IndexTransfer::~IndexTransfer(void)
{
}

void wmdf::IndexTransfer::GetMarketSnapShot( L2MarketSnapshot* marketsnapshot)
{
  memcpy(windcode_,marketsnapshot->wind_code,16);
  int32_t i=0;

  if(marketsnapshot->trade_date != WMDF_CONSTS_32BIT_NIL_VALUE)
  {
    wnfields_[i]->nFieldId = IND_TRADEDATE;
    wnfields_[i]->bValueType = WMDF_VAL_TYPE_LONG;
    wnfields_[i]->Value.l = marketsnapshot->trade_date;
    i++;
  }

  if(marketsnapshot->trade_time != WMDF_CONSTS_32BIT_NIL_VALUE)
  {
    wnfields_[i]->nFieldId = IND_TRADETIME;
    wnfields_[i]->bValueType = WMDF_VAL_TYPE_LONG;
    wnfields_[i]->Value.l = marketsnapshot->trade_time;
    i++;
  }

  if(marketsnapshot->prev_close != WMDF_CONSTS_DOUBLE_NIL_VALUE)
  {
    wnfields_[i]->nFieldId = IND_PREVCLOSE;
    wnfields_[i]->bValueType = WMDF_VAL_TYPE_DOUBLE;
    wnfields_[i]->Value.d = marketsnapshot->prev_close;
    i++;
  }

  if(marketsnapshot->today_open != WMDF_CONSTS_DOUBLE_NIL_VALUE)
  {
    wnfields_[i]->nFieldId = IND_TODAYOPEN;
    wnfields_[i]->bValueType = WMDF_VAL_TYPE_DOUBLE;
    wnfields_[i]->Value.d = marketsnapshot->today_open;
    i++;
  }

  if(marketsnapshot->today_high != WMDF_CONSTS_DOUBLE_NIL_VALUE)
  {
    wnfields_[i]->nFieldId = IND_TODAYHIGH;
    wnfields_[i]->bValueType = WMDF_VAL_TYPE_DOUBLE;
    wnfields_[i]->Value.d = marketsnapshot->today_high;
    i++;
  }
  
  if(marketsnapshot->today_low != WMDF_CONSTS_DOUBLE_NIL_VALUE)
  {
    wnfields_[i]->nFieldId = IND_TODAYLOW;
    wnfields_[i]->bValueType = WMDF_VAL_TYPE_DOUBLE;
    wnfields_[i]->Value.d = marketsnapshot->today_low;
    i++;
  }

  if(marketsnapshot->new_price != WMDF_CONSTS_DOUBLE_NIL_VALUE)
  {
    wnfields_[i]->nFieldId = IND_NEWVALUE;
    wnfields_[i]->bValueType = WMDF_VAL_TYPE_DOUBLE;
    wnfields_[i]->Value.d = marketsnapshot->new_price;
    i++;
  }

  if(marketsnapshot->volume != WMDF_CONSTS_64BIT_NIL_VALUE)
  {
    wnfields_[i]->nFieldId = IND_TOTALVOLUME;
    wnfields_[i]->bValueType = WMDF_VAL_TYPE_UINT64;
    wnfields_[i]->Value.i = marketsnapshot->volume;
    i++;
  }

  if(marketsnapshot->total_amount != WMDF_CONSTS_DOUBLE_NIL_VALUE)
  {
    wnfields_[i]->nFieldId = IND_TOTALAMOUNT;
    wnfields_[i]->bValueType = WMDF_VAL_TYPE_DOUBLE;
    wnfields_[i]->Value.d = marketsnapshot->total_amount;
    i++;
  }

  if(marketsnapshot->bid_price1 != WMDF_CONSTS_DOUBLE_NIL_VALUE)
  {
    wnfields_[i]->nFieldId = IND_BIDPRICE1;
    wnfields_[i]->bValueType = WMDF_VAL_TYPE_DOUBLE;
    wnfields_[i]->Value.d = marketsnapshot->bid_price1;
    i++;
  }

  if(marketsnapshot->bid_price2 != WMDF_CONSTS_DOUBLE_NIL_VALUE)
  {
    wnfields_[i]->nFieldId = IND_BIDPRICE2;
    wnfields_[i]->bValueType = WMDF_VAL_TYPE_DOUBLE;
    wnfields_[i]->Value.d = marketsnapshot->bid_price2;
    i++;
  }

  if(marketsnapshot->bid_price3 != WMDF_CONSTS_DOUBLE_NIL_VALUE)
  {
    wnfields_[i]->nFieldId = IND_BIDPRICE3;
    wnfields_[i]->bValueType = WMDF_VAL_TYPE_DOUBLE;
    wnfields_[i]->Value.d = marketsnapshot->bid_price3;
    i++;
  }

  if(marketsnapshot->bid_price4 != WMDF_CONSTS_DOUBLE_NIL_VALUE)
  {
    wnfields_[i]->nFieldId = IND_BIDPRICE4;
    wnfields_[i]->bValueType = WMDF_VAL_TYPE_DOUBLE;
    wnfields_[i]->Value.d = marketsnapshot->bid_price4;
    i++;
  }

  if(marketsnapshot->bid_price5 != WMDF_CONSTS_DOUBLE_NIL_VALUE)
  {
    wnfields_[i]->nFieldId = IND_BIDPRICE5;
    wnfields_[i]->bValueType = WMDF_VAL_TYPE_DOUBLE;
    wnfields_[i]->Value.d = marketsnapshot->bid_price5;
    i++;
  }

  if(marketsnapshot->bid_price6 != WMDF_CONSTS_DOUBLE_NIL_VALUE)
  {
    wnfields_[i]->nFieldId = IND_BIDPRICE6;
    wnfields_[i]->bValueType = WMDF_VAL_TYPE_DOUBLE;
    wnfields_[i]->Value.d = marketsnapshot->bid_price6;
    i++;
  }

  if(marketsnapshot->bid_price7 != WMDF_CONSTS_DOUBLE_NIL_VALUE)
  {
    wnfields_[i]->nFieldId = IND_BIDPRICE7;
    wnfields_[i]->bValueType = WMDF_VAL_TYPE_DOUBLE;
    wnfields_[i]->Value.d = marketsnapshot->bid_price7;
    i++;
  }

  if(marketsnapshot->bid_price8 != WMDF_CONSTS_DOUBLE_NIL_VALUE)
  {
    wnfields_[i]->nFieldId = IND_BIDPRICE8;
    wnfields_[i]->bValueType = WMDF_VAL_TYPE_DOUBLE;
    wnfields_[i]->Value.d = marketsnapshot->bid_price8;
    i++;
  }

  if(marketsnapshot->bid_price9 != WMDF_CONSTS_DOUBLE_NIL_VALUE)
  {
    wnfields_[i]->nFieldId = IND_BIDPRICE9;
    wnfields_[i]->bValueType = WMDF_VAL_TYPE_DOUBLE;
    wnfields_[i]->Value.d = marketsnapshot->bid_price9;
    i++;
  }

  if(marketsnapshot->bid_price10 != WMDF_CONSTS_DOUBLE_NIL_VALUE)
  {
    wnfields_[i]->nFieldId = IND_BIDPRICE10;
    wnfields_[i]->bValueType = WMDF_VAL_TYPE_DOUBLE;
    wnfields_[i]->Value.d = marketsnapshot->bid_price10;
    i++;
  }


}

void wmdf::IndexTransfer::GetL2OrderQueue( L2OrderQueue* oq)
{
  memcpy(windcode_, oq->wind_code,16);
  orderqueue_->DateTimeStamp=oq->date_time_stamp;
  orderqueue_->ImageStatus=oq->image_status;
  orderqueue_->Side=oq->side;
  orderqueue_->NoPriceLevel=oq->price_level_count;
  for(int32_t i=0; i<oq->price_level_count;i++)
  {
    orderqueue_->PriceLevel[i].PriceLevelOperator=oq->order_queue_levels->price_level_operator;
    orderqueue_->PriceLevel[i].Price=oq->order_queue_levels->price;
    orderqueue_->PriceLevel[i].NumOfOrders=oq->order_queue_levels->orders_total;
    orderqueue_->PriceLevel[i].NoOrders=oq->order_queue_levels->orders_count;
    for(int32_t j=0;j<oq->order_queue_levels->orders_count;i++)
    {
      orderqueue_->PriceLevel[i].Orders[j].PriceLevelOperator=oq->order_queue_levels->order_queue_items->order_queue_operator;
      orderqueue_->PriceLevel[i].Orders[j].OrderQueueOperatorEntryID=oq->order_queue_levels->order_queue_items->order_queue_operator_entry_id;
      orderqueue_->PriceLevel[i].Orders[j].OrderQty=(uint32_t)oq->order_queue_levels->order_queue_items->order_qty;
    }
  }
}

void wmdf::IndexTransfer::GetL2Transactions( L2Transaction* transaction )
{
  memcpy(windcode_,transaction->wind_code,16);
  transaction_->TradeTime=transaction->trade_time;
  transaction_->TradeIndex=transaction->rec_no;
  transaction_->TradePrice=transaction->price;
  transaction_->TradeQty=(uint32_t)transaction->trade_qty;
  transaction_->TradeMoney=(uint32_t)transaction->trade_money;
}

void wmdf::IndexTransfer::GetSZSEL2Orders( SZSEL2Order* orders )
{
  memcpy(windcode_,orders->wind_code,16);
  orderitem_->RecNo=orders->rec_no;
  orderitem_->OrderPrice=orders->order_price;
  orderitem_->OrderQty=(uint32_t)orders->order_qty;
  orderitem_->OrderTime=orders->order_time;
  orderitem_->OrderType=orders->order_kind;
}






