#include "wmdf_invoke.h"
#include <string>
#include <cstring>
#include <cstdio>
#include <iostream>
#include "../../include/wmdf_api.h"
#include "../../include/date_time.h"
#include "monitor_environment.h"
using namespace wmdf;
void RecordCallbackFunc(RecordArgs* args)
{
  if(args!=NULL && args->state != NULL)
  {
    WmdfInvoke* invoke = (WmdfInvoke*)args->state;
    invoke->OnRecordCallback(args);
  }
}

WmdfInvoke::WmdfInvoke()
{
  wmdf_initialize();   
}

WmdfInvoke::~WmdfInvoke(void)
{
  wmdf_finalize();
}

void WmdfInvoke::Connect()
{
  CallbackArgs callback_args={0};
  callback_args.record_callback =RecordCallbackFunc;
  callback_args.state=this;  
  
  OpenArgs open_args={0};               //创建连接参数
  open_args.codec_type = CT_BINARY;		//连接参数赋值  
  std::string username("admin"); 
  std::string pass_word("admin"); 
  memcpy(open_args.user_name,username.c_str(),username.size());
  memcpy(open_args.password,pass_word.c_str(),pass_word.size());
  
  open_args.marktet_open_time = 80000;
  open_args.markt_close_time = 160000;
 
  for(int i = 0;i<MonitorEnvironment::Instance()->server_manager()->server_info_count();i++)
  {
    if(MonitorEnvironment::Instance()->server_manager()->server_infos(i)->status()!=1)
    {
      callback_args.connection_id=i;
      wmdf_set_callback(&callback_args);//设置回调函数

      open_args.connection_id = i;
      memset(open_args.server_addr,0,15);
      memcpy(open_args.server_addr, MonitorEnvironment::Instance()->server_manager()->server_infos(i)->ip().c_str(),MonitorEnvironment::Instance()->server_manager()->server_infos(i)->ip().size());    
      open_args.server_port=MonitorEnvironment::Instance()->server_manager()->server_infos(i)->port();
      if (wmdf_open_connection(&open_args)==SUCCESS)//打开连接
      {
        MonitorEnvironment::Instance()->server_manager()->server_infos(i)->set_status(1);
      }
      else
      {
        MonitorEnvironment::Instance()->server_manager()->server_infos(i)->set_status(-1);
      }
    }    
  }  
}

void WmdfInvoke::Disconnect()
{
  CloseArgs args;
  for(int i = 0;i<MonitorEnvironment::Instance()->server_manager()->server_info_count();i++)
  {
    args.connection_id = i;
    wmdf_close_connection(&args);
  }
}

void WmdfInvoke::OnRecordCallback(RecordArgs* args)
{
  if(args->record->product_id==PRODEUCT_MONITOR_ID)
  {    
    if(args->record->message_id==MONITOR_TRANSFER_STATUS_ID)
    {
      MdfRecord* a_copy_record = MonitorEnvironment::Instance()->CopyMonitorRecord(args->record);
      MonitorEnvironment::Instance()->ProcessTransferDataStats(args->connection_id,a_copy_record);
    }
    else if(args->record->message_id==MONITOR_CLIENT_STATUS_ID)
    {
      MdfRecord* a_copy_record = MonitorEnvironment::Instance()->CopyMonitorRecord(args->record);
      MonitorEnvironment::Instance()->ProcessClientStats(args->connection_id,a_copy_record);
    }
  }
  wmdf_release_record(args);
}


