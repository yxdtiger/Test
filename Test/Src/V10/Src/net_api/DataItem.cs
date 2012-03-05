using System;
using System.Collections.Generic;
using System.Text;

namespace feed_net_api
{
    public class MdfMessage
    {
        public int local_time;      // 本地时间
        public uint packet_num;          // 包号
        public short product_id;         // 产品ID
        public short message_id;         // 消息ID
        public byte major_version;       // 主版本
        public byte minor_version;       // 副版本号
        public int record_count;         // 数据记录条数
        public DataItem[] records;          // 数据记录
    };

    public class DataItem
    {
    }

    /************************************************************************/
    /*                         基本传输消息                           */
    /************************************************************************/
    // 基本传输消息头。
    public class SystemMessageHeader
    {
        public byte major_version { get; set; }          //主版本号
        public byte minor_version { get; set; }          //副版本号
    };

    //心跳包
    public class HeartBeat:DataItem
    {
        public byte place_holder { get; set; }           //心跳占位
    };

    // 订阅产品的信息
    public class ProductInfo
    {
        public short product_id { get; set; }           //产品ID
        public byte major_version { get; set; }        //主版本号
        public byte minor_version { get; set; }        //副版本号
        public byte codec { get; set; }                //编码器类型 UNKNOW = 0,FAST = 1,BINARY = 2, RAWDATA = 3, FEEDHANDLER=4
    };

    //登录请求消息，由客户端发送至服务端
    public class LoginRequest : DataItem
    {
        public string user_name { get; set; }         //用户名最长32字节
        public string password { get; set; }          //密码最长16字节
        public short product_count { get; set; }         //订阅产品个数
        ProductInfo[] product { get; set; }               //产品具体描述ProductInfo
    };

    //服务器信息
    public class ServerInfo
    {
        public string server_addr { get; set; }       //IP地址
        public uint server_port { get; set; }           //服务端口
    };

    //消息状态信息
    public class MessageInfo
    {
        public short message_id { get; set; }           //消息ID
        public byte message_status { get; set; }       //订阅是否成功  1： valid; 2： invalid;
    };

    //产品订阅状态信息
    public class ProductStatus
    {
        public short product_id { get; set; }            //消息ID
        public byte product_status { get; set; }        //产品订阅状态 1 valid; 2 invalid;
        public byte major_version { get; set; }         //主版本号
        public byte minor_version { get; set; }         //副版本号
        public byte codec { get; set; }                 //编码器类型 UNKNOW = 0,FAST = 1,BINARY = 2, RAWDATA = 3, FEEDHANDLER=4
        public int message_count { get; set; }         //订阅消息个数
        public MessageInfo[] message_info;          //消息状态MessageInfo
    };

    // 登录响应消息,由服务端响应客户端
    public class LoginResponse : DataItem
    {
        public byte ack_type { get; set; }            //订阅响应类型  1 valid; 2 invalid;       
        public uint user_id { get; set; }             //用户ID
        public int server_count { get; set; }                //服务器个数
        public ServerInfo[] serverinfo;          //服务器信息
        public int product_count { get; set; }       //订阅产品个数
        public ProductStatus[] product_status;      //产品状态信息
    };

    // 登出通知消息, 用于客户端登出WMDF系统
    public class Logout : DataItem
    {
        public byte logout_type { get; set; }            // define logout type. 1 is server notify to logout;
    };

    //市场预开盘和收盘时发送
    public class MarketDateSignal : DataItem
    {
        public byte market_operation { get; set; }        //1= 开盘 2 = 收盘
        public int market_date { get; set; }             //交易日期
    };

    /************************************************************************/
    /*                         监控消息                               */
    /************************************************************************/
    // 监控消息头
    public class MonitorMessageHeader
    {
        public byte place_holder { get; set; }               //占位符  
    };

    //发送数据统计
    public class TransferDataStat : DataItem
    {
        public short product_id { get; set; }                 //产品ID
        public string source_ip { get; set; }              //服务ip
        public int source_port { get; set; }                //服务端口
        public byte source_status { get; set; }              //服务器状态

