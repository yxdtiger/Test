#include "wmdf_invoke.h"
#include <string>
#include <cstring>
#include <cstdio>
#include <iostream>

void RecordCallbackFunc(RecordArgs* args)
{
  if(args->state != NULL)
  {
    WmdfInvoke* invoke = (WmdfInvoke*)args->state;
    invoke->OnRecordCallback(args->record);
    wmdf_release_record(args);
  }
}

void EventCallbackFunc(EventArgs* args)
{
  if(args->state != NULL)
  {
    WmdfInvoke* invoke = (WmdfInvoke*)args->state;
    invoke->OnEventCallback(args);
  }
}

WmdfInvoke::WmdfInvoke(void)
{
  wmdf_initialize();    
}

WmdfInvoke::~WmdfInvoke(void)
{
  wmdf_finalize();
}

void WmdfInvoke::Connect()
{
  CallbackArgs callback_args;
  callback_args.connection_id=0;
  callback_args.record_callback =RecordCallbackFunc;
  callback_args.event_callback = EventCallbackFunc;
  callback_args.state=this;
  wmdf_set_callback(&callback_args);//设置回调函数
  
  FilterArgs filter_args;
  filter_args.connection_id = 0;
  std::string filter_masks = "300078.SZ,300077.SZ";
  filter_args.filter_masks = (uint8_t*)filter_masks.c_str();
  wmdf_set_filter_mask(&filter_args);


  OpenArgs open_args;//创建连接参数
  open_args.codec_type = CT_FAST;		//连接参数赋值
  open_args.connection_id = 0;
  std::string username("admin"); 
  std::string pass_word("admin"); 
  std::string ip_address("10.100.2.228");
  memcpy(open_args.user_name,username.c_str(),username.size());
  memcpy(open_args.password,pass_word.c_str(),pass_word.size());
  memcpy(open_args.server_addr, ip_address.c_str(),ip_address.size());    
  open_args.server_port=6859;
  open_args.marktet_open_time = 80000;
  open_args.markt_close_time = 160000;
  if (wmdf_open_connection(&open_args)==SUCCESS)//打开连接
  {
    printf("success.\n");
  }
  else
  {
    printf("failed.\n");
  }
}

void WmdfInvoke::Disconnect()
{
  CloseArgs args;
  args.connection_id = 0;
  wmdf_close_connection(&args);
}

void WmdfInvoke::OnEventCallback( EventArgs* args )
{
  printf("event id: %d, context: %s ",args->Context,args->Context);
}

void WmdfInvoke::OnRecordCallback( MdfRecord* record )
{
  if(record->product_id==PRODUCT_SZSE_LEVEL2_ID)
  {
    if(record->message_id==SZSEL2_MARKETDATA_ID)
    {
      ProcessL1MarketData(record);
    }
  }
}

void WmdfInvoke::ProcessL1MarketData( MdfRecord* record )
{
  L2MarketSnapshot* marketDatas = (L2MarketSnapshot*) record->body;
  for(int index=0;index<record->record_count;index++)
  {					      	
    std::cout<<marketDatas[index].wind_code<<std::endl;
    //add your code here
  }
}




