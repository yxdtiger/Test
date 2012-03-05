#include "monitor_invoke.h"
#include <string>
#include <cstring>
#include <cstdio>
#include "../../include/wmdf_api.h"
#include "../../include/date_time.h"
#include "../../environment/splitter.h"
#include "../common/studio_utility.h"
#include "server_manager.h"
#include "model_id_factory.h"
#include "../../monitor/logger.h"

using namespace wmdf;
using namespace std;
void RecordCallbackFunc(RecordArgs* args)
{
  if(args!=NULL)
  {
    if( args->state != NULL)
    {      
      if(args->record->product_id==PRODEUCT_MONITOR_ID)
      {
        MonitorInvoke* invoke = (MonitorInvoke*)args->state;
        invoke->OnRecordCallback(args);
      }
    }
    wmdf_release_record(args);
  }  
}

void EventCallbackFunc(EventArgs* args)
{
  if(args!=NULL)
  {
    if(args->event_id==MEI_SOURCE_DISCONNECTED)
    {
      ServerManager::Instance()->net_info_collection()->find_server_by_cid(args->connection_id)->set_status(-1);
      int socket_id = -1;
      std::string key = ModelIDFactory::GetServer2ClientSocket(args->connection_id,socket_id);
      ServerNetInfo* server_info = ServerManager::Instance()->net_info_collection()->find_server_by_cid(args->connection_id);
      StudioUtility::SetStatus(server_info->GetSymbolModel(key,SST_Client),SMP_Disconnect);
    }
    else if(args->event_id==MEI_SOURCE_CONNECTED)
    {
      ServerManager::Instance()->net_info_collection()->find_server_by_cid(args->connection_id)->set_status(1);
      int socket_id = -1;
      std::string key = ModelIDFactory::GetServer2ClientSocket(args->connection_id,socket_id);
      ServerNetInfo* server_info = ServerManager::Instance()->net_info_collection()->find_server_by_cid(args->connection_id);
      StudioUtility::UnsetStatus(server_info->GetSymbolModel(key,SST_Client) , SMP_Disconnect);
    }
  }
}


MonitorInvoke::MonitorInvoke():BaseInvoke()
{
  
}

MonitorInvoke::~MonitorInvoke(void)
{
  
}



void MonitorInvoke::Connect()
{
  CallbackArgs callback_args={0};
  callback_args.record_callback = RecordCallbackFunc;
  callback_args.event_callback = EventCallbackFunc;
  callback_args.state=this;  
  
  OpenArgs open_args={0};               //创建连接参数
  open_args.codec_type = CT_BINARY;		//连接参数赋值    
  open_args.marktet_open_time = 00000;
  open_args.markt_close_time = 235959;
 
  ServerNetInfo* server_info;
  for(int i = 0;i<ServerManager::Instance()->net_info_collection()->server_info_count();i++)
  {
    if(ServerManager::Instance()->net_info_collection()->server_infos(i)->status()!=1)
    {
      server_info = ServerManager::Instance()->net_info_collection()->server_infos(i);
      
      callback_args.connection_id = server_info->server_id();
      wmdf_set_callback(&callback_args);//设置回调函数
      
      open_args.connection_id = callback_args.connection_id;
      memset(open_args.server_addr,0,15);
      memcpy(open_args.server_addr, server_info->ip().c_str(),server_info->ip().size());    
      open_args.server_port=server_info->monitor_port();
      memcpy(open_args.user_name,server_info->monitor_user_name().c_str(),server_info->monitor_user_name().size());
      memcpy(open_args.password,server_info->monitor_keyword().c_str(),server_info->monitor_keyword().size());
      
      if (wmdf_open_connection(&open_args)==SUCCESS)//打开连接
      {
        server_info->set_status(1);
      }
      else
      {
        server_info->set_status(-1);
      }
    }
  }
}


void MonitorInvoke::ConnectOne( int32_t conn_id )
{
  CallbackArgs callback_args={0};
  callback_args.record_callback = RecordCallbackFunc;
  callback_args.event_callback = EventCallbackFunc;
  callback_args.state=this;  

  OpenArgs open_args={0};               //创建连接参数
  open_args.codec_type = CT_BINARY;		//连接参数赋值    
  open_args.marktet_open_time = 80000;
  open_args.markt_close_time = 80000;

  ServerNetInfo* server_info = ServerManager::Instance()->net_info_collection()->find_server_by_cid(conn_id);
  if(server_info->status()==1)
    return;
  callback_args.connection_id = server_info->server_id();
  wmdf_set_callback(&callback_args);//设置回调函数

  open_args.connection_id = callback_args.connection_id;
  memset(open_args.server_addr,0,15);
  memcpy(open_args.server_addr, server_info->ip().c_str(),server_info->ip().size());    
  open_args.server_port=server_info->monitor_port();
  memcpy(open_args.user_name,server_info->monitor_user_name().c_str(),server_info->monitor_user_name().size());
  memcpy(open_args.password,server_info->monitor_keyword().c_str(),server_info->monitor_keyword().size());

  if (wmdf_open_connection(&open_args)==SUCCESS)//打开连接
  {
    server_info->set_status(1);
  }
  else
  {
    server_info->set_status(-1);
  }
}


