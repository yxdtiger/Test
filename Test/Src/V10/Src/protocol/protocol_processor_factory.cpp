#include "protocol_processor_factory.h"
#include "../include/wmdf_structs.h"
#include "binary_codec_heart_beat.h"
#include "binary_codec_login_request.h"
#include "binary_codec_login_response.h"
#include "binary_codec_logout.h"
#include "binary_codec_date_signal.h"
#include "binary_codec_config_info_request.h"
#include "binary_codec_config_info.h"
#include "binary_codec_config_response.h"

#include "binary_codec_transfer_stat.h"
#include "binary_codec_client_status.h"
#include "binary_codec_monitor_event.h"

#include "fast_codec_shszl1_market_data.h"
#include "fast_codec_shl2_trans.h"
#include "fast_codec_shl2_market_data.h"
#include "fast_codec_1_1_shl2_market_data.h"
#include "fast_codec_shl2_order_queue.h"
#include "fast_codec_index_market_data.h"

#include "fast_codec_szl2_market_data.h"
#include "fast_codec_szl2_trans.h"
#include "fast_codec_1_1_szl2_market_data.h"
#include "fast_codec_szl2_orders.h"

#include "fast_codec_futurel1_market_data.h"
#include "binary_codec_futurel1_market_data.h"
#include "fast_codec_cffel2_market_data.h"
#include "binary_codec_cffel2_market_data.h"
#include "fast_codec_cffel2_1_1_market_data.h"
#include "binary_codec_cffel2_1_1_market_data.h"


#include "binary_codec_shszl1_market_data.h"
#include "binary_codec_shl2_trans.h"
#include "binary_codec_shl2_market_data.h"
#include "binary_codec_1_1_shl2_market_data.h"
#include "binary_codec_shl2_order_queue.h"
#include "binary_codec_szl2_orders.h"
#include "binary_codec_index_market_data.h"

#include "header_codec.h"
#include "message_codec.h"
#include "system_header_codec.h"
#include "market_header_codec.h"
#include "monitor_header_codec.h"

using namespace wmdf;
wmdf::ProtocolProcessorFactory::ProtocolProcessorFactory(void)
{
}

wmdf::ProtocolProcessorFactory::~ProtocolProcessorFactory(void)
{
}

MessageCodec* wmdf::ProtocolProcessorFactory::CreateMessageCodec( int64_t protocol_unique_id, int16_t message_id )
{
  switch(protocol_unique_id)
  {
  case SYSTEM_1_0_BINARY:
    return Create_System_1_0_Binary_Codec(message_id);
  case MONITOR_1_0_BINARY:
    return Create_Monitor_1_0_Binary_Codec(message_id);
    /*上交所*/
  case SSEL1_1_0_FAST:
    return Create_SSEL1_1_0_Fast_Codec(message_id); 
  case SSEL1_1_0_BINARY:
    return Create_SSEL1_1_0_Binary_Codec(message_id);
  case SSEL2_1_0_FAST:
    return Create_SSEL2_1_0_Fast_Codec(message_id);
  case SSEL2_1_0_BINARY:
    return Create_SSEL2_1_0_Binary_Codec(message_id);
  case SSEL2_1_1_FAST:
    return Create_SSEL2_1_1_Fast_Codec(message_id);
  case SSEL2_1_1_BINARY:
    return Create_SSEL2_1_1_Binary_Codec(message_id);
    /*深交所*/
  case SZSEL1_1_0_FAST:
    return Create_SZSEL1_1_0_Fast_Codec(message_id);  
  case SZSEL1_1_0_BINARY:
    return Create_SZSEL1_1_0_Binary_Codec(message_id);
  case SZSEL2_1_0_FAST:
    return Create_SZSEL2_1_0_Fast_Codec(message_id);
  case SZSEL2_1_0_BINARY:
    return Create_SZSEL2_1_0_Binary_Codec(message_id);
  case SZSEL2_1_1_FAST:
    return Create_SZSEL2_1_1_Fast_Codec(message_id);
  case SZSEL2_1_1_BINARY:
    return Create_SZSEL2_1_1_Binary_Codec(message_id);

    /*中金所L1*/  
  case CFFEL1_1_0_FAST:
    return Create_CFFEL1_1_0_Fast_Codec(message_id);  
  case CFFEL1_1_0_BINARY:
    return Create_CFFEL1_1_0_Binary_Codec(message_id);  
    /*中金所L2*/
  case CFFEL2_1_0_FAST:
    return Create_CFFEL2_1_0_Fast_Codec(message_id);
  case CFFEL2_1_0_BINARY:
    return Create_CFFEL2_1_0_Binary_Codec(message_id);
  case CFFEL2_1_1_FAST:
    return Create_CFFEL2_1_1_Fast_Codec(message_id);
  case CFFEL2_1_1_BINARY:
    return Create_CFFEL2_1_1_Binary_Codec(message_id);
    /*上期所L1*/
  case SHFEL1_1_0_FAST:
    return Create_SHFEL1_1_0_Fast_Codec(message_id);
  case SHFEL1_1_0_BINARY:
    return Create_SHFEL1_1_0_Binary_Codec(message_id);
    /*大商所L1*/
  case DCEL1_1_0_FAST:
    return Create_DCEL1_1_0_Fast_Codec(message_id);
  case DCEL1_1_0_BINARY:
    return Create_DCEL1_1_0_Binary_Codec(message_id);
    /*郑商所L1*/
  case CZCEL1_1_0_FAST:
    return Create_CZCEL1_1_0_Fast_Codec(message_id);
  case CZCEL1_1_0_BINARY:
    return Create_CZCEL1_1_0_Binary_Codec(message_id);
  /*新华富时指数*/
  case FTSEL1_1_0_FAST:
    return Create_FTSEL1_1_0_Fast_Codec(message_id);
  case FTSEL1_1_0_BINARY:
    return Create_FTSEL1_1_0_Binary_Codec(message_id);
    /*申万指数*/
  case SWINDEX_1_0_FAST:
    return Create_SWIndex_1_0_Fast_Codec(message_id);
  case SWINDEX_1_0_BINARY:
    return Create_SWIndex_1_0_Binary_Codec(message_id);
	case CSINDEX_1_0_FAST:
		return Create_CSIndex_1_0_Fast_Codec(message_id);
	case CSINDEX_1_0_BINARY:
		return Create_CSIndex_1_0_Binary_Codec(message_id);
	case CIQINDEX_1_0_FAST:
		return Create_CIQIndex_1_0_Fast_Codec(message_id);
	case CIQINDEX_1_0_BINARY:
		return Create_CIQIndex_1_0_Binary_Codec(message_id);
	case WINDINDEX_1_0_FAST:
		return Create_WindIndex_1_0_Fast_Codec(message_id);
	case WINDINDEX_1_0_BINARY:
		return Create_WindIndex_1_0_Binary_Codec(message_id);
  default:
    return NULL;
  }	
}


