#ifndef SHFE_ADAPTER_H_
#define SHFE_ADAPTER_H_

#include "product_adapter.h"

namespace wmdf
{
  class Splitter;


#pragma pack(push)  /* push current alignment to stack */
#pragma pack(1)     /* set alignment to 1 byte boundary */

  typedef struct SHFE_FuturesDataStruct_L2
  {
    char InstrumentID[31];         // 合约代码

    char TradingDay[9];            // 交易日, yyyymmdd			1
    char UpdateTime[13];           // 最后修改时间, hhMMss		2
    int UpdateMillisec;            // 最后修改毫秒, mmm			X

    double LastPrice;              // 最新价					3
    double OpenPrice;              // 今开盘					5
    double HighestPrice;           // 最高价					6
    double LowestPrice;            // 最低价					7
    double LastMatchQty;           // 最新成交量				X
    double Volume;                 // 总成交量					8
    double Turnover;               // 总成交金额				59
    double ClosePrice;             // 今收盘					3（重复）	X
    double AvgPrice;               // 当日均价					79			X

    double PreSettlementPrice;     // 昨结算					75
    double SettlementPrice;        // 今结算					74			X?
    double PreClosePrice;          // 昨收盘					4
    double PreOpenInterest;        // 昨持仓量					77
    double OpenInterest;           // 最新持仓量				76
    double InterestChange;         // 持仓量变化				78			X

    double      BidPrice1;             // 申买价一				11
    double      BidVolume1;            // 申买量一				31
    double      AskPrice1;             // 申卖价一				21
    double      AskVolume1;            // 申卖量一				41


    double UpperLimitPrice;        // 涨停板价					211	
    double LowerLimitPrice;        // 跌停板价					212
  }STRUCT_SHFE;

#pragma pack(pop)


  class ShfeAdapter : public ProductAdapter
  {
  public:
    ShfeAdapter(ProductSource* product_source);
    virtual ~ShfeAdapter();
  protected:
    virtual uint32_t ParseMessage(const uint32_t length);    
    void PublishMessages();
    void ClearRecords();
    bool IsShfeCode(int32_t n);
  private:
    Splitter* splitter_;
    std::vector<STRUCT_SHFE*> old_records_;
  };
}


#endif
