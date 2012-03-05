#ifndef PRODUCT_TRADING_TIME_H
#define PRODUCT_TRADING_TIME_H

#include <string>
#include <vector>
#include "persistable.h"
#include "memento.h"
#include "../../include/wmdf_api.h"

namespace wmdf
{
  class TradingTime: public Persistable
  {
  public:
    TradingTime(void);
    virtual ~TradingTime(void);

    virtual bool LoadState(Memento* m);
    virtual bool SaveState(Memento* m);
    int32_t begin_time() {return begin_time_;}
    void set_begin_time(int32_t begin_time){begin_time_ = begin_time;}
    int32_t end_time() {return end_time_;}
    void set_end_time(int32_t end_time){end_time_ = end_time;}
  private:
    int32_t begin_time_;
    int32_t end_time_;
  };
  
  class ProductTradingInfo: public Persistable
  {
  public:
    ProductTradingInfo(void);
    virtual ~ProductTradingInfo(void);

    virtual bool LoadState(Memento* m);
    virtual bool SaveState(Memento* m);

    int32_t product_id() {return product_id_;}
    void set_product_id(int32_t id){product_id_ = id;}

    //int32_t time_span() {return time_span_;}
    //void set_time_span(int32_t span){time_span_ = span;}

    bool IsTrading();
  private:
    int16_t product_id_;
    //int time_span_;
    std::vector<TradingTime*> trading_times_;
    
  };
}

#endif