void MonitorInvoke::Disconnect()
{
  CloseArgs args={0};
  for(int i = 0;i<ServerManager::Instance()->net_info_collection()->server_info_count();i++)
  {
    args.connection_id = ServerManager::Instance()->net_info_collection()->server_infos(i)->server_id();
    wmdf_close_connection(&args);
    ServerManager::Instance()->net_info_collection()->server_infos(i)->set_status(-1);
  }
}


void MonitorInvoke::DisconnectOne( int32_t conn_id )
{
  CloseArgs args={0};
  args.connection_id = conn_id;
  wmdf_close_connection(&args);
  ServerManager::Instance()->net_info_collection()->find_server_by_cid(conn_id)->set_status(-1);
}


void MonitorInvoke::OnRecordCallback(RecordArgs* args)
{
  if(args->record->product_id==PRODEUCT_MONITOR_ID)
  {    
    if(args->record->message_id==MONITOR_TRANSFER_STATUS_ID)
    {
      ProcessTransferDataStats(args->connection_id,(TransferDataStat*)args->record->body,args->record->record_count);
    }
    else if(args->record->message_id==MONITOR_CLIENT_STATUS_ID)
    {
      ProcessClientStats(args->connection_id,(ClientStatus*)args->record->body,args->record->record_count);
    }
    else if(args->record->message_id==MONITOR_EVENT_ID)
    {
      ProcessEventLog(args->connection_id,(MonitorEvent*)args->record->body,args->record->record_count);
    }
  }
}

void MonitorInvoke::ProcessTransferDataStats(int32_t conn_id,TransferDataStat* status,int count)
{
  ServerNetInfo* server_net_info = ServerManager::Instance()->net_info_collection()->find_server_by_cid(conn_id);
  CheckTransferInfo(count, status, server_net_info, conn_id);
  server_net_info->ClearTransfersStatus();
  for(int i =0;i<count;i++)
  {
    server_net_info->AddTransferStatus(StudioUtility::CopyTransferStat(&status[i]));
  }
}

void MonitorInvoke::ProcessClientStats(int32_t conn_id,ClientStatus* status,int count)
{
  ServerNetInfo* server_net_info = ServerManager::Instance()->net_info_collection()->find_server_by_cid(conn_id);
  CheckClientInfo(count, status, server_net_info, conn_id);
  server_net_info->ClearClientsStatus();
  for(int i =0;i<count;i++)
  {
    server_net_info->AddClientStatus(StudioUtility::CopyClientStatus(&status[i]));
  }
}

void MonitorInvoke::ProcessEventLog( int32_t conn_id,MonitorEvent* events,int count )
{
  ServerNetInfo* server_net_info = ServerManager::Instance()->net_info_collection()->find_server_by_cid(conn_id);
  MonitorEvent* monitor_event =NULL;
  string key;
  if(server_net_info!=NULL&&server_net_info->monitor_events_count()>500)
  {
    server_net_info->ClearMonitorEvent();
  }
  for(int i =0;i<count;i++)
  {
    monitor_event = &events[i];
    Logger::instance()->WriteFormatEventLog((LogLevel)monitor_event->level,monitor_event->event_id,(char*)monitor_event->at_where,(char*)monitor_event->what_happened);
    server_net_info->AddMonitorEvent(StudioUtility::CopyMonitorEvent(monitor_event));
    if(monitor_event->event_id==MEI_SOURCE_CONNECTED)
    {
      Splitter tokens((char*)monitor_event->what_happened,",");
      if (tokens.size() >= 3)
      {
        Splitter ip_info(tokens[1],":");
        string ip = ip_info[1];
        Splitter port_info(tokens[2],":");
        string port =port_info[1];

        key = ModelIDFactory::GetServer2SourcePort(conn_id,ip.c_str(),atoi(port.c_str()));
        StudioUtility::UnsetStatus(server_net_info->GetSymbolModel(key,SST_Source), SMP_Disconnect);
      }
    }
    else if(monitor_event->event_id==MEI_SOURCE_DISCONNECTED)
    {
      Splitter tokens((char*)monitor_event->what_happened,",");
      if (tokens.size() >= 3)
      {
        Splitter ip_info(tokens[1],":");
        string ip = ip_info[1];
        Splitter port_info(tokens[2],":");
        string port =port_info[1];
        key = ModelIDFactory::GetServer2SourcePort(conn_id,ip.c_str(),atoi(port.c_str()));
        StudioUtility::SetStatus(server_net_info->GetSymbolModel(key,SST_Source), SMP_Disconnect);
        if(strcmp((char*)monitor_event->at_where,"product adapter")==0&&tokens.size() >= 4)
        {
          Splitter product_id_info(tokens[3],":");
          string product_id =product_id_info[1];
          StudioUtility::SetProductId(server_net_info->GetSymbolModel(key,SST_Source),atoi(product_id.c_str()));
        }
      }
    }
    else if(monitor_event->event_id==MEI_CLIENT_CONNECTED)
    {
      Splitter tokens((char*)monitor_event->what_happened,",");
      if (tokens.size() >= 2)
      {
        Splitter ip_info(tokens[0],":");
        string ip = ip_info[0];
        Splitter socket_info(tokens[1],":");
        string socket_id = socket_info[1];
        key = ModelIDFactory::GetServer2ClientSocket(conn_id,atoi(socket_id.c_str()));
        StudioUtility::UnsetStatus(server_net_info->GetSymbolModel(key,SST_Client) , SMP_Disconnect);
      }
    }
    else if(monitor_event->event_id==MEI_CLIENT_DISCONNECTED)
    {
      Splitter tokens((char*)monitor_event->what_happened,",");
      if (tokens.size() >= 2)
      {
        Splitter ip_info(tokens[0],":");
        string ip = ip_info[0];
        Splitter socket_info(tokens[1],":");
        string socket_id = socket_info[1];
        key = ModelIDFactory::GetServer2ClientSocket(conn_id,atoi(socket_id.c_str()));
        StudioUtility::SetStatus(server_net_info->GetSymbolModel(key,SST_Client) , SMP_Disconnect);
      }
    }
  }
}

