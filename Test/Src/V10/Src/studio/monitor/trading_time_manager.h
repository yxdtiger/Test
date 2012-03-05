#ifndef TRADING_TIME_MANAGER_H
#define TRADING_TIME_MANAGER_H

#include <map>
#include "product_trading_time.h"

namespace wmdf
{
  class TradingTimeManager:public Persistable
  {
  public:
    TradingTimeManager(void);
    virtual ~TradingTimeManager(void);

    virtual bool LoadState(Memento* m);
    virtual bool SaveState(Memento* m);

    ProductTradingInfo* GetTradingTime(int16_t product_id);
    bool IsTrading(int16_t product_id);
  private:
    std::map<int16_t,ProductTradingInfo*> product_trading_times_;
  };
}

#endif
