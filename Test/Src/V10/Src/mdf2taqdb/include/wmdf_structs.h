#ifndef WMDF_STRUCTS_H
#define WMDF_STRUCTS_H

#include "wmdf_api.h"

/************************************************************************/
/*                         基本传输消息                           */
/************************************************************************/
#pragma pack(push, 1)

// 基本传输消息头。

typedef struct SystemMessageHeader
{
  int8_t major_version;
  int8_t minor_version;
}SystemMessageHeader;

//
typedef struct HeartBeat
{
  int8_t place_holder;
}HeartBeat;

// 订阅产品的信息
typedef struct ProductInfo
{
  int16_t product_id;
  int8_t major_version;
  int8_t minor_version;
  int8_t codec;
}ProductInfo;

// 登录请求消息。由客户端发送至服务端
typedef struct LoginRequest
{
	// 用户名和密码
	uint8_t user_name[32];         //用户名最长32字节
	uint8_t password[16];          //密码最长16字节
  int16_t product_count;
  ProductInfo* product;
}LoginRequest;

typedef struct ServerInfo
{
  uint8_t server_addr[15];
  uint32_t server_port;
}ServerInfo;

typedef struct MessageInfo
{
  int16_t message_id;
  int8_t message_status;  //1 valid; 2 invalid;
}MessageInfo;

typedef struct ProductStatus
{
  int16_t product_id;
  int8_t product_status;  //1 valid; 2 invalid;
  int8_t major_version;
  int8_t minor_version;
  int8_t codec;
  int32_t message_count;  
  MessageInfo* message_info;
}ProductStatus;

// 登录响应消息。由服务端响应客户端
typedef struct LoginResponse
{
	int8_t ack_type;
  uint32_t user_id;    //valid when ack_type = succeed;
  int32_t server_count;
  ServerInfo* serverinfo;
  int32_t product_count;
  ProductStatus* product_status;
}LoginResponse;

// 登出通知消息。用于客户端登出WMDF系统
typedef struct Logout
{
  int8_t logout_type;   // define logout type. 1 is server notify to logout;
}Logout;

//市场预开盘和收盘时发送
typedef struct MarketDateSignal
{
  int8_t market_operation;  //1= 开盘 2 = 收盘
  date_t market_date;
}MarketDateSignal;

/************************************************************************/
/*                         监控消息                               */
/************************************************************************/
// 监控消息头
typedef struct MonitorMessageHeader
{
  int8_t place_holder;
}MonitorMessageHeader;

//发送数据统计
typedef struct TransferDataStat
{
  int16_t product_id;                 //产品ID
  double  receive_data_flow;          //接收到的字节流量(KB/S)
  double  receive_pack_flow;          //接收到的包流量
  uint32_t receive_packet_count;       //接收到的包数
  uint32_t pre_receive_packet_count;   //上次统计时receive包数
  uint64_t receive_data_size;          //原始大小(KB)
  uint64_t pre_receive_data_size;      //上次统计时编码后大小(KB)

  double  send_data_flow;             //发送字节流量(KB/S)
  double  send_pack_flow;             //发送包流量
  uint32_t send_packet_count;          //已发送包数
  uint32_t pre_send_packet_count;      //上次统计时已发送包数
  uint32_t send_packet_delta;          //前次统计到现在发送的包数 

  uint64_t encoded_size;               //编码后大小(KB)
  uint64_t pre_encoded_size;           //上次统计时编码后大小(KB)  

  uint64_t binary_struct_size;         //二进制结构体大小(KB)  
  double compression_ratio;           //压缩比(%)
  uint32_t total_msg_delay;            //平均延迟(ms)
  uint32_t pre_total_msg_delay;        //上次消息的延迟
  uint32_t msg_avg_delay;              //平均延迟
  uint32_t delayed_msg_delta;          //前次统计延迟到现在发生的延迟包数
  uint32_t delayed_msg_count;          //延迟包的总数
  uint32_t pre_delayed_msg_count;      //上次延迟包的总数
}TransferDataStat;