        public double receive_data_flow { get; set; }          //接收到的字节流量(KB/S)
        public double receive_pack_flow { get; set; }          //接收到的包流量
        public uint receive_packet_count { get; set; }       //接收到的包数
        public uint pre_receive_packet_count { get; set; }   //上次统计时receive包数
        public ulong receive_data_size { get; set; }          //原始大小(KB)
        public ulong pre_receive_data_size { get; set; }      //上次统计时编码后大小(KB)

        double send_data_flow { get; set; }             //发送字节流量(KB/S)
        double send_pack_flow { get; set; }             //发送包流量
        public uint send_packet_count { get; set; }          //已发送包数
        public uint pre_send_packet_count { get; set; }      //上次统计时已发送包数
        public uint send_packet_delta { get; set; }          //前次统计到现在发送的包数 

        public ulong encoded_size { get; set; }               //编码后大小(KB)
        public ulong pre_encoded_size { get; set; }           //上次统计时编码后大小(KB)  

        public ulong binary_class_size { get; set; }         //二进制结构体大小(KB)  
        double compression_ratio { get; set; }           //压缩比(%)
        public uint total_msg_delay { get; set; }            //平均延迟(ms)
        public uint pre_total_msg_delay { get; set; }        //上次消息的延迟
        public uint msg_avg_delay { get; set; }              //平均延迟
        public uint delayed_msg_delta { get; set; }          //前次统计延迟到现在发生的延迟包数
        public uint delayed_msg_count { get; set; }          //延迟包的总数
        public uint pre_delayed_msg_count { get; set; }      //上次延迟包的总数
    };
    public class ClientStatus : DataItem
    {
        public int socket_id { get; set; }               //socketid
        public string user_name { get; set; }           //用户名最长32字节
        public string client_ip { get; set; }           //客户IP 
        public int duration { get; set; }                //持续时间
        public double send_data_flow { get; set; }          //传输速率(KB/S)
        public long send_bytes { get; set; }              //总的发送字节数
        public long pre_send_bytes { get; set; }          //前一次发送字节数
        public int msg_in_queue { get; set; }            //缓存队列中的数据包个数
        public int product_count { get; set; }           //订阅产品个数
        public ProductInfo[] product_infos { get; set; }           //订阅产品信息
    };

    public class MonitorEvent : DataItem
    {
        public byte level { get; set; }                    //事件级别 FATAL=0, ERROR=1,WARN=2,INFO=3,DEBUG=4
        public DateTime date_time { get; set; }                //事件发生的时间
        public string at_where { get; set; }             //监控点
        public int event_id { get; set; }                 //事件ID
        public string what_happened { get; set; }       //事件内容，如 登录 “连接失败“等 
    };


    /************************************************************************/
    /*                         行情数据消息                           */
    /************************************************************************/

    // 行情数据消息头
    public class MarketMessageHeader
    {
        public byte place_holder { get; set; }
    };

    // 沪深L1市场快照
    public class L1MarketSnapshot : DataItem
    {
        public string wind_code { get; set; }              //合约代码
        public TimeSpan data_time_stamp { get; set; }              //时间戳
        public double pre_close { get; set; }              //前收价
        public double open_price { get; set; }              //当日开盘价
        public double high_price { get; set; }              //当日最高价
        public double low_price { get; set; }               //当日最低价
        public double new_price { get; set; }               //最新价
        public long total_volume { get; set; }                  //成交量
        public double total_amount { get; set; }	           //成交额  
        public double bid_price1 { get; set; }              //买1价
        public double bid_price2 { get; set; }              //买2价
        public double bid_price3 { get; set; }              //买3价
        public double bid_price4 { get; set; }              //买4价
        public double bid_price5 { get; set; }              //买5价
        public double ask_price1 { get; set; }              //卖1价
        public double ask_price2 { get; set; }              //卖2价
        public double ask_price3 { get; set; }              //卖3价
        public double ask_price4 { get; set; }              //卖4价
        public double ask_price5 { get; set; }              //卖5价
        public int ask_size1 { get; set; }             //卖1量
        public int ask_size2 { get; set; }             //卖2量
        public int ask_size3 { get; set; }             //卖3量
        public int ask_size4 { get; set; }             //卖4量
        public int ask_size5 { get; set; }             //卖5量
        public int bid_size1 { get; set; }             //买1量
        public int bid_size2 { get; set; }             //买2量
        public int bid_size3 { get; set; }             //买3量
        public int bid_size4 { get; set; }             //买4量
        public int bid_size5 { get; set; }             //买5量	
    };

