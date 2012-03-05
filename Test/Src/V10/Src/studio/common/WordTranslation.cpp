#include "../main_frame/StdAfx.h"
#include "WordTranslation.h"
#include "../../include/wmdf_api.h"

using namespace std;

CWordTranslation::CWordTranslation(void)
{
}

CWordTranslation::~CWordTranslation(void)
{
}

string CWordTranslation::TranslationProductID(const int16_t product_id)
{
  string translation_string;
  switch(product_id)
  {
  case PRODUCT_INVALID_ID:
    translation_string = "无效产品ID";
    break;
  case PRODEUCT_SYSTEM_ID:
    translation_string = "系统消息";
    break;
  case PRODEUCT_MONITOR_ID:
    translation_string = "监控产品";
    break;
  case PRODUCT_SSE_LEVEL1_ID:
    translation_string = "上交所Level1";
    break;
  case PRODUCT_SZSE_LEVEL1_ID:
    translation_string = "深交所Level1";
    break;
  case PRODUCT_SSE_LEVEL2_ID:
    translation_string = "上交所Level2";
    break;
  case PRODUCT_SZSE_LEVEL2_ID:
    translation_string = "深交所Level2";
    break;
  case PRODUCT_CFFE_LEVEL1_ID:
    translation_string = "中金所Level1";
    break;
  case PRODUCT_CFFE_LEVEL2_ID:
    translation_string = "中金所Level2";
    break;
  case PRODUCT_SHFE_LEVEL1_ID:
    translation_string = "上期所Level1";
    break;
  case PRODUCT_DCE_LEVEL1_ID:
    translation_string = "大商所Level1";
    break;
  case PRODUCT_CZCE_LEVEL1_ID:
    translation_string = "郑商所Level1";
    break;
  case PRODUCT_FX_ID:
    translation_string = "外汇";
    break;
  case PRODUCT_SIMULATOR_ID:
    translation_string = "产品模拟器";
    break;
  case PRODUCT_FTSE_LEVEL1_ID:
    translation_string = "新华富时指数";
    break;
  default:
    char str[20];
    sprintf(str,"FeedServerRelay%d",product_id);
    translation_string =str;
    break;
  }

  return translation_string;
}

// 将message id 转译成message name
string CWordTranslation::TranslationMessageID(const int16_t product_id, const int16_t message_id)
{
  string translation_string;
  switch(product_id)
  {
  case PRODUCT_INVALID_ID:
    switch(message_id)
    {
    case MESSAGE_INVALID_ID:
      translation_string = "无效消息ID";
    default:
      break;
    }
    break;
  case PRODEUCT_SYSTEM_ID:
    switch(message_id)
    {
    case SYSTEM_HEARTBEAT_ID:
      translation_string = "心跳消息";
      break;
    case SYSTEM_LOGIN_REQUEST_ID:
      translation_string = "登录请求消息";
      break;
    case SYSTEM_LOGIN_RESPONSE_ID:
      translation_string = "登录响应信息";
      break;
    case SYSTEM_LOGOUT_ID:
      translation_string = "登出通知信息";
      break;
//     case SYSTEM_REBUILD_REQUEST_ID:
//       translation_string = "数据重建请求消息";
//       break;
//     case SYSTEM_REBUILD_RESPONSE_ID:
//       translation_string = "数据重建响应消息";
//       break;
    default:
      break;
    }
    break;
  case PRODEUCT_MONITOR_ID:
    switch(message_id)
    {
    case MONITOR_TRANSFER_STATUS_ID:
      translation_string = "监控运行状态消息";
      break;
    case MONITOR_CLIENT_STATUS_ID:
      translation_string = "监控客户端状态消息";
      break;
    default:
      break;
    }
    break;
  case PRODUCT_SSE_LEVEL1_ID:
    switch(message_id)
    {
    case SSEL1_MARKETDATA_ID:
      translation_string = "上交所Level1快照";
      break;
    default:
      break;
    }
    break;
  case PRODUCT_SZSE_LEVEL1_ID:
    switch(message_id)
    {
    case SSEL1_MARKETDATA_ID:
      translation_string = "深交所Level1快照";
      break;
    default:
      break;
    }
    break;
  case PRODUCT_SSE_LEVEL2_ID:
    switch(message_id)
    {
    case SSEL2_MARKETDATA_ID:
      translation_string = "上交所Level2快照";
      break;
    case SSEL2_TRANSACTION_ID:
      translation_string = "上交所Level2逐笔成交";
      break;
    case SSEL2_ORDERQUEUE_ID:
      translation_string = "上交所Level2委托队列";
      break;
    default:
      break;
    }
    break;
  case PRODUCT_SZSE_LEVEL2_ID:
    switch(message_id)
    {
    case SSEL2_MARKETDATA_ID:
      translation_string = "深交所Level2快照";
      break;
    case SSEL2_TRANSACTION_ID:
      translation_string = "深交所Level2逐笔成交";
      break;
    case SSEL2_ORDERQUEUE_ID:
      translation_string = "深交所Level2委托队列";
      break;
    case SZSEL2_SINGLEORDER_ID:
      translation_string = "深交所Level2逐笔委托";
      break;
    default:
      break;
    }
    break;
  case PRODUCT_CFFE_LEVEL1_ID:
    switch(message_id)
    {
    case CFFEL1_MARKETDATA_ID:
      translation_string = "中金所Level1快照";
      break;
    default:
      break;
    }
    break;
  case PRODUCT_CFFE_LEVEL2_ID:
    switch(message_id)
    {
    case CFFEL2_MARKETDATA_ID:
      translation_string = "中金所Level2快照";
      break;
    default:
      break;
    }
    break;
  case PRODUCT_SHFE_LEVEL1_ID:
    switch(message_id)
    {
    case SHFEL1_MARKETDATA_ID:
      translation_string = "上期所Level1快照";
      break;
    default:
      break;
    }
    break;
  case PRODUCT_DCE_LEVEL1_ID:
    switch(message_id)
    {
    case DCEL1_MARKETDATA_ID:
      translation_string = "大商所Level1快照";
      break;
    default:
      break;
    }
    break;
  case PRODUCT_CZCE_LEVEL1_ID:
    switch(message_id)
    {
    case CZCEL1_MARKETDATA_ID:
      translation_string = "郑商所Level1快照";
      break;
    default:
      break;
    }
    break;
  case PRODUCT_FX_ID:
    switch(message_id)
    {
    case FX_MARKETDATA_ID:
      translation_string = "外汇快照";
      break;
    default:
      break;
    }
    break;
  case PRODUCT_SIMULATOR_ID:
    break;
  default:
    break;
  }

  return translation_string;
}