typedef struct ClientStatus
{
  int32_t socket_id;
  uint8_t user_name[32];         //用户名最长32字节
	uint8_t client_ip[15];         //客户IP 
  time_stamp_t duration;         //持续时间
	double send_data_flow;         //传输速率(KB/S)
  int64_t send_bytes;            //总的发送字节数
  int64_t pre_send_bytes;        //前一次发送字节数
  int32_t msg_in_queue;          //缓存队列中的数据包个数
	int32_t product_count;
	ProductInfo* product_infos;
}ClientStatus;

/************************************************************************/
/*                         行情数据消息                           */
/************************************************************************/

// 行情数据消息头
typedef struct MarketMessageHeader
{
	int8_t place_holder;
}MarketMessageHeader;

// 沪深L1市场快照-----------------------Done
typedef struct L1MarketSnapshot
{
	char wind_code[16];
	date_t trade_date;
	time_stamp_t trade_time;

	double new_price;
	double prev_close;
	double today_open;
	double today_high;
	double today_low;

	int64_t volume;
	double total_amount;	
	//int64_t delta_volume;
	//double delta_amount;

	double bid_price1;
	double bid_price2;
	double bid_price3;
	double bid_price4;
	double bid_price5;
	double ask_price1;
	double ask_price2;
	double ask_price3;
	double ask_price4;
	double ask_price5;
	int32_t ask_volume1;
	int32_t ask_volume2;
	int32_t ask_volume3;
	int32_t ask_volume4;
	int32_t ask_volume5;
	int32_t bid_volume1;
	int32_t bid_volume2;
	int32_t bid_volume3;
	int32_t bid_volume4;
	int32_t bid_volume5;	
}L1MarketSnapshot;

// L2市场快照
typedef struct L2MarketSnapshot
{
	char wind_code[16];             
	date_t trade_date;              //交易日期，1
  time_stamp_t trade_time;        //交易时间，2  

  double new_price;               //最新价，3
  double prev_close;              //前收价，4
  double today_open;              //开盘价，5
  double today_high;              //最高价，6
  double today_low;               //最低价，7
  
  int64_t volume;                 //总成交量，8
  double total_amount;            //总成交额，59

  double bid_price1;              //买1价，11
	double bid_price2;              //买2价，12            
	double bid_price3;              //买3价，13
	double bid_price4;              //买4价，14
	double bid_price5;              //买5价，15
	double bid_price6;              //买6价，16
	double bid_price7;              //买7价，17
	double bid_price8;              //买8价，18
	double bid_price9;              //买9价，19
	double bid_price10;             //买10价，20
	double ask_price1;              //卖1价，21
	double ask_price2;              //卖2价，22
	double ask_price3;              //卖3价，23
	double ask_price4;              //卖4价，24
	double ask_price5;              //卖5价，25
	double ask_price6;              //卖6价，26
	double ask_price7;              //卖7价，27              
	double ask_price8;              //卖8价，28
	double ask_price9;              //卖9价，29
	double ask_price10;             //卖10价，30
  int32_t bid_volume1;           //买1量，31
  int32_t bid_volume2;           //买2量，32
  int32_t bid_volume3;           //买3量，33
  int32_t bid_volume4;           //买4量，34
  int32_t bid_volume5;           //买5量，35
  int32_t bid_volume6;           //买6量，36
  int32_t bid_volume7;           //买7量，37
  int32_t bid_volume8;           //买8量，38
  int32_t bid_volume9;           //买9量，39
  int32_t bid_volume10;          //买10量，40
	int32_t ask_volume1;           //卖1量，41
	int32_t ask_volume2;           //卖2量，42
	int32_t ask_volume3;           //卖3量，43
	int32_t ask_volume4;           //卖4量，44
	int32_t ask_volume5;           //卖5量，45
	int32_t ask_volume6;           //卖6量，46
	int32_t ask_volume7;           //卖7量，47
	int32_t ask_volume8;           //卖8量，48
	int32_t ask_volume9;           //卖9量，49
	int32_t ask_volume10;          //卖10量，50

	
  int64_t transactions_count;     //交易总笔数, 71
  double weighted_average_best_bid;	//加权平均委买价格，86，沪L2
  double weighted_average_best_ask; //加权平均委卖价格，87，沪L2
  int64_t l2_total_bid_qty;       //委托买入总量，340，沪L2
  int64_t l2_total_offer_qty;     //委卖卖出总量，341，沪L2
}L2MarketSnapshot;


