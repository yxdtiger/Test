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
  int8_t        major_version;          //主版本号
  int8_t        minor_version;          //副版本号
}SystemMessageHeader;

//心跳包
typedef struct HeartBeat
{
  int8_t        place_holder;           //心跳占位
}HeartBeat;

// 订阅产品的信息
typedef struct ProductInfo
{
  int16_t       product_id;           //产品ID
  int8_t        major_version;        //主版本号
  int8_t        minor_version;        //副版本号
  int8_t        codec;                //编码器类型 UNKNOW = 0,FAST = 1,BINARY = 2, RAWDATA = 3, FEEDHANDLER=4
}ProductInfo;

//登录请求消息，由客户端发送至服务端
typedef struct LoginRequest
{
	uint8_t       user_name[32];         //用户名最长32字节
	uint8_t       password[16];          //密码最长16字节
  int16_t       product_count;         //订阅产品个数
  ProductInfo*  product;               //产品具体描述
}LoginRequest;

//服务器信息
typedef struct ServerInfo
{
  uint8_t       server_addr[15];       //IP地址
  uint32_t      server_port;           //服务端口
}ServerInfo;

//消息状态信息
typedef struct MessageInfo
{
  int16_t       message_id;           //消息ID
  int8_t        message_status;       //订阅是否成功  1： valid; 2： invalid;
}MessageInfo;

//产品订阅状态信息
typedef struct ProductStatus
{
  int16_t      product_id;            //消息ID
  int8_t       product_status;        //产品订阅状态 1 valid; 2 invalid;
  int8_t       major_version;         //主版本号
  int8_t       minor_version;         //副版本号
  int8_t       codec;                 //编码器类型 UNKNOW = 0,FAST = 1,BINARY = 2, RAWDATA = 3, FEEDHANDLER=4
  int32_t      message_count;         //订阅消息个数
  MessageInfo* message_info;          //消息状态
}ProductStatus;

// 登录响应消息,由服务端响应客户端
typedef struct LoginResponse
{
	int8_t         ack_type;            //订阅响应类型  1 valid; 2 invalid;
  uint32_t       user_id;             //用户ID
  int32_t        server_count;        //服务器个数
  ServerInfo*    serverinfo;          //服务器信息
  int32_t        product_count;       //订阅产品个数
  ProductStatus* product_status;      //产品状态信息
}LoginResponse;

// 登出通知消息, 用于客户端登出WMDF系统
typedef struct Logout
{
  int8_t      logout_type;            // define logout type. 1 is server notify to logout;
}Logout;

//市场预开盘和收盘时发送
typedef struct MarketDateSignal
{
  int8_t      market_operation;        //1= 开盘 2 = 收盘
  date_t      market_date;             //交易日期
  int16_t     product_id;              //产品id
}MarketDateSignal;

/************************************************************************/
/*                         监控消息                               */
/************************************************************************/
// 监控消息头
typedef struct MonitorMessageHeader
{
  int8_t      place_holder;               //占位符
}MonitorMessageHeader;

//发送数据统计
typedef struct TransferDataStat
{
  int16_t     product_id;                 //产品ID
  uint8_t     source_ip[15];              //服务ip
  int32_t     source_port;                //服务端口
  uint8_t     source_status;              //服务器状态

  double      receive_data_flow;          //接收到的字节流量(KB/S)
  double      receive_pack_flow;          //接收到的包流量
  uint32_t    receive_packet_count;       //接收到的包数
  uint32_t    pre_receive_packet_count;   //上次统计时receive包数
  uint64_t    receive_data_size;          //原始大小(KB)
  uint64_t    pre_receive_data_size;      //上次统计时编码后大小(KB)

  double      send_data_flow;             //发送字节流量(KB/S)
  double      send_pack_flow;             //发送包流量
  uint32_t    send_packet_count;          //已发送包数
  uint32_t    pre_send_packet_count;      //上次统计时已发送包数
  uint32_t    send_packet_delta;          //前次统计到现在发送的包数

  uint64_t    encoded_size;               //编码后大小(KB)
  uint64_t    pre_encoded_size;           //上次统计时编码后大小(KB)

  uint64_t    binary_struct_size;         //二进制结构体大小(KB)
  double      compression_ratio;          //压缩比(%)
  uint32_t    total_msg_delay;            //平均延迟(ms)
  uint32_t    pre_total_msg_delay;        //上次消息的延迟
  uint32_t    msg_avg_delay;              //平均延迟
  uint32_t    delayed_msg_delta;          //前次统计延迟到现在发生的延迟包数
  uint32_t    delayed_msg_count;          //延迟包的总数
  uint32_t    pre_delayed_msg_count;      //上次延迟包的总数
}TransferDataStat;

