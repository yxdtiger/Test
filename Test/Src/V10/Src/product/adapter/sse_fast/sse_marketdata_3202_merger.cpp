#include "sse_marketdata_3202_merger.h"
#include "../../../include/date_time.h"
#include "../common/rawdata_object.h"
#include <assert.h>
#include <algorithm>
#ifdef __linux__
#include <cstring>
#endif

wmdf::SSE_MarketData_3202_Merger::SSE_MarketData_3202_Merger(void)
{
  order_queue_count_ = 0;
}

wmdf::SSE_MarketData_3202_Merger::~SSE_MarketData_3202_Merger(void)
{
  Clear();
}

void wmdf::SSE_MarketData_3202_Merger::ProcessMarketSnapshot( MarketData_3202* marketdata_3202,L2MarketSnapshot* out )
{

  std::string windcode = marketdata_3202->windcode;
  std::map<std::string,MarketDepth* >::iterator iter = market_depths_.find(windcode);
  if(iter == market_depths_.end())
  {
    MarketDepth* md = new MarketDepth();
    market_depths_.insert(std::pair<std::string,MarketDepth*>(windcode,md));
    ProcessMarketSnapshot(marketdata_3202,*md,out);
  }
  else
  {
    MarketDepth *md = iter->second;
    ProcessMarketSnapshot(marketdata_3202,*md,out);
  }
}

void wmdf::SSE_MarketData_3202_Merger::ProcessMarketSnapshot( MarketData_3202* src,MarketDepth& dst,L2MarketSnapshot* out )
{
  out->new_price = src->last_price;
  out->pre_close = src->preclose_price;
  out->open_price = src->open_price;
  out->high_price = src->high_price;
  out->low_price = src->low_price;
  out->total_volume = src->total_volume_trade;
  out->total_amount = src->total_value_trade;
  out->transactions_count = src->num_trades;
  out->weighted_average_bid_price = src->weighted_avg_bid_price;
  out->weighted_average_ask_price = src->weighted_avg_offer_price;
  out->total_bid_size = src->total_bid_qty;
  out->total_ask_size = src->total_offer_qty;
  if(strcmp((char*)src->instrument_status,"SUSP")==0)
    out->trade_status = 2;
  else if(strcmp((char*)src->instrument_status,"")!=0)
    out->trade_status = 1;

  if(src->iopv == src->iopv)
    out->peratio2 = src->iopv;
  else if(src->yield_to_maturity == src->yield_to_maturity)
    out->peratio2 = src->yield_to_maturity;

  if (src->image_status == IS_FULL)
  {
    ProcessFullImage(src,dst,out);
  }
  else if (src->image_status == IS_UPDATE)
  {
    ProcessUpdateImage(src,dst,out);
  }
}

void wmdf::SSE_MarketData_3202_Merger::ProcessFullImage( MarketData_3202* src,MarketDepth& dst,L2MarketSnapshot* out )
{
  ClearBidMarketDepth(dst);
  double* bidprice = &out->bid_price1;
  int32_t* bidsize = &out->bid_size1;
  bool orderqueue_flag_ = false;
  //assert(src->num_of_bid_level <=10);
  int32_t price_level_count = (src->num_of_bid_level > 10)?10:src->num_of_bid_level;
  for(int32_t index=0;index< price_level_count;index++)
  {
    PriceLevel_3202* price_level= &src->bidlevels[index];
    sPriceLevel* pl = new sPriceLevel();
    pl->price = price_level->price;
    *bidprice = pl->price;
    *bidsize = pl->size = price_level->order_qty;
    dst.bid_price_levels_.push_back(pl);
    if (price_level->no_order > 0)
    {
      orderqueue_flag_ = true;
    }
    bidprice++;
    bidsize++;
  }
  for (int32_t index=price_level_count;index< 10;index++)
  {
    *bidprice = 0;
    *bidsize =0;
    bidprice++;
    bidsize++;
  }
  if (orderqueue_flag_)
  {
    order_queue_count_++;
  }

  ClearAskMarketDepth(dst);
  double* askprice = &out->ask_price1;
  int32_t* asksize = &out->ask_size1;
  orderqueue_flag_ = false;
  //assert(src->num_of_offer_level <=10);
  price_level_count = (src->num_of_offer_level > 10)?10:src->num_of_offer_level;
  for(int32_t index=0;index < price_level_count;index++)
  {
    PriceLevel_3202* price_level= &src->offerlevels[index];
    sPriceLevel* pl = new sPriceLevel();
    pl->price = price_level->price;
    *askprice = pl->price ;
    *asksize = pl->size = price_level->order_qty;
    dst.ask_price_levels_.push_back(pl);
    if (price_level->no_order > 0)
    {
      orderqueue_flag_ = true;
    }
    askprice++;
    asksize++;
  }
  for (int32_t index=price_level_count;index< 10;index++)
  {
    *askprice = 0;
    *asksize =0;
    askprice++;
    asksize++;
  }
  if (orderqueue_flag_)
  {
    order_queue_count_++;
  }
}