    // L2市场快照
    public class L2MarketSnapshot : DataItem
    {
        public string wind_code { get; set; }              //合约代码          
        public TimeSpan data_time_stamp { get; set; }                 //交易时间，2  
        public double prev_close { get; set; }                 //前收价，4
        public double open_price { get; set; }                 //开盘价，5
        public double high_price { get; set; }                 //最高价，6
        public double low_price { get; set; }                  //最低价，7
        public double new_price { get; set; }                  //最新价，3
        public long total_volume { get; set; }                     //总成交量，8
        public double total_amount { get; set; }               //总成交额，59
        public double bid_price1 { get; set; }                 //买1价，11
        public double bid_price2 { get; set; }                 //买2价，12            
        public double bid_price3 { get; set; }                 //买3价，13
        public double bid_price4 { get; set; }                 //买4价，14
        public double bid_price5 { get; set; }                 //买5价，15
        public double bid_price6 { get; set; }                 //买6价，16
        public double bid_price7 { get; set; }                 //买7价，17
        public double bid_price8 { get; set; }                 //买8价，18
        public double bid_price9 { get; set; }                 //买9价，19
        public double bid_price10 { get; set; }                //买10价，20
        public double ask_price1 { get; set; }                 //卖1价，21
        public double ask_price2 { get; set; }                 //卖2价，22
        public double ask_price3 { get; set; }                 //卖3价，23
        public double ask_price4 { get; set; }                 //卖4价，24
        public double ask_price5 { get; set; }                 //卖5价，25
        public double ask_price6 { get; set; }                 //卖6价，26
        public double ask_price7 { get; set; }                 //卖7价，27              
        public double ask_price8 { get; set; }                 //卖8价，28
        public double ask_price9 { get; set; }                 //卖9价，29
        public double ask_price10 { get; set; }                //卖10价，30
        public int bid_size1 { get; set; }                //买1量，31
        public int bid_size2 { get; set; }                //买2量，32
        public int bid_size3 { get; set; }                //买3量，33
        public int bid_size4 { get; set; }                //买4量，34
        public int bid_size5 { get; set; }                //买5量，35
        public int bid_size6 { get; set; }                //买6量，36
        public int bid_size7 { get; set; }                //买7量，37
        public int bid_size8 { get; set; }                //买8量，38
        public int bid_size9 { get; set; }                //买9量，39
        public int bid_size10 { get; set; }               //买10量，40
        public int ask_size1 { get; set; }                //卖1量，41
        public int ask_size2 { get; set; }                //卖2量，42
        public int ask_size3 { get; set; }                //卖3量，43
        public int ask_size4 { get; set; }                //卖4量，44
        public int ask_size5 { get; set; }                //卖5量，45
        public int ask_size6 { get; set; }                //卖6量，46
        public int ask_size7 { get; set; }                //卖7量，47
        public int ask_size8 { get; set; }                //卖8量，48
        public int ask_size9 { get; set; }                //卖9量，49
        public int ask_size10 { get; set; }               //卖10量，50
        public long transactions_count { get; set; }         //交易总笔数, 71
        public double weighted_average_bid_price { get; set; }  //加权平均委买价格，86，沪L2
        public double weighted_average_ask_price { get; set; }  //加权平均委卖价格，87，沪L2
        public long l2_total_bid_size { get; set; }           //委托买入总量，340，沪L2
        public long l2_total_ask_size { get; set; }         //委卖卖出总量，341，沪L2
        public double peratio1 { get; set; }                   // SZSE 股票：价格/上年每股利润
        // SZSE 债券：每百元应计利息
        public double peratio2 { get; set; }                   // SZSE     股票：价格/本年每股利润
        // SZSE/SSE 债券：到期收益率
        // SZSE/SSE 基金：每百份的IOPV或净值
        // SZSE     权证：溢价率
        public byte trade_status { get; set; }               // 交易状态 1=正常交易 2=停牌      v1.1
    };