void MonitorInvoke::CheckClientInfo( int count, ClientStatus* status, ServerNetInfo* server_net_info, int32_t conn_id )
{
  ClientStatus* pre_client_stat = NULL;
  ClientStatus* cur_client_stat = NULL;  
  for(int j=0;j<count;j++)
  {
    cur_client_stat = &status[j];
    pre_client_stat = server_net_info->FindClientStatus(cur_client_stat->socket_id);
    if(cur_client_stat!=NULL && pre_client_stat!=NULL)
    {
      std::string key = ModelIDFactory::GetServer2ClientSocket(conn_id,cur_client_stat->socket_id);
      server_net_info->GetSymbolModel(key,SST_Client)->flow = cur_client_stat->send_data_flow;
      if(cur_client_stat->send_bytes==pre_client_stat->send_bytes)
      {
        StudioUtility::SetStatus(server_net_info->GetSymbolModel(key,SST_Client), SMP_NoSend);
      }
      else
      {
        StudioUtility::UnsetStatus(server_net_info->GetSymbolModel(key,SST_Client), SMP_NoSend);
      }
    }
  }
}

void MonitorInvoke::CheckTransferInfo( int count, TransferDataStat* status, ServerNetInfo* server_net_info, int32_t conn_id )
{
  TransferDataStat* pre_transfer_stat = NULL;
  TransferDataStat* cur_transfer_stat = NULL;

  for(int j=0;j<count;j++)
  {
    cur_transfer_stat = &status[j];
    pre_transfer_stat = server_net_info->FindTransferStatus(cur_transfer_stat->product_id);
    
    if(cur_transfer_stat!=NULL && pre_transfer_stat!=NULL)
    {
      std::string key = ModelIDFactory::GetServer2SourcePort(conn_id,(char*)cur_transfer_stat->source_ip,cur_transfer_stat->source_port);
      server_net_info->GetSymbolModel(key,SST_Server)->flow = cur_transfer_stat->receive_data_flow;

      if(cur_transfer_stat->encoded_size==pre_transfer_stat->encoded_size)
      {
        StudioUtility::SetStatus(server_net_info->GetSymbolModel(key,SST_Source),SMP_NoSend);
      }
      else
      {
        StudioUtility::UnsetStatus(server_net_info->GetSymbolModel(key,SST_Source), SMP_NoSend);
      }

      if(cur_transfer_stat->delayed_msg_delta > 0)
      {
        StudioUtility::SetStatus(server_net_info->GetSymbolModel(key,SST_Source), SMP_Delay);
      }
      else
      {
        StudioUtility::UnsetStatus(server_net_info->GetSymbolModel(key,SST_Source),SMP_Delay);
      }

      if(cur_transfer_stat->source_port==0)
        continue;
      if(cur_transfer_stat->receive_data_size==pre_transfer_stat->receive_data_size)
      {
        StudioUtility::SetStatus(server_net_info->GetSymbolModel(key,SST_Source), SMP_NoReceive);
      }
      else
      {
        StudioUtility::UnsetStatus(server_net_info->GetSymbolModel(key,SST_Source),SMP_NoReceive);
      }

      if(cur_transfer_stat->source_port!=0)
      {
        if(cur_transfer_stat->source_status==0)
        {
          StudioUtility::SetStatus(server_net_info->GetSymbolModel(key,SST_Source),SMP_Disconnect);
          StudioUtility::SetProductId(server_net_info->GetSymbolModel(key,SST_Source),cur_transfer_stat->product_id);
        }
        else
        {
          StudioUtility::UnsetStatus(server_net_info->GetSymbolModel(key,SST_Source),SMP_Disconnect);
        }
      }
    }
  }
}