HeaderCodec* wmdf::ProtocolProcessorFactory::CreateHeaderCodec(int64_t protocol_unique_id )
{
  switch(protocol_unique_id)
  {
  case SYSTEM_1_0_BINARY:
    return CreateSystemHeaderCodec(1,0);
  case MONITOR_1_0_BINARY:
    return CreateMonitorHeaderCodec();
	case SSEL1_1_0_FAST		:	
	case SSEL1_1_0_BINARY	:
	case SSEL2_1_0_FAST		:
	case SSEL2_1_0_BINARY	:
	case SZSEL1_1_0_FAST	:
	case SZSEL1_1_0_BINARY:
	case SZSEL2_1_0_FAST	:	
	case SZSEL2_1_0_BINARY:
	case CFFEL1_1_0_FAST	:	
	case CFFEL1_1_0_BINARY:
	case CFFEL2_1_0_FAST	:	
	case CFFEL2_1_0_BINARY:
  case CFFEL2_1_1_FAST	:	
  case CFFEL2_1_1_BINARY:
	case SHFEL1_1_0_FAST	:	
	case SHFEL1_1_0_BINARY:
	case DCEL1_1_0_FAST		:
	case DCEL1_1_0_BINARY	:
	case CZCEL1_1_0_FAST	:	
	case CZCEL1_1_0_BINARY:
  case SSEL2_1_1_FAST:
  case SSEL2_1_1_BINARY:
  case SZSEL2_1_1_FAST:
  case SZSEL2_1_1_BINARY:
  case FTSEL1_1_0_FAST:
  case FTSEL1_1_0_BINARY:
  case SWINDEX_1_0_FAST:
  case SWINDEX_1_0_BINARY:
	case CSINDEX_1_0_FAST:
	case CSINDEX_1_0_BINARY:
	case CIQINDEX_1_0_FAST:
	case CIQINDEX_1_0_BINARY:
	case WINDINDEX_1_0_FAST:
	case WINDINDEX_1_0_BINARY:
		return CreateMarketHeaderCodec(); 
  default:
    return NULL;
  }
}

HeaderCodec* wmdf::ProtocolProcessorFactory::CreateSystemHeaderCodec(int8_t major_version,int8_t minor_version)
{
  SystemMessageHeader* header = new SystemMessageHeader();
  header->major_version = major_version;
  header->minor_version = minor_version;
  HeaderCodec* codec = new SystemHeaderCodec();
  codec->set_header(header);
  return codec;
}

HeaderCodec* wmdf::ProtocolProcessorFactory::CreateMonitorHeaderCodec()
{
  MonitorMessageHeader* header = new MonitorMessageHeader();
  HeaderCodec* codec = new MonitorHeaderCodec();
  codec->set_header(header);
  return codec;
}

HeaderCodec* wmdf::ProtocolProcessorFactory::CreateMarketHeaderCodec()
{
  MarketMessageHeader* header = new MarketMessageHeader();
  HeaderCodec* codec = new MarketHeaderCodec();
  codec->set_header(header);
  return codec;
}