    // 逐笔成交

    public class L2Transaction : DataItem
    {
        public string wind_code { get; set; }              //合约代码
        public TimeSpan trade_time { get; set; }                 //交易时间
        public int volume { get; set; }                  //成交数量
        public double price { get; set; }                      //成交价格
        public long amount { get; set; }                //成交金额X sse
        public int set_no { get; set; }                     //证券集代号 szse  
        public int rec_no { get; set; }                     //成交序号
        public int buyOrderRecNo { get; set; }              //买方委托索引 szse
        public int sellOrderRecNo { get; set; }             //卖方委托索引 szse
        public byte order_kind { get; set; }                 //成交类别 szse
        public byte function_code { get; set; }              //成交代码 szse
    };


    // L2委托队列级别量
    public class L2OrderQueueItem
    {
        public int operator_type { get; set; }           //委托单操作0-Absent, 1-Add,2-Update,3-Delete
        public int entry_id { get; set; }  //委托单在队列中的位置Start from 0, No Meaning if
        public int size { get; set; }                      //委托量
    };

    // L2委托队列级别
    public class L2OrderQueuePriceLevel
    {
        public int operator_type { get; set; }       //价格操作符0-Absent,1-Add,2-Update,3-Delete
        public double order_price { get; set; }                      //价格
        public int total_number { get; set; }               //当前价位上总委托笔数
        public int orders_number { get; set; }               //此次发生变化的委托笔数
        public L2OrderQueueItem[] order_items;          //委托单个数 等于 orders_count
    };

    // L2委托队列
    public class L2OrderQueue : DataItem
    {
        public string wind_code { get; set; }              //合约代码
        public TimeSpan date_time_stamp { get; set; }      //时间戳
        public byte order_side { get; set; }                 //委托方向 1 = Bid Queue, 2 = Ask Queue, other: Invalid
        public int image_status { get; set; }         //影像类型 1 = FullImage, 2 = Update, other: Invalid
        public int price_level_count { get; set; }    //价格个数 Number of PriceLevelItems, should = PriceLevel_items.size();
        public L2OrderQueuePriceLevel[] price_levels;   //委托价格上的队列详细信息 
    };



    // 深L2逐笔委托
    public class SZSEL2Order : DataItem
    {
        public string wind_code { get; set; }              //合约代码
        public TimeSpan order_time { get; set; }                    //委托时间
        public int set_no { get; set; }                        //证券集代码
        public int rec_no { get; set; }                        //委托索引
        public int size { get; set; }                     //委托数量,  股
        public double price { get; set; }                   //价格
        public byte order_kind { get; set; }                    //委托类别,  1=委买, 2=委卖, 3=撤单
        public byte function_code { get; set; }                 //成交代码
    };

