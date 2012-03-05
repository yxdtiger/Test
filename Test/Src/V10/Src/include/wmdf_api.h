#ifndef WIND_MDF_API_H
#define WIND_MDF_API_H

#include <math.h>

#ifdef __cplusplus
extern "C" {
#endif

#if defined(_WIN32) || defined(_WIN32_WCE) || defined(__WIN32__)
#  ifndef WIN32
#    define WIN32
#  endif
#endif

#ifdef WIN32
#       pragma warning(disable:4091) 
#   if defined DLL_EXPORT
#       define WMDF_API __declspec(dllexport)
#   else
#       define WMDF_API __declspec(dllimport)
#   endif
#else
#   if defined __SUNPRO_C  || defined __SUNPRO_CC
#       define WMDF_API __global
#   elif (defined __GNUC__ && __GNUC__ >= 4) || defined __INTEL_COMPILER
#       define WMDF_API __attribute__ ((visibility("default")))
#   else
#       define WMDF_API
#   endif
#endif

  ////////////////////////////////////////////////////////////////
  //  Platform independent data types                           //
  ////////////////////////////////////////////////////////////////

#if (defined _MSC_VER &&_MSC_VER<=1500)
#ifndef int8_t
  typedef __int8 int8_t;
#endif
#ifndef int16_t
  typedef __int16 int16_t;
#endif
#ifndef int32_t
  typedef __int32 int32_t;
#endif
#ifndef int64_t
  typedef __int64 int64_t;
#endif
#ifndef uint8_t
  typedef unsigned __int8 uint8_t;
#endif
#ifndef uint16_t
  typedef unsigned __int16 uint16_t;
#endif
#ifndef uint32_t
  typedef unsigned __int32 uint32_t;
#endif
#ifndef uint64_t
  typedef unsigned __int64 uint64_t;
#endif
#else
#include <stdint.h>
#endif

#ifndef date_t
  typedef int32_t date_t;
#endif
#ifndef time_stamp_t
  typedef int32_t time_stamp_t;
#endif

#ifndef date_time_t
  typedef double date_time_t;
#endif
  

////////////////////////////////////////////////////////////////
//  NIL value of types                                        //
////////////////////////////////////////////////////////////////
#define WMDF_CONSTS_8BIT_NIL_VALUE   ((int8_t)0x80)                 
#define WMDF_CONSTS_32BIT_NIL_VALUE  ((int)0x80000000)
#define WMDF_CONSTS_64BIT_NIL_VALUE  ((int64_t)0x8000000000000000)
#define WMDF_CONSTS_DOUBLE_NIL_VALUE ((double)log(-1.0))
#define WMDF_CONSTS_STRING_NIL_VALUE (uint8_t*)""

  /************************************************************************/
  /*                         WMDF产品ID                                  */
  /************************************************************************/
  const int16_t PRODUCT_INVALID_ID              = 0;   //无效产品ID
  const int16_t PRODEUCT_SYSTEM_ID              = 1;   //系统消息
  const int16_t PRODEUCT_MONITOR_ID             = 2;   //监控产品
  const int16_t PRODUCT_SSE_LEVEL1_ID           = 10;  //上交所Level1
  const int16_t PRODUCT_SZSE_LEVEL1_ID          = 11;  //深交所Level1
  const int16_t PRODUCT_SSE_LEVEL2_ID           = 12;  //上交所Level2
  const int16_t PRODUCT_SZSE_LEVEL2_ID          = 13;  //深交所Level2

  const int16_t PRODUCT_CFFE_LEVEL1_ID          = 20;  //中金所Level1
  const int16_t PRODUCT_CFFE_LEVEL2_ID          = 21;  //中金所Level2
  const int16_t PRODUCT_SHFE_LEVEL1_ID          = 22;  //上期所Level1
  const int16_t PRODUCT_DCE_LEVEL1_ID           = 23;  //大商所Level1
  const int16_t PRODUCT_CZCE_LEVEL1_ID          = 24;  //郑商所Level1

  const int16_t PRODUCT_FX_ID                   = 25;   //外汇

  const int16_t PRODUCT_FTSE_LEVEL1_ID          = 26;   //新华富时
	const int16_t PRODUCT_SWINDEX_LEVEL1_ID					= 27;		//申万指数
	const int16_t PRODUCT_CSINDEX_LEVEL1_ID					= 28;		//中证指数	
	const int16_t PRODUCT_CIQINDEX_LEVEL1_ID				=29;		//中标指数
	const int16_t PRODUCT_WINDINDEX_LEVEL1_ID				=30;		//万得指数


  const int16_t PRODUCT_SIMULATOR_ID            = 2010; //产品模拟器，用于无行情源时的调试
  const int16_t PRODUCT_RELAY_BEGIN_ID          = 2011; //Feed Server Relay, 用于FeedServer的级联,起始ID
  const int16_t PRODUCT_RELAY_END_ID            = 2019; //Feed Server Relay, 用于FeedServer的级联，截止ID

  /************************************************************************/
  /*                         WMDF产品消息定义                        */
  /************************************************************************/
  const int16_t MESSAGE_INVALID_ID              = 00; //无效消息ID

  const int16_t SYSTEM_HEARTBEAT_ID             = 1;  //心跳消息
  const int16_t SYSTEM_LOGIN_REQUEST_ID         = 2;  //登录请求消息
  const int16_t SYSTEM_LOGIN_RESPONSE_ID        = 3;  //登录响应信息
  const int16_t SYSTEM_LOGOUT_ID                = 4;  //登出通知信息
  const int16_t SYSTEM_MARKET_DATE_SIGNAL_ID    = 5;  //市场开始前和结束后发送交易日期信号ID
  const int16_t SYSTEM_CONFIG_INFO_REQUEST_ID   = 6;  //配置请求信息
  const int16_t SYSTEM_CONFIG_INFO_ID           = 7;  //配置信息
  const int16_t SYSTEM_UPDATE_CONFIG_RESPONSE_ID  = 8;  //更新配置结果

  const int16_t MONITOR_TRANSFER_STATUS_ID      = 1;  // 网络传输状态消息
  const int16_t MONITOR_CLIENT_STATUS_ID        = 2;  // 监控客户端状态消息
  const int16_t MONITOR_EVENT_ID                = 3;  // 网络事件监控
  
  const int16_t SSEL1_MARKETDATA_ID             = 1;  //上交所Level1快照

  const int16_t SZSEL1_MARKETDATA_ID            = 1;  //深交所Level1快照

  const int16_t SSEL2_MARKETDATA_ID             = 1;  //上交所Level2快照
  const int16_t SSEL2_TRANSACTION_ID            = 2;  //上交所Level2逐笔成交
  const int16_t SSEL2_ORDERQUEUE_ID             = 3;  //上交所Level2委托队列
	const int16_t SSEL2_INDEXDATA_ID							= 4;	//上交所Level2指数行情

  const int16_t SZSEL2_MARKETDATA_ID            = 1;  //深交所Level2快照
  const int16_t SZSEL2_TRANSACTION_ID           = 2;  //深交所Level2逐笔成交
  const int16_t SZSEL2_ORDERQUEUE_ID            = 3;  //深交所Level2委托队列
  const int16_t SZSEL2_SINGLEORDER_ID           = 4;  //深交所Level2逐笔委托数据
	const int16_t SZSEL2_INDEXDATA_ID							= 5;	//深交所Level2指数行情

  const int16_t CFFEL1_MARKETDATA_ID            = 1;  //中金所Level1快照
  
  const int16_t CFFEL2_MARKETDATA_ID            = 1;  //中金所Level2快照

  const int16_t SHFEL1_MARKETDATA_ID            = 1;  //上期所Level1快照

  const int16_t DCEL1_MARKETDATA_ID             = 1;  //大商所Level1快照
  
  const int16_t CZCEL1_MARKETDATA_ID            = 1;  //郑商所Level1快照
  
  const int16_t FX_MARKETDATA_ID                = 1;  //外汇快照

  const int16_t FTSE_INDEXDATA_ID               = 1;  //新华富时指数快照
		
	const int16_t SW_INDEXDATA_ID								= 1;	//申万指数快照

	const int16_t CS_INDEXDATA_ID								= 1;	//中证指数

	const int16_t CIQ_INDEXDATA_ID							= 1;	//中标指数

	const int16_t WIND_INDEXDATA_ID							= 1;	//万得指数


  /************************************************************************/
  /*                         WMDF产品版本定义                        */
  /************************************************************************/
  const int8_t VERSION_INVALID_ID           = 0x7F;  // 无效版本ID

  const int8_t MAJOR_VERSION_1_SYSTEM_ID      = 0x01;  //基础传输产品
  const int8_t MINOR_VERSION_0_SYSTEM_ID      = 0x00;

  const int8_t MAJOR_VERSION_1_MONITOR_ID     = 0x01;  //监控产品
  const int8_t MINOR_VERSION_0_MONITOR_ID     = 0x00;

  const int8_t MAJOR_VERSION_1_SSE_LEVEL1_ID  = 0x01;  //上交所Level1
  const int8_t MINOR_VERSION_0_SSE_LEVEL1_ID  = 0x00;

  const int8_t MAJOR_VERSION_1_SZSE_LEVEL1_ID = 0x01;  //深交所Level1
  const int8_t MINOR_VERSION_0_SZSE_LEVEL1_ID = 0x00;

  const int8_t MAJOR_VERSION_1_SSE_LEVEL2_ID  = 0x01;  //上交所Level2
  const int8_t MINOR_VERSION_0_SSE_LEVEL2_ID  = 0x00;
  const int8_t MINOR_VERSION_1_SSE_LEVEL2_ID  = 0x01;

  const int8_t MAJOR_VERSION_1_SZSE_LEVEL2_ID = 0x01;  //深交所Level2
  const int8_t MINOR_VERSION_0_SZSE_LEVEL2_ID = 0x00;
  const int8_t MINOR_VERSION_1_SZSE_LEVEL2_ID = 0x01;

  const int8_t MAJOR_VERSION_1_CFFE_LEVEL1_ID = 0x01;  //中金所Level1
  const int8_t MINOR_VERSION_0_CFFE_LEVEL1_ID = 0x00;

  const int8_t MAJOR_VERSION_1_CFFE_LEVEL2_ID = 0x01;  //中金所Level2
  const int8_t MINOR_VERSION_0_CFFE_LEVEL2_ID = 0x00;
  const int8_t MINOR_VERSION_1_CFFE_LEVEL2_ID = 0x01;

  const int8_t MAJOR_VERSION_1_SHFE_LEVEL1_ID = 0x01;  //上期所Level1
  const int8_t MINOR_VERSION_0_SHFE_LEVEL1_ID = 0x00;

  const int8_t MAJOR_VERSION_1_DCE_LEVEL1_ID  = 0x01;  //大商所Level1
  const int8_t MINOR_VERSION_0_DCE_LEVEL1_ID  = 0x00;

  const int8_t MAJOR_VERSION_1_CZCE_LEVEL1_ID = 0x01;  //郑商所Level1
  const int8_t MINOR_VERSION_0_CZCE_LEVEL1_ID = 0x00;

  const int8_t MAJOR_VERSION_1_FX_ID          = 0x01;   //外汇
  const int8_t MINOR_VERSION_0_FX_ID          = 0x00;

  const int8_t MAJOR_VERSION_1_FTSE_ID        = 0x01;   //新华富时
  const int8_t MINOR_VERSION_0_FTSE_ID        = 0x00;

	const int8_t MAJOR_VERSION_1_SWINDEX_ID				= 0x01;		//申万指数
	const int8_t MINOR_VERSION_0_SWINDEX_ID				= 0x00;

	const int8_t MAJOR_VERSION_1_CSINDEX_ID				= 0x01;		//中证指数
	const int8_t MINOR_VERSION_0_CSINDEX_ID				= 0x00;

	const int8_t MAJOR_VERSION_1_CIQINDEX_ID			= 0x01;		//中标指数
	const int8_t MINOR_VERSION_0_CIQINDEX_ID			= 0x00;

	const int8_t MAJOR_VERSION_1_WINDINDEX_ID			= 0x01;		//万得指数
	const int8_t MINOR_VERSION_0_WINDINDEX_ID			= 0x00;


  /************************************************************************/
  /*                          Newest  VERSION                             */
  /************************************************************************/

#define MAJOR_VERSION_SYSTEM_ID       MAJOR_VERSION_1_SYSTEM_ID
#define MINOR_VERSION_SYSTEM_ID       MINOR_VERSION_0_SYSTEM_ID

#define MAJOR_VERSION_MONITOR_ID      MAJOR_VERSION_1_MONITOR_ID
#define MINOR_VERSION_MONITOR_ID      MINOR_VERSION_0_MONITOR_ID

#define MAJOR_VERSION_SSE_LEVEL1_ID   MAJOR_VERSION_1_SSE_LEVEL1_ID
#define MINOR_VERSION_SSE_LEVEL1_ID   MINOR_VERSION_0_SSE_LEVEL1_ID

#define MAJOR_VERSION_SZSE_LEVEL1_ID  MAJOR_VERSION_1_SZSE_LEVEL1_ID
#define MINOR_VERSION_SZSE_LEVEL1_ID  MINOR_VERSION_0_SZSE_LEVEL1_ID

#define MAJOR_VERSION_SSE_LEVEL2_ID   MAJOR_VERSION_1_SSE_LEVEL2_ID
#define MINOR_VERSION_SSE_LEVEL2_ID   MINOR_VERSION_1_SSE_LEVEL2_ID

#define MAJOR_VERSION_SZSE_LEVEL2_ID  MAJOR_VERSION_1_SZSE_LEVEL2_ID
#define MINOR_VERSION_SZSE_LEVEL2_ID  MINOR_VERSION_1_SZSE_LEVEL2_ID

#define MAJOR_VERSION_CFFE_LEVEL1_ID  MAJOR_VERSION_1_CFFE_LEVEL1_ID
#define MINOR_VERSION_CFFE_LEVEL1_ID  MINOR_VERSION_0_CFFE_LEVEL1_ID

#define MAJOR_VERSION_CFFE_LEVEL2_ID  MAJOR_VERSION_1_CFFE_LEVEL2_ID
#define MINOR_VERSION_CFFE_LEVEL2_ID  MINOR_VERSION_1_CFFE_LEVEL2_ID

#define MAJOR_VERSION_SHFE_LEVEL1_ID  MAJOR_VERSION_1_SHFE_LEVEL1_ID
#define MINOR_VERSION_SHFE_LEVEL1_ID  MINOR_VERSION_0_SHFE_LEVEL1_ID

#define MAJOR_VERSION_DCE_LEVEL1_ID   MAJOR_VERSION_1_DCE_LEVEL1_ID
#define MINOR_VERSION_DCE_LEVEL1_ID   MINOR_VERSION_0_DCE_LEVEL1_ID

#define MAJOR_VERSION_CZCE_LEVEL1_ID  MAJOR_VERSION_1_CZCE_LEVEL1_ID
#define MINOR_VERSION_CZCE_LEVEL1_ID  MINOR_VERSION_0_CZCE_LEVEL1_ID

#define MAJOR_VERSION_FX_ID           MAJOR_VERSION_1_FX_ID
#define MINOR_VERSION_FX_ID           MINOR_VERSION_0_FX_ID

#define MAJOR_VERSION_FTSE_ID         MAJOR_VERSION_1_FTSE_ID
#define MINOR_VERSION_FTSE_ID         MINOR_VERSION_0_FTSE_ID

#define MAJOR_VERSION_SWINDEX_ID					MAJOR_VERSION_1_SWINDEX_ID
#define MINOR_VERSION_SWINDEX_ID					MINOR_VERSION_0_SWINDEX_ID

#define MAJOR_VERSION_CSINDEX_ID					MAJOR_VERSION_1_CSINDEX_ID
#define MINOR_VERSION_CSINDEX_ID					MINOR_VERSION_0_CSINDEX_ID

#define MAJOR_VERSION_CIQINDEX_ID					MAJOR_VERSION_1_CIQINDEX_ID
#define MINOR_VERSION_CIQINDEX_ID					MINOR_VERSION_0_CIQINDEX_ID

#define MAJOR_VERSION_WINDINDEX_ID				MAJOR_VERSION_1_WINDINDEX_ID
#define MINOR_VERSION_WINDINDEX_ID				MINOR_VERSION_0_WINDINDEX_ID

  /************************************************************************/
  /*                       WMDF Protocol Processor Unique ID              */
  /************************************************************************/

#define ID_SHIFT_CODEC		  0
#define ID_SHIFT_MIN_VER    32
#define ID_SHIFT_MAJ_VER    40
#define ID_SHIFT_PID		    48

#define MAKE_UNIQUE_ID(pId,majVer,minVer,codec)								                        \
  ((((int64_t)pId) << ID_SHIFT_PID)		| (((int64_t)majVer)   << ID_SHIFT_MAJ_VER) |	  \
  (((int64_t)minVer)  << ID_SHIFT_MIN_VER) | (((int64_t)codec) << ID_SHIFT_CODEC))

#define SYSTEM_1_0_BINARY (int64_t)MAKE_UNIQUE_ID( PRODEUCT_SYSTEM_ID, MAJOR_VERSION_1_SYSTEM_ID,  MINOR_VERSION_0_SYSTEM_ID,  CT_BINARY)
#define MONITOR_1_0_BINARY (int64_t)MAKE_UNIQUE_ID( PRODEUCT_MONITOR_ID, MAJOR_VERSION_1_MONITOR_ID,  MINOR_VERSION_0_MONITOR_ID,  CT_BINARY)

#define SSEL1_1_0_FAST		(int64_t)MAKE_UNIQUE_ID( PRODUCT_SSE_LEVEL1_ID, MAJOR_VERSION_1_SSE_LEVEL1_ID,  MINOR_VERSION_0_SSE_LEVEL1_ID,  CT_FAST)
#define SSEL1_1_0_BINARY	(int64_t)MAKE_UNIQUE_ID( PRODUCT_SSE_LEVEL1_ID, MAJOR_VERSION_1_SSE_LEVEL1_ID,  MINOR_VERSION_0_SSE_LEVEL1_ID,  CT_BINARY)

#define	SSEL2_1_0_FAST		(int64_t)MAKE_UNIQUE_ID( PRODUCT_SSE_LEVEL2_ID, MAJOR_VERSION_1_SSE_LEVEL2_ID,  MINOR_VERSION_0_SSE_LEVEL2_ID,  CT_FAST)
#define SSEL2_1_0_BINARY	(int64_t)MAKE_UNIQUE_ID( PRODUCT_SSE_LEVEL2_ID, MAJOR_VERSION_1_SSE_LEVEL2_ID,  MINOR_VERSION_0_SSE_LEVEL2_ID,  CT_BINARY)

#define	SSEL2_1_1_FAST		(int64_t)MAKE_UNIQUE_ID( PRODUCT_SSE_LEVEL2_ID, MAJOR_VERSION_1_SSE_LEVEL2_ID,  MINOR_VERSION_1_SSE_LEVEL2_ID,  CT_FAST)
#define SSEL2_1_1_BINARY	(int64_t)MAKE_UNIQUE_ID( PRODUCT_SSE_LEVEL2_ID, MAJOR_VERSION_1_SSE_LEVEL2_ID,  MINOR_VERSION_1_SSE_LEVEL2_ID,  CT_BINARY)

#define SZSEL1_1_0_FAST		(int64_t)MAKE_UNIQUE_ID(PRODUCT_SZSE_LEVEL1_ID, MAJOR_VERSION_1_SZSE_LEVEL1_ID, MINOR_VERSION_0_SZSE_LEVEL1_ID, CT_FAST)
#define SZSEL1_1_0_BINARY	(int64_t)MAKE_UNIQUE_ID(PRODUCT_SZSE_LEVEL1_ID, MAJOR_VERSION_1_SZSE_LEVEL1_ID, MINOR_VERSION_0_SZSE_LEVEL1_ID, CT_BINARY)

#define SZSEL2_1_0_FAST		(int64_t)MAKE_UNIQUE_ID(PRODUCT_SZSE_LEVEL2_ID, MAJOR_VERSION_1_SZSE_LEVEL2_ID, MINOR_VERSION_0_SZSE_LEVEL2_ID, CT_FAST)
#define SZSEL2_1_0_BINARY	(int64_t)MAKE_UNIQUE_ID(PRODUCT_SZSE_LEVEL2_ID, MAJOR_VERSION_1_SZSE_LEVEL2_ID, MINOR_VERSION_0_SZSE_LEVEL2_ID, CT_BINARY)	

#define SZSEL2_1_1_FAST		(int64_t)MAKE_UNIQUE_ID(PRODUCT_SZSE_LEVEL2_ID, MAJOR_VERSION_1_SZSE_LEVEL2_ID, MINOR_VERSION_1_SZSE_LEVEL2_ID, CT_FAST)
#define SZSEL2_1_1_BINARY	(int64_t)MAKE_UNIQUE_ID(PRODUCT_SZSE_LEVEL2_ID, MAJOR_VERSION_1_SZSE_LEVEL2_ID, MINOR_VERSION_1_SZSE_LEVEL2_ID, CT_BINARY)	

#define CFFEL1_1_0_FAST		(int64_t)MAKE_UNIQUE_ID(PRODUCT_CFFE_LEVEL1_ID, MAJOR_VERSION_1_CFFE_LEVEL1_ID, MINOR_VERSION_0_CFFE_LEVEL1_ID, CT_FAST)
#define CFFEL1_1_0_BINARY	(int64_t)MAKE_UNIQUE_ID(PRODUCT_CFFE_LEVEL1_ID, MAJOR_VERSION_1_CFFE_LEVEL1_ID, MINOR_VERSION_0_CFFE_LEVEL1_ID, CT_BINARY)

#define CFFEL2_1_0_FAST		(int64_t)MAKE_UNIQUE_ID(PRODUCT_CFFE_LEVEL2_ID, MAJOR_VERSION_1_CFFE_LEVEL2_ID, MINOR_VERSION_0_CFFE_LEVEL2_ID, CT_FAST)
#define CFFEL2_1_0_BINARY	(int64_t)MAKE_UNIQUE_ID(PRODUCT_CFFE_LEVEL2_ID, MAJOR_VERSION_1_CFFE_LEVEL2_ID, MINOR_VERSION_0_CFFE_LEVEL2_ID, CT_BINARY)

#define CFFEL2_1_1_FAST		(int64_t)MAKE_UNIQUE_ID(PRODUCT_CFFE_LEVEL2_ID, MAJOR_VERSION_1_CFFE_LEVEL2_ID, MINOR_VERSION_1_CFFE_LEVEL2_ID, CT_FAST)
#define CFFEL2_1_1_BINARY	(int64_t)MAKE_UNIQUE_ID(PRODUCT_CFFE_LEVEL2_ID, MAJOR_VERSION_1_CFFE_LEVEL2_ID, MINOR_VERSION_1_CFFE_LEVEL2_ID, CT_BINARY)

#define SHFEL1_1_0_FAST		(int64_t)MAKE_UNIQUE_ID(PRODUCT_SHFE_LEVEL1_ID, MAJOR_VERSION_1_SHFE_LEVEL1_ID, MINOR_VERSION_0_SHFE_LEVEL1_ID, CT_FAST)
#define SHFEL1_1_0_BINARY	(int64_t)MAKE_UNIQUE_ID(PRODUCT_SHFE_LEVEL1_ID, MAJOR_VERSION_1_SHFE_LEVEL1_ID, MINOR_VERSION_0_SHFE_LEVEL1_ID, CT_BINARY)

#define DCEL1_1_0_FAST		(int64_t)MAKE_UNIQUE_ID(PRODUCT_DCE_LEVEL1_ID,	MAJOR_VERSION_1_DCE_LEVEL1_ID,  MINOR_VERSION_0_DCE_LEVEL1_ID,  CT_FAST)
#define DCEL1_1_0_BINARY	(int64_t)MAKE_UNIQUE_ID(PRODUCT_DCE_LEVEL1_ID,	MAJOR_VERSION_1_DCE_LEVEL1_ID,  MINOR_VERSION_0_DCE_LEVEL1_ID,  CT_BINARY)

#define CZCEL1_1_0_FAST		(int64_t)MAKE_UNIQUE_ID(PRODUCT_CZCE_LEVEL1_ID, MAJOR_VERSION_1_CZCE_LEVEL1_ID, MINOR_VERSION_0_CZCE_LEVEL1_ID, CT_FAST)
#define CZCEL1_1_0_BINARY	(int64_t)MAKE_UNIQUE_ID(PRODUCT_CZCE_LEVEL1_ID, MAJOR_VERSION_1_CZCE_LEVEL1_ID, MINOR_VERSION_0_CZCE_LEVEL1_ID, CT_BINARY)

#define FTSEL1_1_0_FAST		(int64_t)MAKE_UNIQUE_ID(PRODUCT_FTSE_LEVEL1_ID, MAJOR_VERSION_1_FTSE_ID, MINOR_VERSION_0_FTSE_ID, CT_FAST)
#define FTSEL1_1_0_BINARY	(int64_t)MAKE_UNIQUE_ID(PRODUCT_FTSE_LEVEL1_ID, MAJOR_VERSION_1_FTSE_ID, MINOR_VERSION_0_FTSE_ID, CT_BINARY)

#define SWINDEX_1_0_FAST		(int64_t)MAKE_UNIQUE_ID(PRODUCT_SWINDEX_LEVEL1_ID, MAJOR_VERSION_1_SWINDEX_ID, MINOR_VERSION_0_SWINDEX_ID, CT_FAST)
#define SWINDEX_1_0_BINARY	(int64_t)MAKE_UNIQUE_ID(PRODUCT_SWINDEX_LEVEL1_ID, MAJOR_VERSION_1_SWINDEX_ID, MINOR_VERSION_0_SWINDEX_ID, CT_BINARY)


#define CSINDEX_1_0_FAST		(int64_t)MAKE_UNIQUE_ID(PRODUCT_CSINDEX_LEVEL1_ID, MAJOR_VERSION_1_CSINDEX_ID, MINOR_VERSION_0_CSINDEX_ID, CT_FAST)
#define CSINDEX_1_0_BINARY	(int64_t)MAKE_UNIQUE_ID(PRODUCT_CSINDEX_LEVEL1_ID, MAJOR_VERSION_1_CSINDEX_ID, MINOR_VERSION_0_CSINDEX_ID, CT_BINARY)

#define CIQINDEX_1_0_FAST		(int64_t)MAKE_UNIQUE_ID(PRODUCT_CIQINDEX_LEVEL1_ID, MAJOR_VERSION_1_CIQINDEX_ID, MINOR_VERSION_0_CIQINDEX_ID, CT_FAST)
#define CIQINDEX_1_0_BINARY	(int64_t)MAKE_UNIQUE_ID(PRODUCT_CIQINDEX_LEVEL1_ID, MAJOR_VERSION_1_CIQINDEX_ID, MINOR_VERSION_0_CIQINDEX_ID, CT_BINARY)

#define WINDINDEX_1_0_FAST		(int64_t)MAKE_UNIQUE_ID(PRODUCT_WINDINDEX_LEVEL1_ID, MAJOR_VERSION_1_WINDINDEX_ID, MINOR_VERSION_0_WINDINDEX_ID, CT_FAST)
#define WINDINDEX_1_0_BINARY	(int64_t)MAKE_UNIQUE_ID(PRODUCT_WINDINDEX_LEVEL1_ID, MAJOR_VERSION_1_WINDINDEX_ID, MINOR_VERSION_0_WINDINDEX_ID, CT_BINARY)

/////////////////////////////////////////////////////////////////////////
// Return Values for DLL functions:  All API functions having a return
//   will return one of these codes
//////////////////////////////////////////////////////////////////////////
//to do define all return value
typedef enum eReturnValue
{
  INVALID_VALUE          = -1,	           //无效返回值
  SUCCESS                = 0,              //执行成功

  AUTHORITY_VALID        = 1,		           //权限有效
  AUTHORITY_INVALID      = 2, 		         //权限无效
  SEND_DATA_FAILD        =  3,             //推送数据失败
  USER_NOT_EXIST         = 4,              //用户名不存在
  PASSWORD_INCORRECT     = 5,              //密码错误
  IP_UNAUTHORIZED        = 6,              //IP未授权
  LOAD_API_FAILED        = 7,              //加载API失败
  UNLOAD_API_FAILED      = 8,              //卸载API失败
  LOGOUT_FAILED          = 9,              //登出失败
  NORMAL_DISCONNECT      = 10,             //正常断开
  ABNORMAL_DISCONNECT    = 11,             //异常断开
  CONNECTION_ID_INVALID  = 12,             //无效的连接ID(0-7)
  CONNECTION_OVERLOAD    = 13,             //连接数超过最大限制数（8）
  CONNECTION_FAILED      = 14,             //连接失败   
  CONNECTION_ID_INUSE    = 15,             //连接已经被使用 
  IDEXPIRED              = 15              //账号过期
  

} eReturnValue;


typedef enum MonitorEventId
{
  MEI_NOMAL_EVENT                   = 0,       //普通事件  
  MEI_CLIENT_CONNECTED              = 1,       //客户端连接
  MEI_CLIENT_DISCONNECTED           = 2,       //客户端断开连接
  MEI_CLIENT_NOTEXIST               = 3,       //客户端连接不存在 
  MEI_AUTHORITY_CONFIRMED           = 6,       //权限校验确认
  MEI_AUTHORITY_NONCONFORM          = 7,       //权限校验不通过
  MEI_SOURCE_CONNECTED              = 8,       //数据源适配连接成功 
  MEI_SOURCE_DISCONNECTED           = 9,       //数据源适配断开连接
  MEI_SOURCE_NOTAVAILABLE           = 10,      //数据源不存在
  MEI_SOURCE_LOGIN_FAILED           = 11,      //登录数据源失败
  MEI_SOURCE_LOGIN_SUCCEEDED        = 12,      //登录数据源成功
  MEI_FEED_SERVER_RESET             = 13,      //服务端重置  
  MEI_FEED_SERVER_PREMARKETOPEN     = 14,      //预开盘
  MEI_FEED_SERVER_MARKETCLOSE       = 15       //收盘
}MonitorEventId;

//编码器类型
typedef enum CodecType
{
  CT_UNKNOW       = 0,      //未知编码方式     
  CT_FAST         = 1,      //fast编码
  CT_BINARY       = 2,      //二进制编码
  CT_RAWDATA      = 3,      //交易所原始数据
  CT_FEEDHANDLER  = 4,      //feedhandler格式
  CT_DBFDATA      = 5			//DBF
} CodecType;

typedef enum MarketOperation
{
  MO_OPEN = 1,             //市场开盘
  MO_CLOSE = 2             //市场收盘
};

typedef enum LogoutType
{
  LT_SERVERNOTIFY         //服务程序通知客户端断开连接
} LogoutType;

////////////////////////////////////////////////////////////////
//  All API structures
////////////////////////////////////////////////////////////////

#pragma pack(push, 1)

//初始化API时所需参数
typedef struct OpenArgs
{
  uint8_t connection_id;         //0-31(最多支持32个连接)
  uint8_t user_name[32];         //用户名最长32字节
  uint8_t password[16];          //密码最长16字节
  uint8_t server_addr[15];       //服务器地址
  uint32_t server_port;          //服务器端口
  CodecType codec_type;          //协议编码器类型
  int32_t marktet_open_time;     //开市时间
  int32_t markt_close_time;      //收市时间
} OpenArgs;


typedef struct BackupSourceArgs
{
  uint8_t user_name[32];         //用户名最长32字节
  uint8_t password[16];          //密码最长16字节
  uint8_t server_addr[15];       //服务器地址
  uint32_t server_port;          //服务器端口
}BackupSourceArgs;

typedef struct OpenExArgs
{
  uint8_t connection_id;         //0-31(最多支持32个连接)
  uint8_t user_name[32];         //用户名最长32字节
  uint8_t password[16];          //密码最长16字节
  uint8_t server_addr[15];       //服务器地址
  uint32_t server_port;          //服务器端口
  CodecType codec_type;          //协议编码器类型
  int32_t marktet_open_time;     //开市时间
  int32_t markt_close_time;      //收市时间  
  int32_t retry_times;           //切换备源前重连次数
  int32_t timeout;               //连接超时时间（毫秒）
  int32_t backup_source_count;   //备源个数
  BackupSourceArgs* backup_sources;  //备源配置
}OpenExArgs;

typedef struct CloseArgs
{
  uint8_t connection_id;         //0-31(最多支持32个连接)
} CloseArgs;

typedef struct APIVersionArgs
{
  uint32_t major;               //API主版本号
  uint32_t minor;               //API副版本号
  uint32_t patch;               //API补丁版本号
} APIVersionArgs;

typedef struct ServerInfoArgs
{
  uint8_t connection_id;         //0-31(最多支持32个连接)
  uint8_t user_name[32];         //用户名最长32字节
  uint8_t password[16];          //密码最长16字节
  uint8_t server_addr[15];       //服务器地址
  uint32_t server_port;          //服务器端口
};

typedef struct FilterArgs
{
  uint8_t connection_id;           //0-31(最多支持32个连接)
  uint8_t filter_masks[3000];      //windcode过滤字符串,以逗号分隔，如600000.SH,600001.SH
}FilterArgs;

typedef struct MdfRecord
{
  time_stamp_t local_time;      // 本地时间
	uint32_t packet_num;          // 包号
  int16_t product_id;           // 产品ID
  int16_t message_id;           // 消息ID
  int8_t major_version;         // 主版本
  int8_t minor_version;         // 副版本号
  void* header;                 // 产品包头
  int32_t record_count;         // 数据记录条数
  void* body;                   // 数据记录
} MdfRecord;

typedef struct RecordArgs
{
  uint8_t         connection_id;         //0-31(最多支持32个连接)
  MdfRecord*      record;                //数据记录 
  void*           state;                 //回调对象句柄
} RecordArgs;


typedef struct EventArgs
{
  uint8_t       connection_id;            //0-31(最多支持32个连接)
  int8_t        level;                    //事件级别 FATAL=0, ERROR=1,WARN=2,INFO=3,DEBUG=4
  date_time_t   date_time;                //事件发生的时间
  uint8_t       at_where[60];             //监控点
  int32_t       event_id;                 //事件ID
  uint8_t       what_happened[150];       //事件内容，如 登录 “连接失败“等 
  void*         state;                    //回调对象句柄
} EventArgs;

typedef void (*RecordCallback)(RecordArgs* record);

typedef void (*FeedEventCallback)(EventArgs* args);

typedef struct CallbackArgs
{
  uint8_t connection_id;               //0-31(最多支持32个连接)
  RecordCallback record_callback;      //数据回调
  FeedEventCallback event_callback;    //事件回调
  void* state;
}CallbackArgs;

#pragma pack(pop)

////////////////////////////////////////////////////////////////
//  API function prototypes
////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////
//  version 1.0 
////////////////////////////////////////////////////////////////
WMDF_API eReturnValue wmdf_initialize();
WMDF_API eReturnValue wmdf_finalize();
WMDF_API eReturnValue wmdf_open_connection(OpenArgs* args);
WMDF_API eReturnValue wmdf_open_connection_ex(OpenExArgs* args);
WMDF_API eReturnValue wmdf_close_connection(CloseArgs* args);
WMDF_API eReturnValue wmdf_set_filter_mask(FilterArgs* args);
WMDF_API eReturnValue wmdf_get_api_version(APIVersionArgs* args);
WMDF_API eReturnValue wmdf_get_server_info(ServerInfoArgs* args);
WMDF_API eReturnValue wmdf_set_callback(CallbackArgs* args);
WMDF_API eReturnValue wmdf_get_record(RecordArgs* args);
WMDF_API eReturnValue wmdf_release_record(RecordArgs* args);

//#define release(recode) {wmdf_release_product_message(record);record=NULL;}


#ifdef __cplusplus
}
#endif

#endif