typedef struct ClientStatus
{
  int32_t        socket_id;               //socketid
  uint8_t        user_name[32];           //用户名最长32字节
	uint8_t        client_ip[15];           //客户IP
  time_stamp_t   duration;                //持续时间
	double         send_data_flow;          //传输速率(KB/S)
  int64_t        send_bytes;              //总的发送字节数
  int64_t        pre_send_bytes;          //前一次发送字节数
  int32_t        msg_in_queue;            //缓存队列中的数据包个数
	int32_t        product_count;           //订阅产品个数
	ProductInfo*   product_infos;           //订阅产品信息
}ClientStatus;

typedef struct MonitorEvent
{
  int8_t        level;                    //事件级别 FATAL=0, ERROR=1,WARN=2,INFO=3,DEBUG=4
  date_time_t   date_time;                //事件发生的时间
  uint8_t       at_where[60];             //监控点
  int32_t       event_id;                 //事件ID
  uint8_t       what_happened[150];       //事件内容，如 登录 “连接失败“等
}MonitorEvent;


/************************************************************************/
/*                         行情数据消息                           */
/************************************************************************/

// 行情数据消息头
typedef struct MarketMessageHeader
{
	int8_t place_holder;
}MarketMessageHeader;

// 沪深L1市场快照
typedef struct L1MarketSnapshot
{
	char          wind_code[16];           //证券代码
	time_stamp_t  data_time_stamp;         //时间戳
	double        pre_close;               //前收价
	double        open_price;              //当日开盘价
	double        high_price;              //当日最高价
	double        low_price;               //当日最低价
  double        new_price;               //最新价
	int64_t       total_volume;            //成交量
	double        total_amount;	           //成交额
	double        bid_price1;              //买1价
	double        bid_price2;              //买2价
	double        bid_price3;              //买3价
	double        bid_price4;              //买4价
	double        bid_price5;              //买5价
	double        ask_price1;              //卖1价
	double        ask_price2;              //卖2价
	double        ask_price3;              //卖3价
	double        ask_price4;              //卖4价
	double        ask_price5;              //卖5价
	int32_t       ask_size1;               //卖1量
	int32_t       ask_size2;               //卖2量
	int32_t       ask_size3;               //卖3量
	int32_t       ask_size4;               //卖4量
	int32_t       ask_size5;               //卖5量
	int32_t       bid_size1;               //买1量
	int32_t       bid_size2;               //买2量
	int32_t       bid_size3;               //买3量
	int32_t       bid_size4;               //买4量
	int32_t       bid_size5;               //买5量
}L1MarketSnapshot;