ProtocolProcessor* wmdf::ProtocolProcessorFactory::CreateProtocolCodec(ProtocolDescription* protocolDescription)
{	
  ProtocolProcessor* protocolProcessor = new ProtocolProcessor(protocolDescription);	
  int64_t uniqueID = protocolProcessor->unique_id();
  HeaderCodec* headerCodec = CreateHeaderCodec(uniqueID);
  protocolProcessor->set_header_codec(headerCodec);
	switch(uniqueID)
	{
  case SYSTEM_1_0_BINARY:
    protocolProcessor->AddMessageCodec(SYSTEM_HEARTBEAT_ID,CreateMessageCodec(uniqueID,SYSTEM_HEARTBEAT_ID));    
    protocolProcessor->AddMessageCodec(SYSTEM_LOGIN_REQUEST_ID,CreateMessageCodec(uniqueID,SYSTEM_LOGIN_REQUEST_ID));
    protocolProcessor->AddMessageCodec(SYSTEM_LOGIN_RESPONSE_ID,CreateMessageCodec(uniqueID,SYSTEM_LOGIN_RESPONSE_ID));
    protocolProcessor->AddMessageCodec(SYSTEM_LOGOUT_ID,CreateMessageCodec(uniqueID,SYSTEM_LOGOUT_ID));		
    protocolProcessor->AddMessageCodec(SYSTEM_MARKET_DATE_SIGNAL_ID,CreateMessageCodec(uniqueID,SYSTEM_MARKET_DATE_SIGNAL_ID));		
    protocolProcessor->AddMessageCodec(SYSTEM_CONFIG_INFO_REQUEST_ID,CreateMessageCodec(uniqueID,SYSTEM_CONFIG_INFO_REQUEST_ID));
    protocolProcessor->AddMessageCodec(SYSTEM_CONFIG_INFO_ID,CreateMessageCodec(uniqueID,SYSTEM_CONFIG_INFO_ID));
    protocolProcessor->AddMessageCodec(SYSTEM_UPDATE_CONFIG_RESPONSE_ID,CreateMessageCodec(uniqueID,SYSTEM_UPDATE_CONFIG_RESPONSE_ID));
		break;
  case MONITOR_1_0_BINARY:    
    protocolProcessor->AddMessageCodec(MONITOR_TRANSFER_STATUS_ID,CreateMessageCodec(uniqueID,MONITOR_TRANSFER_STATUS_ID));		
    protocolProcessor->AddMessageCodec(MONITOR_CLIENT_STATUS_ID,CreateMessageCodec(uniqueID,MONITOR_CLIENT_STATUS_ID));		
    protocolProcessor->AddMessageCodec(MONITOR_EVENT_ID,CreateMessageCodec(uniqueID,MONITOR_EVENT_ID));
    break;
  /*上交所*/
	case SSEL1_1_0_FAST:
		protocolProcessor->AddMessageCodec(SSEL1_MARKETDATA_ID,CreateMessageCodec(uniqueID,SSEL1_MARKETDATA_ID));
//     protocolProcessor->AddMessageCodec(SSEL1_TRANSACTION_ID,CreateMessageCodec(uniqueID,SSEL1_TRANSACTION_ID));
//     protocolProcessor->AddMessageCodec(SSEL1_MINUTEBAR_ID,CreateMessageCodec(uniqueID,SSEL1_MINUTEBAR_ID));
		break;
  case SSEL1_1_0_BINARY:
    protocolProcessor->AddMessageCodec(SSEL1_MARKETDATA_ID,CreateMessageCodec(uniqueID,SSEL1_MARKETDATA_ID));
//     protocolProcessor->AddMessageCodec(SSEL1_TRANSACTION_ID,CreateMessageCodec(uniqueID,SSEL1_TRANSACTION_ID));
//     protocolProcessor->AddMessageCodec(SSEL1_MINUTEBAR_ID,CreateMessageCodec(uniqueID,SSEL1_MINUTEBAR_ID));
    break;
  case SSEL2_1_0_FAST:
  case SSEL2_1_1_FAST:
    protocolProcessor->AddMessageCodec(SSEL2_MARKETDATA_ID,CreateMessageCodec(uniqueID,SSEL2_MARKETDATA_ID));
    protocolProcessor->AddMessageCodec(SSEL2_TRANSACTION_ID,CreateMessageCodec(uniqueID,SSEL2_TRANSACTION_ID));
    protocolProcessor->AddMessageCodec(SSEL2_ORDERQUEUE_ID,CreateMessageCodec(uniqueID,SSEL2_ORDERQUEUE_ID));
    //protocolProcessor->AddMessageCodec(SSEL2_MINUTEBAR_ID,CreateMessageCodec(uniqueID,SSEL2_MINUTEBAR_ID));
    protocolProcessor->AddMessageCodec(SSEL2_INDEXDATA_ID,CreateMessageCodec(uniqueID,SSEL2_INDEXDATA_ID));
		break;	
	case SSEL2_1_0_BINARY:
  case SSEL2_1_1_BINARY:
		protocolProcessor->AddMessageCodec(SSEL2_MARKETDATA_ID,CreateMessageCodec(uniqueID,SSEL2_MARKETDATA_ID));
    protocolProcessor->AddMessageCodec(SSEL2_TRANSACTION_ID,CreateMessageCodec(uniqueID,SSEL2_TRANSACTION_ID));
    protocolProcessor->AddMessageCodec(SSEL2_ORDERQUEUE_ID,CreateMessageCodec(uniqueID,SSEL2_ORDERQUEUE_ID));
    //protocolProcessor->AddMessageCodec(SSEL2_MINUTEBAR_ID,CreateMessageCodec(uniqueID,SSEL2_MINUTEBAR_ID));
    protocolProcessor->AddMessageCodec(SSEL2_INDEXDATA_ID,CreateMessageCodec(uniqueID,SSEL2_INDEXDATA_ID));
		break;
	/*深交所*/
	case SZSEL1_1_0_FAST:
		protocolProcessor->AddMessageCodec(SZSEL1_MARKETDATA_ID,CreateMessageCodec(uniqueID,SZSEL1_MARKETDATA_ID));
//     protocolProcessor->AddMessageCodec(SZSEL1_TRANSACTION_ID,CreateMessageCodec(uniqueID,SZSEL1_TRANSACTION_ID));
//     protocolProcessor->AddMessageCodec(SZSEL1_MINUTEBAR_ID,CreateMessageCodec(uniqueID,SZSEL1_MINUTEBAR_ID));
		break;
	case SZSEL1_1_0_BINARY:
		protocolProcessor->AddMessageCodec(SZSEL1_MARKETDATA_ID,CreateMessageCodec(uniqueID,SZSEL1_MARKETDATA_ID));
//     protocolProcessor->AddMessageCodec(SZSEL1_TRANSACTION_ID,CreateMessageCodec(uniqueID,SZSEL1_TRANSACTION_ID));
//     protocolProcessor->AddMessageCodec(SZSEL1_MINUTEBAR_ID,CreateMessageCodec(uniqueID,SZSEL1_MINUTEBAR_ID));
		break;
  case SZSEL2_1_0_FAST:
  case SZSEL2_1_1_FAST:
    protocolProcessor->AddMessageCodec(SZSEL2_MARKETDATA_ID,CreateMessageCodec(uniqueID,SZSEL2_MARKETDATA_ID));
    protocolProcessor->AddMessageCodec(SZSEL2_TRANSACTION_ID,CreateMessageCodec(uniqueID,SZSEL2_TRANSACTION_ID));
    protocolProcessor->AddMessageCodec(SZSEL2_ORDERQUEUE_ID,CreateMessageCodec(uniqueID,SZSEL2_ORDERQUEUE_ID));
//    protocolProcessor->AddMessageCodec(SZSEL2_MINUTEBAR_ID,CreateMessageCodec(uniqueID,SZSEL2_MINUTEBAR_ID));
    protocolProcessor->AddMessageCodec(SZSEL2_SINGLEORDER_ID,CreateMessageCodec(uniqueID,SZSEL2_SINGLEORDER_ID));
    protocolProcessor->AddMessageCodec(SZSEL2_INDEXDATA_ID,CreateMessageCodec(uniqueID,SZSEL2_INDEXDATA_ID));
    break;
	case SZSEL2_1_0_BINARY:
  case SZSEL2_1_1_BINARY:
		protocolProcessor->AddMessageCodec(SZSEL2_MARKETDATA_ID,CreateMessageCodec(uniqueID,SZSEL2_MARKETDATA_ID));
    protocolProcessor->AddMessageCodec(SZSEL2_TRANSACTION_ID,CreateMessageCodec(uniqueID,SZSEL2_TRANSACTION_ID));
    protocolProcessor->AddMessageCodec(SZSEL2_ORDERQUEUE_ID,CreateMessageCodec(uniqueID,SZSEL2_ORDERQUEUE_ID));
//    protocolProcessor->AddMessageCodec(SZSEL2_MINUTEBAR_ID,CreateMessageCodec(uniqueID,SZSEL2_MINUTEBAR_ID));
    protocolProcessor->AddMessageCodec(SZSEL2_SINGLEORDER_ID,CreateMessageCodec(uniqueID,SZSEL2_SINGLEORDER_ID));
    protocolProcessor->AddMessageCodec(SZSEL2_INDEXDATA_ID,CreateMessageCodec(uniqueID,SZSEL2_INDEXDATA_ID));
		break;

	/*中金所L1*/
	case CFFEL1_1_0_FAST:
    protocolProcessor->AddMessageCodec(CFFEL1_MARKETDATA_ID,CreateMessageCodec(uniqueID,CFFEL1_MARKETDATA_ID));
//     protocolProcessor->AddMessageCodec(CFFEL1_TRANSACTION_ID,CreateMessageCodec(uniqueID,CFFEL1_TRANSACTION_ID));
//     protocolProcessor->AddMessageCodec(CFFEL1_MINUTEBAR_ID,CreateMessageCodec(uniqueID,CFFEL1_MINUTEBAR_ID));
  /*上期所L1*/
  case SHFEL1_1_0_FAST:
    protocolProcessor->AddMessageCodec(SHFEL1_MARKETDATA_ID,CreateMessageCodec(uniqueID,SHFEL1_MARKETDATA_ID));
//     protocolProcessor->AddMessageCodec(SHFEL1_TRANSACTION_ID,CreateMessageCodec(uniqueID,SHFEL1_TRANSACTION_ID));
//     protocolProcessor->AddMessageCodec(SHFEL1_MINUTEBAR_ID,CreateMessageCodec(uniqueID,SHFEL1_MINUTEBAR_ID));
  /*大商所L1*/
  case DCEL1_1_0_FAST:
    protocolProcessor->AddMessageCodec(DCEL1_MARKETDATA_ID,CreateMessageCodec(uniqueID,DCEL1_MARKETDATA_ID));
//     protocolProcessor->AddMessageCodec(DCEL1_TRANSACTION_ID,CreateMessageCodec(uniqueID,DCEL1_TRANSACTION_ID));
//     protocolProcessor->AddMessageCodec(DCEL1_MINUTEBAR_ID,CreateMessageCodec(uniqueID,DCEL1_MINUTEBAR_ID));
  /*郑商所L1*/
  case CZCEL1_1_0_FAST:
		protocolProcessor->AddMessageCodec(CZCEL1_MARKETDATA_ID,CreateMessageCodec(uniqueID,CZCEL1_MARKETDATA_ID));
//     protocolProcessor->AddMessageCodec(CZCEL1_TRANSACTION_ID,CreateMessageCodec(uniqueID,CZCEL1_TRANSACTION_ID));
//     protocolProcessor->AddMessageCodec(CZCEL1_MINUTEBAR_ID,CreateMessageCodec(uniqueID,CZCEL1_MINUTEBAR_ID));
		break;	
	/*中金所L1*/
  case CFFEL1_1_0_BINARY:
    protocolProcessor->AddMessageCodec(CFFEL1_MARKETDATA_ID,CreateMessageCodec(uniqueID,CFFEL1_MARKETDATA_ID));
//     protocolProcessor->AddMessageCodec(CFFEL1_TRANSACTION_ID,CreateMessageCodec(uniqueID,CFFEL1_TRANSACTION_ID));
//     protocolProcessor->AddMessageCodec(CFFEL1_MINUTEBAR_ID,CreateMessageCodec(uniqueID,CFFEL1_MINUTEBAR_ID));
  /*上期所L1*/
  case SHFEL1_1_0_BINARY:
    protocolProcessor->AddMessageCodec(SHFEL1_MARKETDATA_ID,CreateMessageCodec(uniqueID,SHFEL1_MARKETDATA_ID));
//     protocolProcessor->AddMessageCodec(SHFEL1_TRANSACTION_ID,CreateMessageCodec(uniqueID,SHFEL1_TRANSACTION_ID));
//     protocolProcessor->AddMessageCodec(SHFEL1_MINUTEBAR_ID,CreateMessageCodec(uniqueID,SHFEL1_MINUTEBAR_ID));
  /*大商所L1*/
  case DCEL1_1_0_BINARY:
    protocolProcessor->AddMessageCodec(DCEL1_MARKETDATA_ID,CreateMessageCodec(uniqueID,DCEL1_MARKETDATA_ID));
//     protocolProcessor->AddMessageCodec(DCEL1_TRANSACTION_ID,CreateMessageCodec(uniqueID,DCEL1_TRANSACTION_ID));
//     protocolProcessor->AddMessageCodec(DCEL1_MINUTEBAR_ID,CreateMessageCodec(uniqueID,DCEL1_MINUTEBAR_ID));
  /*郑商所L1*/
  case CZCEL1_1_0_BINARY:
		protocolProcessor->AddMessageCodec(CZCEL1_MARKETDATA_ID,CreateMessageCodec(uniqueID,CZCEL1_MARKETDATA_ID));
//     protocolProcessor->AddMessageCodec(CZCEL1_TRANSACTION_ID,CreateMessageCodec(uniqueID,CZCEL1_TRANSACTION_ID));
//     protocolProcessor->AddMessageCodec(CZCEL1_MINUTEBAR_ID,CreateMessageCodec(uniqueID,CZCEL1_MINUTEBAR_ID));
		break;

  /*中金所L2*/
  case CFFEL2_1_0_FAST:
  case CFFEL2_1_1_FAST:
    protocolProcessor->AddMessageCodec(CFFEL2_MARKETDATA_ID,CreateMessageCodec(uniqueID,CFFEL2_MARKETDATA_ID));
//     protocolProcessor->AddMessageCodec(CFFEL2_TRANSACTION_ID,CreateMessageCodec(uniqueID,CFFEL2_TRANSACTION_ID));
//     protocolProcessor->AddMessageCodec(CFFEL2_MINUTEBAR_ID,CreateMessageCodec(uniqueID,CFFEL2_MINUTEBAR_ID));
    break;
	case CFFEL2_1_0_BINARY:
  case CFFEL2_1_1_BINARY:
		protocolProcessor->AddMessageCodec(CFFEL2_MARKETDATA_ID,CreateMessageCodec(uniqueID,CFFEL2_MARKETDATA_ID));
//     protocolProcessor->AddMessageCodec(CFFEL2_TRANSACTION_ID,CreateMessageCodec(uniqueID,CFFEL2_TRANSACTION_ID));
//     protocolProcessor->AddMessageCodec(CFFEL2_MINUTEBAR_ID,CreateMessageCodec(uniqueID,CFFEL2_MINUTEBAR_ID));
		break;

  /*新华富时指数*/
  case FTSEL1_1_0_FAST:
    protocolProcessor->AddMessageCodec(FTSE_INDEXDATA_ID,CreateMessageCodec(uniqueID,FTSE_INDEXDATA_ID));
    break;
  case FTSEL1_1_0_BINARY:
    protocolProcessor->AddMessageCodec(FTSE_INDEXDATA_ID,CreateMessageCodec(uniqueID,FTSE_INDEXDATA_ID));
    break;
    /*申万指数*/
  case SWINDEX_1_0_FAST:
    protocolProcessor->AddMessageCodec(SW_INDEXDATA_ID,CreateMessageCodec(uniqueID,SW_INDEXDATA_ID));
    break;
  case SWINDEX_1_0_BINARY:
    protocolProcessor->AddMessageCodec(SW_INDEXDATA_ID,CreateMessageCodec(uniqueID,SW_INDEXDATA_ID));
    break;
	case CSINDEX_1_0_FAST:
		protocolProcessor->AddMessageCodec(CS_INDEXDATA_ID,CreateMessageCodec(uniqueID,CS_INDEXDATA_ID));
		break;
	case CSINDEX_1_0_BINARY:
		protocolProcessor->AddMessageCodec(CS_INDEXDATA_ID,CreateMessageCodec(uniqueID,CS_INDEXDATA_ID));
		break;
	case CIQINDEX_1_0_FAST:
		protocolProcessor->AddMessageCodec(CIQ_INDEXDATA_ID,CreateMessageCodec(uniqueID,CIQ_INDEXDATA_ID));
		break;
	case CIQINDEX_1_0_BINARY:
		protocolProcessor->AddMessageCodec(CIQ_INDEXDATA_ID,CreateMessageCodec(uniqueID,CIQ_INDEXDATA_ID));
		break;
	case WINDINDEX_1_0_FAST:
		protocolProcessor->AddMessageCodec(WIND_INDEXDATA_ID,CreateMessageCodec(uniqueID,WIND_INDEXDATA_ID));
		break;
	case WINDINDEX_1_0_BINARY:
		protocolProcessor->AddMessageCodec(WIND_INDEXDATA_ID,CreateMessageCodec(uniqueID,WIND_INDEXDATA_ID));
		break;
	default:
		break;
	}	
  return protocolProcessor;
}