    // 中金所L2市场快照
    public class CFEL2MarketSnapshot : DataItem
    {
        public string wind_code { get; set; }              //合约代码
        public Date trade_date { get; set; }                  // 交易日期, yyyymmdd			1
        public TimeSpan data_time_stamp { get; set; }                  // 最后修改时间, hhMMss		2
        public double prev_close { get; set; }                  // 昨收盘					4
        public double open_price { get; set; }                  // 今开盘					5
        public double high_price { get; set; }                  // 今最高价					6
        public double low_price { get; set; }                   // 今最低价					7
        public double new_price { get; set; }                   // 最新价					3
        public long total_volume { get; set; }                      // 总成交量					8
        public double total_amount { get; set; }                // 总成交金额				59
        public double bid_price1 { get; set; }                  // 申买价一				11
        public double bid_price2 { get; set; }                  // 申买价二				12
        public double bid_price3 { get; set; }                  // 申买价三				13
        public double bid_price4 { get; set; }                  // 申买价四				14
        public double bid_price5 { get; set; }                  // 申买价五				15
        public double ask_price1 { get; set; }                  // 申卖价一				21
        public double ask_price2 { get; set; }                  // 申卖价二				22
        public double ask_price3 { get; set; }                  // 申卖价三				23
        public double ask_price4 { get; set; }                  // 申卖价四				24
        public double ask_price5 { get; set; }                  // 申卖价五				25
        public int ask_size1 { get; set; }                 // 申卖量一				41
        public int ask_size2 { get; set; }                 // 申卖量二				42
        public int ask_size3 { get; set; }                 // 申卖量三				43
        public int ask_size4 { get; set; }                 // 申买量四				34
        public int ask_size5 { get; set; }                 // 申卖量五				45
        public int bid_size1 { get; set; }                 // 申买量一				31
        public int bid_size2 { get; set; }                 // 申买量二				32
        public int bid_size3 { get; set; }                 // 申买量三				33
        public int bid_size4 { get; set; }                 // 申卖量四				44
        public int bid_size5 { get; set; }                 // 申买量五				35
        public double pre_settle { get; set; }                 // 昨结算					75
        public long position { get; set; }                    // 最新持仓量				76
        public long pre_position { get; set; }               // 昨持仓量					77
        public double high_limit { get; set; }                  // 涨停板价					211	
        public double low_limit { get; set; }                   // 跌停板价					212
    };

    // 期货L1市场快照
    public class FuturesMarketSnapshot : DataItem
    {
        public string wind_code { get; set; }              //合约代码
        public int trade_date { get; set; }                 //交易日    1
        public TimeSpan data_time_stamp { get; set; }                 //最后修改时间 2
        public double pre_close { get; set; }                 //昨收盘  4
        public double open_price { get; set; }                 //今开盘  5
        public double high_price { get; set; }                 //最高价  6
        public double low_price { get; set; }                  //最低价  7
        public double new_price { get; set; }                  //最新价  3
        public long total_volume { get; set; }                     //总成交量  8
        public double total_amount { get; set; }                     //总成交金额  59    郑商所无
        public double bid_price1 { get; set; }                 //申买价一  11
        public double ask_price1 { get; set; }                 //申卖价一  21
        public int ask_size1 { get; set; }                //申卖量一  41
        public int bid_size1 { get; set; }                //申买量一  31
        public double pre_settle { get; set; }                //昨结算  75        中金所、郑商所有
        public long pre_position { get; set; }              //昨持仓量    77   中金所有
        public double settle { get; set; }                     //今结算  74        郑商所有
        public long position { get; set; }                   //最新持仓量  76   中金所、郑商所有
        public double average_price { get; set; }              //均价  79          郑商所有
        public double high_limit { get; set; }                 //涨停板  211
        public double low_limit { get; set; }                  //跌停板  212
    };

    //交易所及第三方指数快照
    public class IndexSnapshot : DataItem
    {
        public string wind_code { get; set; }
        public TimeSpan data_time_stamp { get; set; }							//交易时间，2
        public double pre_close { get; set; }										//昨日收盘指数，632
        public double open_price { get; set; }										//今日开盘指数，5
        public double high_price { get; set; }										//最高指数，6
        public double low_price { get; set; }										//最低指数，7
        public double new_price { get; set; }										//最新指数，3
        public long total_volume { get; set; }												//总成交量,8
        public double total_amount { get; set; }									//相应指数的总成交金额，59
    };

}