// L2市场快照
typedef struct L2MarketSnapshot
{
	char          wind_code[16];
  time_stamp_t  data_time_stamp;            //交易时间，2
  double        pre_close;                  //前收价，4
  double        open_price;                 //开盘价，5
  double        high_price;                 //最高价，6
  double        low_price;                  //最低价，7
  double        new_price;                  //最新价，3
  int64_t       total_volume;               //总成交量，8
  double        total_amount;               //总成交额，59
  double        bid_price1;                 //买1价，11
	double        bid_price2;                 //买2价，12
	double        bid_price3;                 //买3价，13
	double        bid_price4;                 //买4价，14
	double        bid_price5;                 //买5价，15
	double        bid_price6;                 //买6价，16
	double        bid_price7;                 //买7价，17
	double        bid_price8;                 //买8价，18
	double        bid_price9;                 //买9价，19
	double        bid_price10;                //买10价，20
	double        ask_price1;                 //卖1价，21
	double        ask_price2;                 //卖2价，22
	double        ask_price3;                 //卖3价，23
	double        ask_price4;                 //卖4价，24
	double        ask_price5;                 //卖5价，25
	double        ask_price6;                 //卖6价，26
	double        ask_price7;                 //卖7价，27
	double        ask_price8;                 //卖8价，28
	double        ask_price9;                 //卖9价，29
	double        ask_price10;                //卖10价，30
  int32_t       bid_size1;                  //买1量，31
  int32_t       bid_size2;                  //买2量，32
  int32_t       bid_size3;                  //买3量，33
  int32_t       bid_size4;                  //买4量，34
  int32_t       bid_size5;                  //买5量，35
  int32_t       bid_size6;                  //买6量，36
  int32_t       bid_size7;                  //买7量，37
  int32_t       bid_size8;                  //买8量，38
  int32_t       bid_size9;                  //买9量，39
  int32_t       bid_size10;                 //买10量，40
	int32_t       ask_size1;                  //卖1量，41
	int32_t       ask_size2;                  //卖2量，42
	int32_t       ask_size3;                  //卖3量，43
	int32_t       ask_size4;                  //卖4量，44
	int32_t       ask_size5;                  //卖5量，45
	int32_t       ask_size6;                  //卖6量，46
	int32_t       ask_size7;                  //卖7量，47
	int32_t       ask_size8;                  //卖8量，48
	int32_t       ask_size9;                  //卖9量，49
	int32_t       ask_size10;                 //卖10量，50
  int64_t       transactions_count;         //交易总笔数, 71
  double        weighted_average_bid_price; //加权平均委买价格，86，沪L2
  double        weighted_average_ask_price; //加权平均委卖价格，87，沪L2
  int64_t       total_bid_size;             //委托买入总量，340，沪L2
  int64_t       total_ask_size;             //委卖卖出总量，341，沪L2
  double        peratio1;                   // SZSE 股票：价格/上年每股利润    v1.1
                                            // SZSE 债券：每百元应计利息
  double        peratio2;                   // SZSE     股票：价格/本年每股利润 v1.1
                                            // SZSE/SSE 债券：到期收益率
                                            // SZSE/SSE 基金：每百份的IOPV或净值
                                            // SZSE     权证：溢价率
  int8_t        trade_status;               // 交易状态 1=正常交易 2=停牌      v1.1
}L2MarketSnapshot;


// 逐笔成交
typedef struct L2Transaction
{
	char          wind_code[16];              //证券代码
	time_stamp_t  trade_time;                 //交易时间
	int32_t       volume;                     //成交数量
  double        price;                      //成交价格
  double        amount;                     //成交金额 sse
  int32_t       set_no;                     //证券集代号 szse
  int32_t       rec_no;                     //成交序号
  int32_t       buyOrderRecNo;              //买方委托索引 szse
  int32_t       sellOrderRecNo;             //卖方委托索引 szse
	int8_t        order_kind;                 //成交类别 szse
	int8_t        function_code;              //成交代码 szse
}L2Transaction;


// L2委托队列级别量
typedef struct L2OrderQueueItem
{
	int32_t       operator_type;            //委托单操作0-Absent, 1-Add,2-Update,3-Delete
	int32_t       entry_id;                 //委托单在队列中的位置Start from 0, No Meaning if
	int32_t       size;                     //委托量
}L2OrderQueueItem;

// L2委托队列级别
typedef struct L2OrderQueuePriceLevel
{
	int32_t           operator_type;            //价格操作符0-Absent,1-Add,2-Update,3-Delete
	double            order_price;              //价格
	int32_t           total_number;             //当前价位上总委托笔数
	int32_t           orders_number;            //此次发生变化的委托笔数
	L2OrderQueueItem* order_items;              //order_items中Item数等于orders_number
}L2OrderQueuePriceLevel;

// L2委托队列
typedef struct L2OrderQueue
{
	char                    wind_code[16];        //证券代码
	time_stamp_t            data_time_stamp;      //时间戳
	uint8_t                 order_side;           //委托方向 1 = Bid Queue, 2 = Ask Queue, other: Invalid
	int32_t                 image_status;         //影像类型 1 = FullImage, 2 = Update, other: Invalid
	int32_t                 price_level_count;    //价格个数 Number of PriceLevelItems, should = PriceLevel_items.size();
	L2OrderQueuePriceLevel* price_levels;         //委托价格上的队列详细信息
}L2OrderQueue;


// 深L2逐笔委托
typedef struct SZSEL2Order
{
	char         wind_code[16];                 //证券代码
  time_stamp_t order_time;                    //委托时间
	int32_t      set_no;                        //证券集代码
	int32_t      rec_no;                        //委托索引
	int32_t      size;                          //委托数量,  股
  double       price;                         //价格
	char         order_kind;                    //委托类别,  1=委买, 2=委卖, 3=撤单
	char         function_code;                 //成交代码
}SZSEL2Order;