MessageCodec* wmdf::ProtocolProcessorFactory::Create_System_1_0_Binary_Codec(int16_t message_id)
{
  switch(message_id)
  {
  case SYSTEM_HEARTBEAT_ID:
    return new BinaryCodecHeartBeat();
  case SYSTEM_LOGIN_REQUEST_ID:
    return new BinaryCodecLoginRequest();
  case SYSTEM_LOGIN_RESPONSE_ID:
    return new BinaryCodecLoginResponse();
  case SYSTEM_LOGOUT_ID:
    return new BinaryCodecLogout();
  case SYSTEM_MARKET_DATE_SIGNAL_ID:
    return new BinaryCodecDateSignal();
  case SYSTEM_CONFIG_INFO_REQUEST_ID:
    return new BinaryCodecConfigInfoRequest();
  case SYSTEM_CONFIG_INFO_ID:
    return new BinaryCodecConfigInfo();
  case SYSTEM_UPDATE_CONFIG_RESPONSE_ID:
    return new BinaryCodecConfigResponse();
  default:
    break;
  }
  return NULL;
}

MessageCodec* wmdf::ProtocolProcessorFactory::Create_Monitor_1_0_Binary_Codec(int16_t message_id)
{
  switch(message_id)
  {
  case MONITOR_TRANSFER_STATUS_ID:
    return new BinaryCodecTransferStat();
  case MONITOR_CLIENT_STATUS_ID:
    return new BinaryCodecClientStatus();
  case MONITOR_EVENT_ID:
    return new BinaryCodecMonitorEvent();
  default:
    break;
  }
  return NULL;
}


