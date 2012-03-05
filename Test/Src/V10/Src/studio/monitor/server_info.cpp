#include "server_info.h"
#include "../common/studio_utility.h"
#include "../../environment/utils.h"
#include "monitor_environment.h"

using namespace wmdf;

wmdf::ServerNetInfo::ServerNetInfo(void)
{
  status_ = 0;
  is_drawed_ = false;
  index_of_event_ = 0;
  server_id_ = -1;
}

wmdf::ServerNetInfo::~ServerNetInfo(void)
{
  ClearTransfersStatus();
  ClearClientsStatus();
  ClearMonitorEvent();
  ClearModels();
}

bool wmdf::ServerNetInfo::LoadState(Memento* m )
{
  ip_=m->GetString("IP");
  monitor_port_=m->GetInteger("MonitorPort");
  server_type_=m->GetInteger("ServerType");
  quote_port_=m->GetInteger("QuotePort");
  monitor_user_name_ = m->GetString("MonitorID");
  monitor_keyword_ = m->GetString("MonitorKey");
  quote_user_name_ = m->GetString("QuoteID");
  quote_keyword_ = m->GetString("QuoteKey");
  return true;
}

bool wmdf::ServerNetInfo::SaveState(Memento* m )
{
  m->SetName("ServerInfo");
  m->SetString("IP",ip_);
  m->SetInteger("MonitorPort",monitor_port_);
  m->SetInteger("ServerType",server_type_);
  m->SetInteger("QuotePort",quote_port_);
  m->SetString("MonitorID",monitor_user_name_);
  m->SetString("MonitorKey",monitor_keyword_);
  m->SetString("QuoteID",quote_user_name_);
  m->SetString("QuoteKey",quote_keyword_);
  return true;
}


void wmdf::ServerNetInfo::ClearTransfersStatus()
{
  boost::mutex::scoped_lock lock(transfer_mutex_);
  for (int i=0;i<transfer_statuses_.size();i++)
  {
    delete transfer_statuses_[i];
  }
  transfer_statuses_.clear();
}

void wmdf::ServerNetInfo::ClearMonitorEvent()
{
  boost::mutex::scoped_lock lock(event_mutex_);
  for (int i=0;i<monitor_events_.size();i++)
  {
    delete monitor_events_[i];
  }
  monitor_events_.clear();
}


void wmdf::ServerNetInfo::ClearClientsStatus()
{
  boost::mutex::scoped_lock lock(client_mutex_);
  for (int i=0;i<client_statuses_.size();i++)
  {
    Utils::DestroyClientStatus(client_statuses_[i]);
  }
  client_statuses_.clear();
}

void wmdf::ServerNetInfo::AddTransferStatus( TransferDataStat* status )
{
  boost::mutex::scoped_lock lock(transfer_mutex_);
  transfer_statuses_.push_back(status);
}

void wmdf::ServerNetInfo::AddMonitorEvent( MonitorEvent* monitor_event )
{
  boost::mutex::scoped_lock lock(event_mutex_);
  monitor_events_.push_back(monitor_event);
}

void wmdf::ServerNetInfo::AddClientStatus( ClientStatus* status )
{
  boost::mutex::scoped_lock lock(client_mutex_);
  client_statuses_.push_back(status);
}

void wmdf::ServerNetInfo::LockClientsStatus()
{
  client_mutex_.lock();
}

void wmdf::ServerNetInfo::UnlockClientsStatus()
{
  client_mutex_.unlock();
}


void wmdf::ServerNetInfo::LockTransfersStatus()
{
  transfer_mutex_.lock();
}

void wmdf::ServerNetInfo::UnlockTransfersStatus()
{
  transfer_mutex_.unlock();
}

