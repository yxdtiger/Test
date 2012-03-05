#ifndef WMDF_INVOKE_H
#define WMDF_INVOKE_H
#include <vector>
#include <map>

#include "../../include/wmdf_structs.h"
#include "server_manager.h"
#include "base_invoke.h"


class MonitorInvoke:public BaseInvoke
{
public:
  MonitorInvoke();
  virtual ~MonitorInvoke(void);

  virtual void Connect();
  virtual void Disconnect();
  virtual void ConnectOne(int32_t conn_id);
  virtual void DisconnectOne(int32_t conn_id);
  virtual void OnRecordCallback(RecordArgs* args);
private:
  void ProcessTransferDataStats(int32_t conn_id,TransferDataStat* status,int count);

  void CheckTransferInfo( int count, TransferDataStat* status, wmdf::ServerNetInfo* server_net_info, int32_t conn_id );

  void ProcessClientStats(int32_t conn_id,ClientStatus* status,int count);

  void CheckClientInfo( int count, ClientStatus* status, wmdf::ServerNetInfo* server_net_info, int32_t conn_id );

  void ProcessEventLog(int32_t conn_id,MonitorEvent* events,int count);

  //void UpdateSymbolStatus(int32_t conn_id, string symbol_name,bool status);

};

#endif