MessageCodec* wmdf::ProtocolProcessorFactory::Create_SSEL1_1_0_Fast_Codec(int16_t message_id)
{
  switch(message_id)
  {
  case SSEL1_MARKETDATA_ID:
    return new FastCodecSHSZL1MarketData();
//   case SSEL1_TRANSACTION_ID:
//     //TODO(xdyuan)
//     break;
//   case SSEL1_MINUTEBAR_ID:
//     //TODO(xdyuan)
//     break;
  default:
    break;
  }
  return NULL;
}

MessageCodec* wmdf::ProtocolProcessorFactory::Create_SSEL1_1_0_Binary_Codec(int16_t message_id)
{
  switch(message_id)
  {
  case SSEL1_MARKETDATA_ID:
    return new BinaryCodecSHSZL1MarketData();
// 	case SSEL1_TRANSACTION_ID:
// 		//TODO(xdyuan)
// 		break;
// 	case SSEL1_MINUTEBAR_ID:
// 		//TODO(xdyuan)
// 		break;
   default:
    break;
  }
return NULL;
}

MessageCodec* wmdf::ProtocolProcessorFactory::Create_SSEL2_1_0_Fast_Codec(int16_t message_id)
{
  switch(message_id)
  {
  case SSEL2_MARKETDATA_ID:
    return new FastCodecSHL2MarketData();
  case SSEL2_TRANSACTION_ID:
    return new FastCodecSHL2Trans();
  case SSEL2_ORDERQUEUE_ID:
    return new FastCodecSHL2OrderQueue();
  case SSEL2_INDEXDATA_ID:
    return new FastCodecIndexMarketData();
  default:
    break;
  }
  return NULL;
}

