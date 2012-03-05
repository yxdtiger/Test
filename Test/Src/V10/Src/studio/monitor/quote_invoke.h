#ifndef QUOTE_INVOKE_H
#define QUOTE_INVOKE_H
#include "../main_frame/StdAfx.h"
#include <vector>
#include <map>
#include "base_invoke.h"
#include "server_manager.h"
#include "../../include/wmdf_structs.h"


class QuoteInvoke:public BaseInvoke
{
public:
  QuoteInvoke(int32_t conn_id,HWND handle);
  virtual ~QuoteInvoke(void);

  virtual void Connect();
  virtual void Disconnect();
  virtual void OnRecordCallback(RecordArgs* args);
  void SetFilter(std::string wind_code){wind_code_ = wind_code;}  
private:
  void ProcessL1MarketData(L1MarketSnapshot* market_datas,int count);
  void ProcessL2MarketData(L2MarketSnapshot* market_datas,int count);
  void ProcessL2Transaction(L2Transaction* trans,int count);
  void ProcessFuturesMarketSnapshot(FuturesMarketSnapshot* trans,int count);
  void ProcessCFFEL2MarketSnapshot(CFEL2MarketSnapshot* trans,int count);

  std::string wind_code_;
  int32_t conn_id_;
  HWND handle_;
};


#endif