// 逐笔成交
typedef struct L2Transaction
{
	char wind_code[16];         //证券代码
	time_stamp_t trade_time;    //交易时间
	int32_t set_no;             //证券集代号 szse  
	int32_t rec_no;             //成交序号
	int32_t buyOrderRecNo;      //买方委托索引 szse
	int32_t sellOrderRecNo;     //卖方委托索引 szse
	double price;               //成交价格
	int32_t trade_qty;          //成交数量
	int8_t order_kind;          //成交类别 szse
	int8_t function_code;       //成交代码 szse
  int32_t trade_channel;        //X sse
  int64_t trade_money;          //X sse
}L2Transaction;


// L2委托队列级别量
typedef struct L2OrderQueueItem
{
	int32_t order_queue_operator;           //0-Absent, 1-Add,2-Update,3-Delete
	int32_t order_queue_operator_entry_id;  //Start from 0, No Meaning if
	int32_t order_qty;
}L2OrderQueueItem;

// L2委托队列级别
typedef struct L2OrderQueuePriceLevel
{
	int32_t price_level_operator;     //0-Absent,1-Add,2-Update,3-Delete
	double price;
	int32_t orders_total;             //当前价位上总委托笔数
	int32_t orders_count;             //此次发生变化的委托笔数
	L2OrderQueueItem* order_queue_items;     // size = orders_count
}L2OrderQueuePriceLevel;

// L2委托队列
typedef struct L2OrderQueue
{
	char wind_code[16];
	time_stamp_t date_time_stamp;             // 163025
	uint8_t side;                             // 1 = Bid Queue, 2 = Ask Queue, other: Invalid
	int32_t image_status;                     // 1 = FullImage, 2 = Update, other: Invalid
	int32_t price_level_count;                // Number of PriceLevelItems, should = PriceLevel_items.size();
	L2OrderQueuePriceLevel* order_queue_levels;
}L2OrderQueue;

//上交所市场概况
typedef struct MarketOverView
{
	char wind_code[16];								
	time_stamp_t date_time_stamp;							//163025
	int32_t origtime;													//更新时间
	int32_t origdate;													//更新日期
}MarketOverView;

//上交所虚拟竞价
typedef struct VirtualAuctionPrice
{
	char wind_code[16];
	time_stamp_t date_time_stamp;							//163025
	double price;														//虚拟开盘参考价格
	int32_t virtual_auction_qty;							//虚拟匹配量
	int32_t leave_qty;												//虚拟未匹配量
	uint8_t side;															//1 = bid, 2 = offer
}VirtualAuctionPrice;



// 深L2逐笔委托
typedef struct SZSEL2Order
{
	char wind_code[16];           //证券代码
	int32_t set_no;               //X
	int32_t rec_no;               //委托索引
	double order_price;           //价格
	int32_t order_qty;            //委托数量,  股
	time_stamp_t order_time;      //委托时间,  HHMMSSXX
	char order_kind;              //委托类别,  1=委买, 2=委卖, 3=撤单
	char function_code;           //成交代码
}SZSEL2Order;