MessageCodec* wmdf::ProtocolProcessorFactory::Create_SSEL2_1_1_Fast_Codec(int16_t message_id)
{
  switch(message_id)
  {
  case SSEL2_MARKETDATA_ID:
    return new FastCodec_1_1_SHL2MarketData();
  case SSEL2_TRANSACTION_ID:
    return new FastCodecSHL2Trans();
  case SSEL2_ORDERQUEUE_ID:
    return new FastCodecSHL2OrderQueue();
  case SSEL2_INDEXDATA_ID:
    return new FastCodecIndexMarketData();
  default:
    break;
  }
  return NULL;
}

MessageCodec* wmdf::ProtocolProcessorFactory::Create_SSEL2_1_0_Binary_Codec(int16_t message_id)
{
  switch(message_id)
  {
  case SSEL2_MARKETDATA_ID:
    return new BinaryCodecSHL2MarketData();
  case SSEL2_TRANSACTION_ID:
    return new BinaryCodecSHL2Trans();
  case SSEL2_ORDERQUEUE_ID:
    return new BinaryCodecSHL2OrderQueue(); 
  case SSEL2_INDEXDATA_ID:
    return new BinaryCodecIndexMarketData();
  default:
    break;
  }
  return NULL;
}

MessageCodec* wmdf::ProtocolProcessorFactory::Create_SSEL2_1_1_Binary_Codec(int16_t message_id)
{
  switch(message_id)
  {
  case SSEL2_MARKETDATA_ID:
    return new BinaryCodec_1_1_SHL2MarketData();
  case SSEL2_TRANSACTION_ID:
    return new BinaryCodecSHL2Trans();
  case SSEL2_ORDERQUEUE_ID:
    return new BinaryCodecSHL2OrderQueue();
  case SSEL2_INDEXDATA_ID:
    return new BinaryCodecIndexMarketData();
  default:
    break;
  }
  return NULL;
}

MessageCodec* wmdf::ProtocolProcessorFactory::Create_SZSEL1_1_0_Fast_Codec(int16_t message_id)
{
  switch(message_id)
  {
  case SZSEL1_MARKETDATA_ID:
    return new FastCodecSHSZL1MarketData();
// 	case SZSEL1_TRANSACTION_ID:
// 		//TODO(xdyuan)
// 		break;
// 	case SZSEL1_MINUTEBAR_ID:
// 		//TODO(xdyuan)
// 		break;
  default:
    break;
  }
  return NULL;
}