wmdf::ServerNetInfo* wmdf::ServerNetInfo::Copy()
{
  ServerNetInfo* net_info = new ServerNetInfo();
  net_info->set_server_id(server_id_);
  net_info->set_ip(ip_);
  net_info->set_monitor_port(monitor_port_);
  net_info->set_server_type(server_type_);
  net_info->set_quote_port(quote_port_);
  net_info->set_status(status_);
  net_info->set_is_drawed(is_drawed_);
  net_info->set_index_of_event(index_of_event_);
  LockTransfersStatus();
  for (int i = 0;i<transfer_statuses_.size();i++)
  {
    net_info->AddTransferStatus(StudioUtility::CopyTransferStat(transfer_statuses_[i]));
  }
  UnlockTransfersStatus();

  LockClientsStatus();
  for (int i =0;i<client_statuses_.size();i++)
  {
    net_info->AddClientStatus(StudioUtility::CopyClientStatus(client_statuses_[i]));
  }
  UnlockClientsStatus();
  
  LockMonitorEvent();
  for (int i = 0;i<monitor_events_.size();i++)
  {
    net_info->AddMonitorEvent(StudioUtility::CopyMonitorEvent(monitor_events_[i]));
  }
  UnlockMonitorEvent();

  return net_info;
}


ClientStatus* wmdf::ServerNetInfo::FindClientStatus( int32_t socket_id )
{
  for (int i = 0 ;i<client_statuses_.size();i++)
  {
    if(client_status(i)->socket_id==socket_id)
    {
      return client_statuses_[i];
    }
  }
  return NULL;
}

TransferDataStat* wmdf::ServerNetInfo::FindTransferStatus( int16_t product_id )
{
  for (int i = 0 ;i<transfer_statuses_.size();i++)
  {
    if(transfer_status(i)->product_id==product_id)
    {
      return transfer_statuses_[i];
    }
  }
  return NULL;
}

void wmdf::ServerNetInfo::LockMonitorEvent()
{
  event_mutex_.lock();
}

void wmdf::ServerNetInfo::UnlockMonitorEvent()
{
  event_mutex_.unlock();
}

SymbolModel* wmdf::ServerNetInfo::GetSymbolModel( std::string key,StudioSymbolType type )
{
  map<std::string,SymbolModel* >::iterator iter = symbol_models_.find(key);
  if(iter != symbol_models_.end())
  {
    return iter->second;
  }
  else
  {
    SymbolModel* symbol_model = new SymbolModel();
    memset(symbol_model,0,sizeof(SymbolModel));
    symbol_model->symbol_name = key;
    symbol_model->symbol_type = type;
    symbol_models_.insert(make_pair(key,symbol_model));
    return symbol_model;
  }
}

void wmdf::ServerNetInfo::RestAllModels()
{
  map<std::string,SymbolModel* >::iterator iter = symbol_models_.begin();
  while(iter!=symbol_models_.end())
  {
    if(NULL !=iter->second)
    {
      iter->second->symbol_status = SMP_OK;
    }
    iter++;
  }
}

void wmdf::ServerNetInfo::ClearWarningStatus()
{
  map<std::string,SymbolModel* >::iterator iter = symbol_models_.begin();
  while(iter!=symbol_models_.end())
  {
    if(NULL !=iter->second)
    {
      StudioUtility::UnsetStatus(iter->second,SMP_Disconnect);
    }
    iter++;
  }
}

void wmdf::ServerNetInfo::ClearModels()
{
  map<std::string,SymbolModel* >::iterator iter = symbol_models_.begin();
  while(iter!=symbol_models_.end())
  {
    if(NULL !=iter->second)
    {
      delete iter->second;
      iter->second = NULL;
    }
    iter++;
  }
}

bool wmdf::ServerNetInfo::NeedAlarm()
{
  map<std::string,SymbolModel* >::iterator iter = symbol_models_.begin();
  while(iter!=symbol_models_.end())
  {
    if(NULL !=iter->second)
    {
      if(iter->second->symbol_status&SMP_Disconnect)
      {
        if(MonitorEnvironment::Instance()->trading_time_manager()->IsTrading(iter->second->product_id))
          return true;
      }
    }
    iter++;
  }
  return false;
}