// 中金所L2市场快照
typedef struct CFEL2MarketSnapshot
{
	char          wind_code[16];               // 合约代码,如:IF1106
	date_t        trade_date;                  // 交易日期, yyyymmdd			1
	time_stamp_t  data_time_stamp;             // 最后修改时间, hhMMss		2
	double        pre_close;                   // 昨收盘					4
	double        open_price;                  // 今开盘					5
	double        high_price;                  // 今最高价					6
	double        low_price;                   // 今最低价					7
  double        new_price;                   // 最新价					3
  int64_t       total_volume;                // 总成交量					8
  double        total_amount;                // 总成交金额				59
	double        bid_price1;                  // 申买价一				11
	double        bid_price2;                  // 申买价二				12
	double        bid_price3;                  // 申买价三				13
	double        bid_price4;                  // 申买价四				14
	double        bid_price5;                  // 申买价五				15
	double        ask_price1;                  // 申卖价一				21
	double        ask_price2;                  // 申卖价二				22
	double        ask_price3;                  // 申卖价三				23
	double        ask_price4;                  // 申卖价四				24
	double        ask_price5;                  // 申卖价五				25
	int32_t       ask_size1;                   // 申卖量一				41
	int32_t       ask_size2;                   // 申卖量二				42
  int32_t       ask_size3;                   // 申卖量三				43
	int32_t       ask_size4;                   // 申买量四				34
	int32_t       ask_size5;                   // 申卖量五				45
	int32_t       bid_size1;                   // 申买量一				31
	int32_t       bid_size2;                   // 申买量二				32
	int32_t       bid_size3;                   // 申买量三				33
	int32_t       bid_size4;                   // 申卖量四				44
	int32_t       bid_size5;                   // 申买量五				35
	double        pre_settle;                  // 昨结算					75
	int64_t       position;                    // 最新持仓量				76
	int64_t       pre_position;                // 昨持仓量					77
	double        high_limit;                  // 涨停板价					211
	double        low_limit;                   // 跌停板价					212
  double        settle;                      // 今结算
}CFEL2MarketSnapshot;

// 期货L1市场快照
typedef struct FuturesMarketSnapshot
{
  char          wind_code[16];              //合约代码
  date_t        trade_date;                 //交易日    1
  time_stamp_t  data_time_stamp;            //最后修改时间 2
  double        pre_close;                  //昨收盘  4
  double        open_price;                 //今开盘  5
  double        high_price;                 //最高价  6
  double        low_price;                  //最低价  7
  double        new_price;                  //最新价  3
  int64_t       total_volume;               //总成交量  8
  double        total_amount;               //总成交金额  59    郑商所无
  double        bid_price1;                 //申买价一  11
  double        ask_price1;                 //申卖价一  21
  int32_t       ask_size1;                  //申卖量一  41
  int32_t       bid_size1;                  //申买量一  31
  double        pre_settle;                 //昨结算  75        中金所、郑商所有
  int64_t       pre_position;               //昨持仓量    77
  double        settle;                     //今结算  74        郑商所有
  int64_t       position;                   //最新持仓量  76
  double        average_price;              //均价  79          郑商所有
  double        high_limit;                 //涨停板  211
  double        low_limit;                  //跌停板  212
}FuturesMarketSnapshot;

//交易所及第三方指数快照
typedef struct IndexSnapshot
{
  char					wind_code[16];
  time_stamp_t	data_time_stamp;							//交易时间，2
  double				pre_close;										//昨日收盘指数，632
  double				open_price;										//今日开盘指数，5
  double				high_price;										//最高指数，6
  double				low_price;										//最低指数，7
  double				new_price;										//最新指数，3
  int64_t				total_volume;									//总成交量,8
  double				total_amount;									//相应指数的总成交金额，59
}IndexSnapshot;

// 外汇市场快照
typedef struct FXMarketSnapshot
{
	char wind_code[16];
	date_t trade_date;
	time_stamp_t data_time_stamp;
	double pre_close;
	double open_price;
	double high_price;
	double low_price;
	double new_price;
  double bid_price1;
	double ask_price1;
}FXMarketSnapshot;

#pragma pack(pop)



#endif