MessageCodec* wmdf::ProtocolProcessorFactory::Create_SZSEL1_1_0_Binary_Codec(int16_t message_id)
{
  switch(message_id)
  {
  case SZSEL1_MARKETDATA_ID:
    return new BinaryCodecSHSZL1MarketData();
// 	case SZSEL1_TRANSACTION_ID:
// 		//TODO(xdyuan)
// 		break;
// 	case SZSEL1_MINUTEBAR_ID:
// 		//TODO(xdyuan)
		break;
  default:
    break;
  }
	return NULL;
}

MessageCodec* wmdf::ProtocolProcessorFactory::Create_SZSEL2_1_0_Fast_Codec(int16_t message_id)
{
  switch(message_id)
  {
  case SZSEL2_MARKETDATA_ID:
    return new FastCodecSZL2MarketData();
  case SZSEL2_TRANSACTION_ID:
    return new FastCodecSZL2Trans();
  case SZSEL2_ORDERQUEUE_ID:
    return new FastCodecSHL2OrderQueue();
//   case SZSEL2_MINUTEBAR_ID:
//     break;//TODO(xdyuan)
  case SZSEL2_SINGLEORDER_ID:
    return new FastCodecSZL2Orders();
  case SZSEL2_INDEXDATA_ID:
    return new FastCodecIndexMarketData();
  default:
    break;
  }
  return NULL;
}

MessageCodec* wmdf::ProtocolProcessorFactory::Create_SZSEL2_1_1_Fast_Codec(int16_t message_id)
{
  switch(message_id)
  {
  case SZSEL2_MARKETDATA_ID:
    return new FastCodec_1_1_SZL2MarketData();
  case SZSEL2_TRANSACTION_ID:
    return new FastCodecSZL2Trans();
  case SZSEL2_ORDERQUEUE_ID:
    return new FastCodecSHL2OrderQueue();
    //   case SZSEL2_MINUTEBAR_ID:
    //     break;//TODO(xdyuan)
  case SZSEL2_SINGLEORDER_ID:
    return new FastCodecSZL2Orders();
  case SZSEL2_INDEXDATA_ID:
    return new FastCodecIndexMarketData();
  default:
    break;
  }
  return NULL;
}

MessageCodec* wmdf::ProtocolProcessorFactory::Create_SZSEL2_1_0_Binary_Codec(int16_t message_id)
{
  switch(message_id)
  {
  case SZSEL2_MARKETDATA_ID:
    return new BinaryCodecSHL2MarketData();
  case SZSEL2_TRANSACTION_ID:
    return new BinaryCodecSHL2Trans();
  case SZSEL2_ORDERQUEUE_ID:
    return new BinaryCodecSHL2OrderQueue();
//   case SZSEL2_MINUTEBAR_ID:
//     break;//TODO(xdyuan)
  case SZSEL2_SINGLEORDER_ID:
    return new BinaryCodecSZL2Orders();
  case SZSEL2_INDEXDATA_ID:
    return new BinaryCodecIndexMarketData();
  default:
    break;
  }
  return NULL;
}
MessageCodec* wmdf::ProtocolProcessorFactory::Create_SZSEL2_1_1_Binary_Codec(int16_t message_id)
{
  switch(message_id)
  {
  case SZSEL2_MARKETDATA_ID:
    return new BinaryCodec_1_1_SHL2MarketData();
  case SZSEL2_TRANSACTION_ID:
    return new BinaryCodecSHL2Trans();
  case SZSEL2_ORDERQUEUE_ID:
    return new BinaryCodecSHL2OrderQueue();
    //   case SZSEL2_MINUTEBAR_ID:
    //     break;//TODO(xdyuan)
  case SZSEL2_SINGLEORDER_ID:
    return new BinaryCodecSZL2Orders();
  case SZSEL2_INDEXDATA_ID:
    return new BinaryCodecIndexMarketData();
  default:
    break;
  }
  return NULL;
}


MessageCodec* wmdf::ProtocolProcessorFactory::Create_CFFEL1_1_0_Fast_Codec(int16_t message_id)
{
  switch(message_id)
  {
  case CFFEL1_MARKETDATA_ID:
    return new FastCodecFutureL1MarketData();
  default:
    break;
  }
return NULL;
}

MessageCodec* wmdf::ProtocolProcessorFactory::Create_CFFEL1_1_0_Binary_Codec(int16_t message_id)
{
  switch(message_id)
  {
  case CFFEL1_MARKETDATA_ID:
    return new BinaryCodecFutureL1MarketData();
  default:
    break;
  }
  return NULL;
}

MessageCodec* wmdf::ProtocolProcessorFactory::Create_CFFEL2_1_0_Fast_Codec(int16_t message_id)
{
  switch(message_id)
  {
  case CFFEL2_MARKETDATA_ID:
    return new FastCodecCFFEL2MarketData();
  default:
    break;
  }
  return NULL;
}

MessageCodec* wmdf::ProtocolProcessorFactory::Create_CFFEL2_1_0_Binary_Codec(int16_t message_id)
{
  switch(message_id)
  {
  case CFFEL2_MARKETDATA_ID:
    return new BinaryCodecCFFEL2MarketData();
  default:
    break;
  }
  return NULL;
}

MessageCodec* wmdf::ProtocolProcessorFactory::Create_SHFEL1_1_0_Fast_Codec(int16_t message_id)
{
  switch(message_id)
  {
  case SHFEL1_MARKETDATA_ID:
    return new FastCodecFutureL1MarketData();
  default:
    break;
  }
  return NULL;
}

