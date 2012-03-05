#ifndef STUDIO_UTILITY_H
#define STUDIO_UTILITY_H
#include "../../include/wmdf_api.h"
#include "../../monitor/logger.h"
#include "../../include/wmdf_structs.h"
#include "persistable.h"
#include "../../include/wmdf_internal_api.h"
#include <string>
#include "afxwin.h"

namespace wmdf
{
  typedef enum StudioMonitorStatus
  {
    SMP_OK = 0,
    SMP_Disconnect = 0x01,
    SMP_NoReceive = 0x02,
    SMP_NoSend    = 0x04,
    SMP_Block     = 0x08,
    SMP_Delay     = 0x10
  }StudioMonitorStatus;

  typedef enum StudioSymbolType
  {
    SST_Source = 1,
    SST_Server = 2,
    SST_Client = 3,
  }StudioSymbolType;

  typedef struct SymbolModel
  {
    std::string symbol_name;
    int16_t product_id;
    StudioSymbolType symbol_type;
    StudioMonitorStatus symbol_status;
    double flow;
  }SymbolModel;

  typedef struct WarningInfo
  {
    std::string key;
    int16_t product_id;
    StudioMonitorStatus monitor_point;   //¼à¿Øµã
    LogLevel warningLevel;    
  }WarningInfo;

  class StudioUtility
  {
  public:
    StudioUtility(void);
    virtual ~StudioUtility(void);    

    static bool IsFileExist(const char * path); 
    static bool CompareAndReset(int m,int& n);
    static bool CompareAndReset(int64_t m,int64_t& n);
    static bool CompareAndReset(double m,double& n);
    static TransferDataStat* CopyTransferStat(TransferDataStat* stat);
    static ClientStatus* CopyClientStatus(ClientStatus* stat);
    static MonitorEvent* CopyMonitorEvent(MonitorEvent* monitor_event);
    static L1MarketSnapshot* CopyL1MarketSnapshot(L1MarketSnapshot* item);
    static L2MarketSnapshot* CopyL2MarketSnapshot(L2MarketSnapshot* item);
    static L2Transaction* CopyL2Transaction(L2Transaction* item);
    static FuturesMarketSnapshot* CopyFuturesMarketData(FuturesMarketSnapshot* item);
    static CFEL2MarketSnapshot* CopyCFFEL2MarketData(CFEL2MarketSnapshot* item);
    
    static void SetStatus(SymbolModel* model,StudioMonitorStatus status);
    static void UnsetStatus(SymbolModel* model,StudioMonitorStatus status);
    static void SetProductId(SymbolModel* model,int16_t productId);
    static ProductInfo* GetProductInfo(ClientStatus* stat,int16_t product_id);
    static void InitServerList(CComboBox& combox);
    static bool GetConfigInfo(int server_index, ConfigFileType configType,Persistable* persistable);
    static bool SetConfigInfo(int server_index, ConfigFileType configType,Persistable* persistable);
  };
}

#endif
