#include "product_trading_time.h"
#include "../../include/date_time.h"

using namespace wmdf;
wmdf::TradingTime::TradingTime(void)
{
}

wmdf::TradingTime::~TradingTime(void)
{
}

bool wmdf::TradingTime::LoadState( Memento* m )
{
  begin_time_=m->GetInteger("BeginTime");
  end_time_=m->GetInteger("EndTime");
  return true;
}

bool wmdf::TradingTime::SaveState( Memento* m )
{
  m->SetName("TradingTime");
  m->SetInteger("BeginTime",begin_time_);
  m->SetInteger("EndTime",end_time_);
  return true;
}

wmdf::ProductTradingInfo::ProductTradingInfo(void)
{
}

wmdf::ProductTradingInfo::~ProductTradingInfo(void)
{
  for (int i=0;i<trading_times_.size();i++)
  {
    delete trading_times_[i];
  }
  trading_times_.clear();
}

bool wmdf::ProductTradingInfo::LoadState( Memento* m )
{
  product_id_ =m->GetInteger("ProductID");
  //time_span_ =m->GetInteger("TimeSpan");
  int child_num=m->GetChildCount();  
  for(int i=0;i<child_num;i++)
  {
    Memento* sub_m=m->GetChild(i);
    TradingTime* pt=new TradingTime();
    pt->LoadState(sub_m);
    trading_times_.push_back(pt);
  }
  return true;
}

bool wmdf::ProductTradingInfo::SaveState( Memento* m )
{
  m->SetName("ProductInfo");
  m->SetInteger("ProductID",product_id_);
  //m->SetInteger("TimeSpan",time_span_);
  for(uint32_t i=0;i<trading_times_.size();i++)
  {
    Memento* sub_m=m->CreateChild("TradingTime");
    trading_times_[i]->SaveState(sub_m);
  }
  return true;
}

bool wmdf::ProductTradingInfo::IsTrading()
{
  TradingTime* trading_time = NULL;
  int32_t curr_time = DateTime::time2hms(DateTime::get_current_time()); 
  for (int i=0;i<trading_times_.size();i++)
  {
    trading_time = trading_times_[i];
    if(trading_time->begin_time()<=curr_time&&trading_time->end_time()>=curr_time)
      return true;
  }
  return false;
}
