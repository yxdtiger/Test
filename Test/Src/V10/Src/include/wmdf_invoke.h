#ifndef WMDF_INVOKE_H
#define WMDF_INVOKE_H
#include "wmdf_api.h"
#include "wmdf_structs.h"

class WmdfInvoke
{
public:
  WmdfInvoke(void);
  virtual ~WmdfInvoke(void);
  void Connect();
  void Disconnect();
  void OnRecordCallback(MdfRecord* record);
  void OnEventCallback(EventArgs* args);
private:
  void ProcessL1MarketData(MdfRecord* record);
};


#endif