bool lessmark(const wmdf::sPriceLevel* s1,const wmdf::sPriceLevel* s2)
{
  return s1->price < s2->price;
}
bool greatermark(const wmdf::sPriceLevel* s1,const wmdf::sPriceLevel* s2)
{
  return s1->price > s2->price;
}
void wmdf::SSE_MarketData_3202_Merger::ProcessUpdateImage( MarketData_3202* src,MarketDepth& dst,L2MarketSnapshot* out )
{
  //handle bid side
  bool orderqueue_flag = false;
  if (src->num_of_bid_level > 0)
  {
    for(int32_t index=0;index < src->num_of_bid_level;index++)
    {
      PriceLevel_3202* price_level= &src->bidlevels[index];
      ProcessUpdatePriceLevel(price_level,dst.bid_price_levels_);
      if (price_level->no_order>0)
      {
        orderqueue_flag = true;
      }
    }

    sort(dst.bid_price_levels_.begin(), dst.bid_price_levels_.end(),greatermark) ;
    if (orderqueue_flag)  {  order_queue_count_++;  }
    //maybe need sort;
    double* bidprice = &out->bid_price1;
    int32_t* bidsize = &out->bid_size1;
    //assert(dst.bid_price_levels_.size() <= 10);
    uint32_t price_level_count = (dst.bid_price_levels_.size() > 10)?10:dst.bid_price_levels_.size();
    for (uint32_t index = 0;index < price_level_count;index++)
    {
      *bidprice = dst.bid_price_levels_[index]->price;
      *bidsize = dst.bid_price_levels_[index]->size;
      bidprice++;
      bidsize++;
    }
    for (int32_t index=price_level_count;index< 10;index++)
    {
      *bidprice = 0;
      *bidsize =0;
      bidprice++;
      bidsize++;
    }
  }

  if (src->num_of_offer_level > 0)
  {
    // handle ask side;
    orderqueue_flag = false;
    for(int32_t index=0;index < src->num_of_offer_level;index++)
    {

      PriceLevel_3202* price_level= &src->offerlevels[index];
      ProcessUpdatePriceLevel(price_level,dst.ask_price_levels_);
      if (price_level->no_order>0)
      {
        orderqueue_flag = true;
      }
    }
    sort(dst.ask_price_levels_.begin(), dst.ask_price_levels_.end(),lessmark) ;
    if (orderqueue_flag)  {  order_queue_count_++;  }
    //maybe need sort;
    double* askprice = &out->ask_price1;
    int32_t* asksize = &out->ask_size1;
    //assert(dst.ask_price_levels_.size() <= 10);
    uint32_t price_level_count = (dst.ask_price_levels_.size() > 10)?10:dst.ask_price_levels_.size();
    for (uint32_t index = 0;index < price_level_count;index++)
    {
      *askprice = dst.ask_price_levels_[index]->price;
      *asksize = dst.ask_price_levels_[index]->size;
      askprice++;
      asksize++;
    }
    for (int32_t index=price_level_count;index< 10;index++)
    {
      *askprice = 0;
      *asksize =0;
      askprice++;
      asksize++;
    }
  }

}


void wmdf::SSE_MarketData_3202_Merger::ProcessUpdatePriceLevel( PriceLevel_3202* src, std::vector<sPriceLevel*>& dst )
{
  if (src->price_level_operator == EPO_DELETE)
  {
    std::vector<sPriceLevel*>::iterator it = dst.begin();
    while (it != dst.end())
    {
      if ((*it)->price == src->price)
      {
        delete *it;
        it = dst.erase(it);
      }
      else
        it++;
    }
  }
  else if (src->price_level_operator == EPO_UPDATE)
  {
    for (uint32_t subindex=0;subindex < dst.size();subindex++)
    {
      if (dst[subindex]->price == src->price)
      {
        dst[subindex]->size = src->order_qty;
        break;
      }
    }
  }
  else if (src->price_level_operator == EPO_ADD)
  {
    sPriceLevel* pl = new sPriceLevel();
    pl->price = src->price;
    pl->size = src->order_qty;
    dst.push_back(pl);
  }

}

void wmdf::SSE_MarketData_3202_Merger::ClearBidMarketDepth( MarketDepth& market_depth )
{
  for (uint32_t index=0;index<market_depth.bid_price_levels_.size();index++)
  {
    delete market_depth.bid_price_levels_[index];
  }
  market_depth.bid_price_levels_.clear();
}

void wmdf::SSE_MarketData_3202_Merger::ClearAskMarketDepth( MarketDepth& market_depth )
{
  for (uint32_t index=0;index<market_depth.ask_price_levels_.size();index++)
  {
    delete market_depth.ask_price_levels_[index];
  }
  market_depth.ask_price_levels_.clear();
}

void wmdf::SSE_MarketData_3202_Merger::Clear()
{
  for(std::map<std::string,MarketDepth* >::iterator iter = market_depths_.begin();iter!=market_depths_.end();++iter)
  {
    MarketDepth *md = iter->second;
    if(md!=NULL)
    {
      ClearAskMarketDepth(*md);
      ClearBidMarketDepth(*md);
      delete md;
      md = NULL;
    }
  }
  market_depths_.clear();
}
