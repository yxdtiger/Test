#include "trading_time_manager.h"

using namespace wmdf;
wmdf::TradingTimeManager::TradingTimeManager(void)
{
}

wmdf::TradingTimeManager::~TradingTimeManager(void)
{
  std::map<int16_t,ProductTradingInfo*> ::iterator iter = product_trading_times_.begin();
  while(iter!=product_trading_times_.end())
  {
    if(NULL !=iter->second)
    {
      delete iter->second;
      iter->second = NULL;
    }
    iter++;
  }
}

bool wmdf::TradingTimeManager::LoadState( Memento* m )
{
  int child_num=m->GetChildCount();
  for(int i=0;i<child_num;i++)
  {
    Memento* sub_m=m->GetChild(i);
    ProductTradingInfo* pt=new ProductTradingInfo();
    pt->LoadState(sub_m);
    product_trading_times_.insert(make_pair(pt->product_id(),pt));
  }
  return true;
}

bool wmdf::TradingTimeManager::SaveState( Memento* m )
{
  m->SetName("MarketInfo");
  std::map<int16_t,ProductTradingInfo*> ::iterator iter = product_trading_times_.begin();
  while(iter!=product_trading_times_.end())
  {
    Memento* sub_m=m->CreateChild("ProductInfo");
    iter->second->SaveState(sub_m);
    iter++;
  }
  return true;
}

ProductTradingInfo* wmdf::TradingTimeManager::GetTradingTime( int16_t product_id )
{
  std::map<int16_t,ProductTradingInfo*> ::iterator iter = product_trading_times_.find(product_id);
  if(iter != product_trading_times_.end())
  {
    return iter->second;
  }
  return NULL;
}

bool wmdf::TradingTimeManager::IsTrading( int16_t product_id )
{
  if(product_id<=0)
    return true;
  ProductTradingInfo* trading_time = GetTradingTime(product_id);
  if(trading_time==NULL)
    return true;
  return trading_time->IsTrading();
}
