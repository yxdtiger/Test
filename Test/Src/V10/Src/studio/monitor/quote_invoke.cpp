#include "quote_invoke.h"
#include <string>
#include <cstring>
#include <cstdio>

#include "../../include/wmdf_api.h"
#include "../../include/date_time.h"
#include "../common/studio_utility.h"
#include "server_manager.h"
using namespace wmdf;

void QuoteCallbackFunc(RecordArgs* args)
{
  if(args!=NULL)
  {
    if( args->state != NULL)
    {          
      if(args->record->product_id!=PRODUCT_INVALID_ID)//行情
      {
        QuoteInvoke* invoke = (QuoteInvoke*)args->state;
        invoke->OnRecordCallback(args);
      }
    }
    wmdf_release_record(args);
  }  
}

QuoteInvoke::QuoteInvoke(int32_t conn_id,HWND handle):BaseInvoke(),conn_id_(conn_id),handle_(handle)
{
}

QuoteInvoke::~QuoteInvoke(void)
{
}



void QuoteInvoke::Connect()
{
  CallbackArgs callback_args={0};
  callback_args.record_callback =QuoteCallbackFunc;
  callback_args.state=this;  

  OpenArgs open_args={0};               //创建连接参数
  open_args.codec_type = CT_FAST;		//连接参数赋值    
  open_args.marktet_open_time = 00000;
  open_args.markt_close_time = 235959;

  ServerNetInfo* server_info =ServerManager::Instance()->net_info_collection()->find_server_by_cid(conn_id_);
  callback_args.connection_id = conn_id_+1;//给监控端口分配偶数，行情端口基数
  wmdf_set_callback(&callback_args);//设置回调函数


  //FilterArgs* args;
  //args->connection_id = conn_id_;
  //args->filter_masks = (uint8_t*)wind_code_.c_str();
  //wmdf_set_filter_mask(args);

  open_args.connection_id = callback_args.connection_id;
  memset(open_args.server_addr,0,15);
  memcpy(open_args.server_addr, server_info->ip().c_str(),server_info->ip().size());    
  open_args.server_port=server_info->quote_port();
  memcpy(open_args.user_name,server_info->quote_user_name().c_str(),server_info->quote_user_name().size());
  memcpy(open_args.password,server_info->quote_keyword().c_str(),server_info->quote_keyword().size());
  wmdf_open_connection(&open_args);//打开连接
}

void QuoteInvoke::Disconnect()
{
  CloseArgs args;
  args.connection_id = conn_id_+1;
  wmdf_close_connection(&args);
}


