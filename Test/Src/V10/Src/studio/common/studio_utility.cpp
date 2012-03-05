#include "studio_utility.h"
#include <iostream>

#ifdef linux
#include <unistd.h>
#define MAX_PATH 260
#endif

#ifdef _WIN32
#include <io.h>
#endif

#include "../monitor/server_info.h"
#include "../monitor/server_manager.h"
#include "../../environment/utils.h"
#include "../../environment/application_directory.h"
#include "../../environment/strutils.h"

wmdf::StudioUtility::StudioUtility(void)
{
}

wmdf::StudioUtility::~StudioUtility(void)
{
}

TransferDataStat* wmdf::StudioUtility::CopyTransferStat(TransferDataStat* stat)
{
  TransferDataStat* status_copy = new TransferDataStat();
  memcpy(status_copy,stat,sizeof(TransferDataStat));
  return status_copy;
}

ClientStatus* wmdf::StudioUtility::CopyClientStatus(ClientStatus* stat)
{
  ClientStatus* status_copy = new ClientStatus();
  memcpy(status_copy,stat,sizeof(ClientStatus));
  status_copy->product_infos = new ProductInfo[stat->product_count];
  memcpy(status_copy->product_infos,stat->product_infos,sizeof(ProductInfo)*stat->product_count);
  return status_copy;
}

MonitorEvent* wmdf::StudioUtility::CopyMonitorEvent( MonitorEvent* monitor_event )
{
  MonitorEvent* event_copy = new MonitorEvent();
  memcpy(event_copy,monitor_event,sizeof(MonitorEvent));
  return event_copy;
}


L1MarketSnapshot* wmdf::StudioUtility::CopyL1MarketSnapshot( L1MarketSnapshot* item )
{
  L1MarketSnapshot* copy_item = new L1MarketSnapshot();
  memcpy(copy_item,item,sizeof(L1MarketSnapshot));
  return copy_item;
}

L2MarketSnapshot* wmdf::StudioUtility::CopyL2MarketSnapshot( L2MarketSnapshot* item )
{
  L2MarketSnapshot* copy_item = new L2MarketSnapshot();
  memcpy(copy_item,item,sizeof(L2MarketSnapshot));
  return copy_item;
}


L2Transaction* wmdf::StudioUtility::CopyL2Transaction( L2Transaction* item )
{
  L2Transaction* copy_item = new L2Transaction();
  memcpy(copy_item,item,sizeof(L2Transaction));
  return copy_item;
}

FuturesMarketSnapshot* wmdf::StudioUtility::CopyFuturesMarketData( FuturesMarketSnapshot* item )
{
  FuturesMarketSnapshot* copy_item = new FuturesMarketSnapshot();
  memcpy(copy_item,item,sizeof(FuturesMarketSnapshot));
  return copy_item;
}

CFEL2MarketSnapshot* wmdf::StudioUtility::CopyCFFEL2MarketData( CFEL2MarketSnapshot* item )
{
  CFEL2MarketSnapshot* copy_item = new CFEL2MarketSnapshot();
  memcpy(copy_item,item,sizeof(CFEL2MarketSnapshot));
  return copy_item;
}

ProductInfo* wmdf::StudioUtility::GetProductInfo( ClientStatus* stat,int16_t product_id )
{
  if(stat==NULL)
    return NULL;
  for(int i = 0;i<stat->product_count;i++)
  {
    if(stat->product_infos[i].product_id == product_id)
    {
      return &stat->product_infos[i];
    }
  }
  return NULL;
}

bool wmdf::StudioUtility::IsFileExist( const char * path )
{
#ifdef WIN32
  return _access(path,0)==0;
#elif __linux__
  return access(path,0)==0;
#endif
}

bool wmdf::StudioUtility::CompareAndReset( int m,int& n )
{
  if(m==WMDF_CONSTS_32BIT_NIL_VALUE)
    return true;
  else
  {
    bool flag = (m==n);
    n=m;
    return flag;
  }
 
}

bool wmdf::StudioUtility::CompareAndReset( int64_t m,int64_t& n )
{
  if(m==WMDF_CONSTS_64BIT_NIL_VALUE)
    return true;
  else
  {
    bool flag = (m==n);
    n=m;
    return flag;
  }  
}