MessageCodec* wmdf::ProtocolProcessorFactory::Create_SHFEL1_1_0_Binary_Codec(int16_t message_id)
{
  switch(message_id)
  {
  case SHFEL1_MARKETDATA_ID:
    return new BinaryCodecFutureL1MarketData();
  default:
    break;
  }
  return NULL;
}

MessageCodec* wmdf::ProtocolProcessorFactory::Create_DCEL1_1_0_Fast_Codec(int16_t message_id)
{
  switch(message_id)
  {
  case DCEL1_MARKETDATA_ID:
    return new FastCodecFutureL1MarketData();
  default:
    break;
  }
  return NULL;
}

MessageCodec* wmdf::ProtocolProcessorFactory::Create_DCEL1_1_0_Binary_Codec(int16_t message_id)
{
  switch(message_id)
  {
  case DCEL1_MARKETDATA_ID:
    return new BinaryCodecFutureL1MarketData();
  default:
    break;
  }
  return NULL;
}

MessageCodec* wmdf::ProtocolProcessorFactory::Create_CZCEL1_1_0_Fast_Codec(int16_t message_id)
{
  switch(message_id)
  {
  case CZCEL1_MARKETDATA_ID:
    return new FastCodecFutureL1MarketData();
  default:
    break;
  }
  return NULL;
}

MessageCodec* wmdf::ProtocolProcessorFactory::Create_CZCEL1_1_0_Binary_Codec(int16_t message_id)
{
  switch(message_id)
  {
  case CZCEL1_MARKETDATA_ID:
    return new BinaryCodecFutureL1MarketData();
  default:
    break;
  }
  return NULL;
}

MessageCodec* wmdf::ProtocolProcessorFactory::Create_FTSEL1_1_0_Fast_Codec( int16_t message_id )
{
  switch(message_id)
  {
  case FTSE_INDEXDATA_ID:
    return new FastCodecIndexMarketData();
  default:
    break;
  }
  return NULL;
}

MessageCodec* wmdf::ProtocolProcessorFactory::Create_FTSEL1_1_0_Binary_Codec( int16_t message_id )
{
  switch(message_id)
  {
  case FTSE_INDEXDATA_ID:
    return new BinaryCodecIndexMarketData();
  default:
    break;
  }
  return NULL;
}

MessageCodec* wmdf::ProtocolProcessorFactory::Create_CFFEL2_1_1_Fast_Codec( int16_t message_id )
{
  switch(message_id)
  {
  case CFFEL2_MARKETDATA_ID:
    return new FastCodecCFFEL2_1_1_MarketData();
  default:
    break;
  }
  return NULL;
}

MessageCodec* wmdf::ProtocolProcessorFactory::Create_CFFEL2_1_1_Binary_Codec( int16_t message_id )
{
  switch(message_id)
  {
  case CFFEL2_MARKETDATA_ID:
    return new BinaryCodecCFFEL2_1_1_MarketData();
  default:
    break;
  }
  return NULL;
}

MessageCodec* wmdf::ProtocolProcessorFactory::Create_SWIndex_1_0_Fast_Codec( int16_t message_id )
{
	switch(message_id)
	{
	case SW_INDEXDATA_ID:
		return new FastCodecIndexMarketData();
	default:
		break;
	}
	return NULL;
}

MessageCodec* wmdf::ProtocolProcessorFactory::Create_SWIndex_1_0_Binary_Codec( int16_t message_id )
{
	switch(message_id)
	{
	case SW_INDEXDATA_ID:
		return new BinaryCodecIndexMarketData();
	default:
		break;
	}
	return NULL;
}

MessageCodec* wmdf::ProtocolProcessorFactory::Create_CSIndex_1_0_Fast_Codec( int16_t message_id )
{
	switch(message_id)
	{
	case CS_INDEXDATA_ID:
		return new FastCodecIndexMarketData();
	default:
		break;
	}
	return NULL;
}

MessageCodec* wmdf::ProtocolProcessorFactory::Create_CSIndex_1_0_Binary_Codec( int16_t message_id )
{
	switch(message_id)
	{
	case CS_INDEXDATA_ID:
		return new BinaryCodecIndexMarketData();
	default:
		break;
	}
	return NULL;
}

MessageCodec* wmdf::ProtocolProcessorFactory::Create_CIQIndex_1_0_Fast_Codec( int16_t message_id )
{
	switch (message_id)
	{
	case CIQ_INDEXDATA_ID:
		return new FastCodecIndexMarketData();
	default:
		break;
	}
	return NULL;
}

MessageCodec* wmdf::ProtocolProcessorFactory::Create_CIQIndex_1_0_Binary_Codec( int16_t message_id )
{
	switch(message_id)
	{
	case CIQ_INDEXDATA_ID:
		return new BinaryCodecIndexMarketData();
	default:
		break;
	}
	return NULL;
}

MessageCodec* wmdf::ProtocolProcessorFactory::Create_WindIndex_1_0_Fast_Codec( int16_t message_id )
{
	switch(message_id)
	{
	case WIND_INDEXDATA_ID:
		return new FastCodecIndexMarketData();
	default:
		break;
	}
	return NULL;
}

MessageCodec* wmdf::ProtocolProcessorFactory::Create_WindIndex_1_0_Binary_Codec( int16_t message_id )
{
	switch(message_id)
	{
	case WIND_INDEXDATA_ID:
		return new BinaryCodecIndexMarketData();
	default:
		break;
	}
	return NULL;
}