void QuoteInvoke::OnRecordCallback(RecordArgs* args)
{
  if(args->record->product_id==PRODUCT_SSE_LEVEL1_ID)
  {
    if(args->record->message_id==SSEL1_MARKETDATA_ID)
    {
      ProcessL1MarketData((L1MarketSnapshot*)args->record->body,args->record->record_count);
    }    
  }
  else if(args->record->product_id==PRODUCT_SZSE_LEVEL1_ID)
  {
    if(args->record->message_id==SZSEL1_MARKETDATA_ID)
    {
      ProcessL1MarketData((L1MarketSnapshot*)args->record->body,args->record->record_count);
    }   
  }
  else if(args->record->product_id==PRODUCT_SSE_LEVEL2_ID)
  {
    if(args->record->message_id==SSEL2_MARKETDATA_ID)
    {
      ProcessL2MarketData((L2MarketSnapshot*)args->record->body,args->record->record_count);
    }
    else if(args->record->message_id==SSEL2_TRANSACTION_ID)
    {
      ProcessL2Transaction((L2Transaction*)args->record->body,args->record->record_count);
    }
  }
  else if(args->record->product_id==PRODUCT_SZSE_LEVEL2_ID)
  {
    if(args->record->message_id==SZSEL2_MARKETDATA_ID)
    {
      ProcessL2MarketData((L2MarketSnapshot*)args->record->body,args->record->record_count);
    }
    else if(args->record->message_id==SZSEL2_TRANSACTION_ID)
    {
      ProcessL2Transaction((L2Transaction*)args->record->body,args->record->record_count);
    }
  }
  else if(args->record->product_id==PRODUCT_CFFE_LEVEL1_ID)
  {
    if(args->record->message_id==CFFEL1_MARKETDATA_ID)
    {
      ProcessFuturesMarketSnapshot((FuturesMarketSnapshot*)args->record->body,args->record->record_count);
    }
  }
  else if(args->record->product_id==PRODUCT_CFFE_LEVEL2_ID)
  {
    if(args->record->message_id==CFFEL2_MARKETDATA_ID)
    {
      ProcessCFFEL2MarketSnapshot((CFEL2MarketSnapshot*)args->record->body,args->record->record_count);
    }
  }
  else if(args->record->product_id==PRODUCT_SHFE_LEVEL1_ID)
  {
    if(args->record->message_id==SHFEL1_MARKETDATA_ID)
    {
      ProcessFuturesMarketSnapshot((FuturesMarketSnapshot*)args->record->body,args->record->record_count);
    }
  }
  else if(args->record->product_id==PRODUCT_DCE_LEVEL1_ID)
  {
    if(args->record->message_id==DCEL1_MARKETDATA_ID)
    {
      ProcessFuturesMarketSnapshot((FuturesMarketSnapshot*)args->record->body,args->record->record_count);
    }
  }
  else if(args->record->product_id==PRODUCT_CZCE_LEVEL1_ID)
  {
    if(args->record->message_id==CZCEL1_MARKETDATA_ID)
    {
      ProcessFuturesMarketSnapshot((FuturesMarketSnapshot*)args->record->body,args->record->record_count);
    }
  }
}


void QuoteInvoke::ProcessL1MarketData( L1MarketSnapshot* market_datas,int count )
{
  for(int i =0;i<count;i++)
  {
    if(strcmp(market_datas[i].wind_code,wind_code_.c_str())==0)
    {
      L1MarketSnapshot* item = StudioUtility::CopyL1MarketSnapshot(&market_datas[i]);
      SendMessage(handle_,WM_L1_MKT_RECORD,(WPARAM)item,NULL);
    }
  }
}

void QuoteInvoke::ProcessL2MarketData(L2MarketSnapshot* market_datas,int count)
{
  for(int i =0;i<count;i++)
  {
    if(strcmp(market_datas[i].wind_code,wind_code_.c_str())==0)
    {
      L2MarketSnapshot* item = StudioUtility::CopyL2MarketSnapshot(&market_datas[i]);
      SendMessage(handle_,WM_L2_MKT_RECORD,(WPARAM)item,NULL);
    }
  }
}


void QuoteInvoke::ProcessL2Transaction(L2Transaction* trans,int count)
{
  for(int i =0;i<count;i++)
  {
    if(strcmp(trans[i].wind_code,wind_code_.c_str())==0)
    {
      L2Transaction* item = StudioUtility::CopyL2Transaction(&trans[i]);
      SendMessage(handle_,WM_L2_TRANS_RECORD,(WPARAM)item,NULL);
    }
  }
}

void QuoteInvoke::ProcessFuturesMarketSnapshot( FuturesMarketSnapshot* trans,int count )
{
  for(int i =0;i<count;i++)
  {
    if(strcmp(trans[i].wind_code,wind_code_.c_str())==0)
    {
      FuturesMarketSnapshot* item = StudioUtility::CopyFuturesMarketData(&trans[i]);
      SendMessage(handle_,WM_L1_Future_MKT_RECORD,(WPARAM)item,NULL);
    }
  }
}

void QuoteInvoke::ProcessCFFEL2MarketSnapshot( CFEL2MarketSnapshot* trans,int count )
{
  for(int i =0;i<count;i++)
  {
    if(strcmp(trans[i].wind_code,wind_code_.c_str())==0)
    {
      CFEL2MarketSnapshot* item = StudioUtility::CopyCFFEL2MarketData(&trans[i]);
      SendMessage(handle_,WM_L2_CFFEL2_MKT_RECORD,(WPARAM)item,NULL);
    }
  }
}