bool wmdf::StudioUtility::CompareAndReset( double m,double& n )
{
  if(m!=m)
    return true;
  else 
  {
    bool flag = (m==n);
    n=m;
    return flag;
  }
}

void wmdf::StudioUtility::SetStatus( SymbolModel* model,StudioMonitorStatus status )
{
  model->symbol_status = (StudioMonitorStatus)((int)model->symbol_status | status);
}

void wmdf::StudioUtility::UnsetStatus( SymbolModel* model,StudioMonitorStatus status )
{
  model->symbol_status = (StudioMonitorStatus)((int)model->symbol_status &~status);
}

void wmdf::StudioUtility::SetProductId( SymbolModel* model,int16_t productId )
{
  model->product_id = productId;
}

void wmdf::StudioUtility::InitServerList(CComboBox& combox)
{
  ServerNetInfo* server_info = NULL;
  combox.InsertString(0,"本地服务");
  for(int i =0;i<ServerManager::Instance()->net_info_collection()->server_info_count();i++)
  {
    server_info = ServerManager::Instance()->net_info_collection()->server_infos(i);
    combox.InsertString(i+1,StrUtils::formatstring(30,"%d-%s:%d",server_info->server_id(),server_info->ip().c_str(),server_info->quote_port()).c_str());
  }
  combox.SetCurSel(0);
}

bool wmdf::StudioUtility::GetConfigInfo( int server_index, ConfigFileType configType,Persistable* persistable )
{
  if(server_index ==0)
  {
    switch(configType)
    {
    case CFT_SYSTEM:
      Utils::Deserialize(ApplicationDirectory::SystemDescriptionFile(),persistable);
      break;
    case CFT_USER:
      Utils::Deserialize(ApplicationDirectory::UserDescriptionFile(),persistable);
      break;
    case CFT_PRODUCT:
      Utils::Deserialize(ApplicationDirectory::ProductDescriptionFile(),persistable);
      break;
    case CFT_SOURCE:
      Utils::Deserialize(ApplicationDirectory::SourceDescriptionFile(),persistable);
      break;
    default:
      break;
    }
    
  }
  else
  {
    ServerNetInfo* server_info = ServerManager::Instance()->net_info_collection()->server_infos(server_index-1);
    ConfigInfoRequset request = {0};
    request.config_type = configType;
    request.connection_id = server_info->server_id();
    ConfigInfo* config = wmdf_request_configinfo(&request);
    if(config==NULL||config->length==0)
    {
      return false;
    }
    Utils::Deserialize((char*)config->bytes,persistable);
    free(config);
  }
  return true;
}

bool wmdf::StudioUtility::SetConfigInfo(int server_index,ConfigFileType configType,Persistable* persistable)
{
  if(server_index ==0)
  {
    switch(configType)
    {
      case CFT_SYSTEM:
        Utils::Serialize(ApplicationDirectory::SystemDescriptionFile(), persistable);
        break;
      case CFT_USER:
        Utils::Serialize(ApplicationDirectory::UserDescriptionFile(), persistable);
        break;
      case CFT_PRODUCT:
        Utils::Serialize(ApplicationDirectory::ProductDescriptionFile(), persistable);
        break;
      case CFT_SOURCE:
        Utils::Serialize(ApplicationDirectory::SourceDescriptionFile(), persistable);
        break;
      default:
        break;
    }
    return true;
  }
  else
  {
    ServerNetInfo* server_info = ServerManager::Instance()->net_info_collection()->server_infos(server_index-1);
    string xml = Utils::Serialize(persistable);

    ConfigInfo* config_info = (ConfigInfo*)malloc(sizeof(ConfigInfo)+xml.size());
    config_info->config_type = configType;
    config_info->connection_id = server_info->server_id();
    config_info->length = xml.size();
    memcpy(config_info->bytes,xml.c_str(),xml.size());
    eReturnValue return_value = wmdf_set_configinfo(config_info);
    free(config_info);
    return SUCCESS==return_value;
  }
}