// 中金所L2市场快照
typedef struct CFEL2MarketSnapshot
{
	char wind_code[16];                 // 合约代码,如:IF1106
	date_t trade_date;                  // 交易日期, yyyymmdd			1
	time_stamp_t trade_time;            // 最后修改时间, hhMMss		2
  //int UpdateMillisec;                 // 最后修改毫秒, mmm			X
	double new_price;                   // 最新价					3
	double prev_close;                  // 昨收盘					4
	double today_open;                  // 今开盘					5
	double today_high;                  // 今最高价					6
	double today_low;                   // 今最低价					7
	
	double bid_price1;                  // 申买价一				11
	double bid_price2;                  // 申买价二				12
	double bid_price3;                  // 申买价三				13
	double bid_price4;                  // 申买价四				14
	double bid_price5;                  // 申买价五				15
	double ask_price1;                  // 申卖价一				21
	double ask_price2;                  // 申卖价二				22
	double ask_price3;                  // 申卖价三				23
	double ask_price4;                  // 申卖价四				24
	double ask_price5;                  // 申卖价五				25
	int32_t ask_volume1;                // 申卖量一				41
	int32_t ask_volume2;                // 申卖量二				42
  int32_t ask_volume3;                // 申卖量三				43
	int32_t ask_volume4;                // 申买量四				34
	int32_t ask_volume5;                // 申卖量五				45
	int32_t bid_volume1;                // 申买量一				31
	int32_t bid_volume2;                // 申买量二				32
	int32_t bid_volume3;                // 申买量三				33
	int32_t bid_volume4;                // 申卖量四				44
	int32_t bid_volume5;                // 申买量五				35

  int64_t volume;                     // 总成交量					8
	double total_amount;                    // 总成交金额				59
	double prev_settle;               // 昨结算					75
	int64_t position;                 // 最新持仓量				76
	int64_t prev_position;            // 昨持仓量					77
	double high_limit;                  // 涨停板价					211	
	double low_limit;                   // 跌停板价					212
}CFEL2MarketSnapshot;

// 期货L1市场快照
typedef struct FuturesMarketSnapshot
{
  char wind_code[16];           //合约代码
  date_t trade_date;            //交易日  yyyymmdd  1
  time_stamp_t trade_time;      //最后修改时间,hhmmss 2

  double prev_close;            //昨收盘  4
  double today_open;            //今开盘  5
  double today_high;            //最高价  6
  double today_low;             //最低价  7
  double new_price;             //最新价  3

  int64_t volume;               //总成交量  8
  double amount;                //总成交金额  59    郑商所无

  double bid_price1;            //申买价一  11
  double ask_price1;            //申卖价一  21
  int32_t ask_volume1;          //申卖量一  41
  int32_t bid_volume1;          //申买量一  31

 
  double settle;                //今结算  74        郑商所有
  double prev_settle;           //昨结算  75        中金所、郑商所有
  int64_t position;             //最新持仓量  76    中金所、郑商所有
  int64_t prev_position;        //昨持仓量    77    中金所有
  double average_price;         //均价  79          郑商所有
  double high_limit;            //涨停板  211
  double low_limit;             //跌停板  212
}FuturesMarketSnapshot;

// 港股L2市场快照
typedef struct HKEXL2MarketSnapshot
{
	char wind_code[16];
	date_t trade_date;
	time_stamp_t trade_time;
	double new_price;
	double prev_close;
	double today_open;
	double today_high;
	double today_low;
	int64_t volume;
	uint32_t delta_volume;
	double bid_price1;
	double bid_price2;
	double bid_price3;
	double bid_price4;
	double bid_price5;
	double ask_price1;
	double ask_price2;
	double ask_price3;
	double ask_price4;
	double ask_price5;
	int32_t ask_volume1;
	int32_t ask_volume2;
	int32_t ask_volume3;
	int32_t ask_volume4;
	int32_t ask_volume5;
	int32_t bid_volume1;
	int32_t bid_volume2;
	int32_t bid_volume3;
	int32_t bid_volume4;
	int32_t bid_volume5;
	double best_bid;
	double best_ask;
	int32_t bid_orders1;
	int32_t bid_orders2;
	int32_t bid_orders3;
	int32_t bid_orders4;
	int32_t bid_orders5;
	int32_t ask_orders1;
	int32_t ask_orders2;
	int32_t ask_orders3;
	int32_t ask_orders4;
	int32_t ask_orders5;
	int32_t transactions_count;
	double settle;
	double prev_settle;
	double high_limit;
	double low_limit;
}HKEXL2MarketSnapshot;




// 外汇市场快照
typedef struct FXMarketSnapshot
{
	char wind_code[16];
	date_t trade_date;
	time_stamp_t trade_time;
	double new_price;
	double prev_close;
	double today_open;
	double today_high;
	double today_low;
	double bid_price1;
	double ask_price1;
}FXMarketSnapshot;

#pragma pack(pop)



#endif