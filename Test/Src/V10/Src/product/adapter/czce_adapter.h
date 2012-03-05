#ifndef CZCE_ADAPTER_H_
#define CZCE_ADAPTER_H_

#include "product_adapter.h"



namespace wmdf
{

#pragma pack(push)  /* push current alignment to stack */
#pragma pack(1)     /* set alignment to 1 byte boundary */

  typedef struct CZC_FuturesDataStruct_L2
  {
    char Length[3];						   //结构体长度

    char InstrumentId[20];       //合约编码

    char PreClose[12];					//前收盘价格		4
    char PreSettle[12];					//前结算价格		75
    char OpenPrice[12];					//开盘价			5

    char BidPrice[12];					//买入价格			11
    char BidLot[12];					  //买入数量			31
    char AskPrice[12];					//卖出价			21
    char AskLot[12];					  //卖出数量			41

    char LastPrice[12];					//最新价			3
    char OpenInterest[12];		  //持仓量			76

    char HighPrice[12];					//最高价			6
    char LowPrice[12];					//最低价			7
    char ClosePrice[12];				//收盘价			3(重复) X
    char SettlePrice[12];				//当日交割结算价	74
    char ClearPrice[12];				//结算价			X

    char AveragePrice[12];			//均价				79
    char LifeHigh[12];					//历史最高成交价格	X
    char LifeLow[12];					  //历史最低成交价格	X
    char HighLimit[12];					//涨停板			211
    char LowLimit[12];					//跌停板			212

    char TotalVolume[12];				//总成交量		8

  }STRUCT_CZC;

#pragma pack(pop)

  class CzceAdapter : public ProductAdapter
  {
  public:
    CzceAdapter(ProductSource* product_source);
    virtual ~CzceAdapter();
  protected:
    virtual uint32_t ParseMessage(const uint32_t length);    
    void PublishMessages();
    void ClearRecords();
  private:
    
    std::vector<STRUCT_CZC*> old_records_;
  };
}


#endif