// 将委托队列的side转译成name
string CWordTranslation::TranslationOrderQueueL2Side(const uint8_t side)
{
  string translation_string;
  switch(side)
  {
  case 1:
    translation_string = "Bid";
    break;
  case 2:
    translation_string = "Ask";
    break;
  default:
    translation_string = "Invalid";
    break;
  }
  return translation_string;
}

// 将委托队列的image_status转译成name
string CWordTranslation::TranslationOrderQueueL2ImageStatus(const int32_t image_status)
{
  string translation_string;
  switch(image_status)
  {
  case 1:
    translation_string = "full";
    break;
  case 2:
    translation_string = "update";
    break;
  default:
    translation_string = "invalid";
    break;
  }
  return translation_string;
}

// 将委托队列的price_level_operator转译成name
string CWordTranslation::TranslationOrderQueueL2PriceLevelOperator(const int32_t price_level_operator)
{
  string translation_string;
  switch(price_level_operator)
  {
  case 0:
    translation_string = "absent";
    break;
  case 1:
    translation_string = "add";
    break;
  case 2:
    translation_string = "update";
    break;
  case 3:
    translation_string = "delete";
    break;
  default:
    break;
  }
  return translation_string;
}

// 将委托队列的order_queue_operator转译成name
string CWordTranslation::TranslationOrderQueueL2OrderQueueOperator(const int32_t order_queue_operator)
{
  string translation_string;
  switch(order_queue_operator)
  {
  case 0:
    translation_string = "absent";
    break;
  case 1:
    translation_string = "add";
    break;
  case 2:
    translation_string = "update";
    break;
  case 3:
    translation_string = "delete";
    break;
  default:
    break;
  }
  return translation_string;
}

// 将逐笔委托的order_kind转译成name
string CWordTranslation::TranslationOrdersL2order_kind(const char order_kind)
{
  string translation_string;
  switch(order_kind)
  {
  case 1:
    translation_string = "bid";
    break;
  case 2:
    translation_string = "offer";
    break;
  case 3:
    translation_string = "cancel";
    break;
  default:
    break;
  }
  return translation_string;
}

std::string CWordTranslation::TranslateServerStatus( const int32_t server_stat )
{
  if(server_stat>0)
  {
    return "连接成功";
  }
  else if(server_stat ==0)
  {
    return "未连接";
  }
  else
    return "连接断开";
}
