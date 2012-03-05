using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices;

namespace feed_net_api
{


    /************************************************************************/
    /*                         基本传输消息                           */
    /************************************************************************/
    // 基本传输消息头。
    [StructLayout(LayoutKind.Sequential, Pack = 1)]
    public struct sSystemMessageHeader
    {
        public byte major_version;          //主版本号
        public byte minor_version;          //副版本号
    };

    //心跳包
    [StructLayout(LayoutKind.Sequential, Pack = 1)]
    public struct sHeartBeat
    {
        public byte place_holder;           //心跳占位
    };

    // 订阅产品的信息
    [StructLayout(LayoutKind.Sequential, Pack = 1)]
    public struct sProductInfo
    {
        public short product_id;           //产品ID
        public byte major_version;        //主版本号
        public byte minor_version;        //副版本号
        public byte codec;                //编码器类型 UNKNOW = 0,FAST = 1,BINARY = 2, RAWDATA = 3, FEEDHANDLER=4
    };

    //登录请求消息，由客户端发送至服务端
    [StructLayout(LayoutKind.Sequential, Pack = 1)]
    public struct sLoginRequest
    {
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 32)]
        public byte[] user_name;         //用户名最长32字节
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 16)]
        public byte[] password;          //密码最长16字节
        public short product_count;         //订阅产品个数
        public IntPtr product;               //产品具体描述ProductInfo
    };

    //服务器信息
    [StructLayout(LayoutKind.Sequential, Pack = 1)]
    public struct sServerInfo
    {
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 15)]
        public byte[] server_addr;       //IP地址
        public uint server_port;           //服务端口
    };

    //消息状态信息
    [StructLayout(LayoutKind.Sequential, Pack = 1)]
    public struct sMessageInfo
    {
        public short message_id;           //消息ID
        public byte message_status;       //订阅是否成功  1： valid; 2： invalid;
    };

    //产品订阅状态信息
    [StructLayout(LayoutKind.Sequential, Pack = 1)]
    public struct sProductStatus
    {
        public short product_id;            //消息ID
        public byte product_status;        //产品订阅状态 1 valid; 2 invalid;
        public byte major_version;         //主版本号
        public byte minor_version;         //副版本号
        public byte codec;                 //编码器类型 UNKNOW = 0,FAST = 1,BINARY = 2, RAWDATA = 3, FEEDHANDLER=4
        public int message_count;         //订阅消息个数
        public IntPtr message_info;          //消息状态MessageInfo
    };

    // 登录响应消息,由服务端响应客户端
    [StructLayout(LayoutKind.Sequential, Pack = 1)]
    public struct sLoginResponse
    {
        public byte ack_type;            //订阅响应类型  1 valid; 2 invalid;       
        public uint user_id;             //用户ID
        public int server_count;        //服务器个数
        public IntPtr serverinfo;          //服务器信息
        public int product_count;       //订阅产品个数
        public IntPtr product_status;      //产品状态信息
    };

    // 登出通知消息, 用于客户端登出WMDF系统
    [StructLayout(LayoutKind.Sequential, Pack = 1)]
    public struct sLogout
    {
        public byte logout_type;            // define logout type. 1 is server notify to logout;
    };

    //市场预开盘和收盘时发送
    [StructLayout(LayoutKind.Sequential, Pack = 1)]
    public struct sMarketDateSignal
    {
        public byte market_operation;        //1= 开盘 2 = 收盘
        public int market_date;             //交易日期
        public short product_id;
    };

    /************************************************************************/
    /*                         监控消息                               */
    /************************************************************************/
    // 监控消息头
    [StructLayout(LayoutKind.Sequential, Pack = 1)]
    public struct sMonitorMessageHeader
    {
        public byte place_holder;               //占位符  
    };

    //发送数据统计
    [StructLayout(LayoutKind.Sequential, Pack = 1)]
    public struct sTransferDataStat
    {
        public short product_id;                 //产品ID
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 15)]
        public byte[] source_ip;                 //服务ip
        public int source_port;                  //服务端口
        byte source_status;                      //服务器状态

        public double receive_data_flow;          //接收到的字节流量(KB/S)
        public double receive_pack_flow;          //接收到的包流量
        public uint receive_packet_count;         //接收到的包数
        public uint pre_receive_packet_count;     //上次统计时receive包数
        public ulong receive_data_size;           //原始大小(KB)
        public ulong pre_receive_data_size;       //上次统计时编码后大小(KB)

        public double send_data_flow;             //发送字节流量(KB/S)
        public double send_pack_flow;             //发送包流量
        public uint send_packet_count;            //已发送包数
        public uint pre_send_packet_count;        //上次统计时已发送包数
        public uint send_packet_delta;            //前次统计到现在发送的包数 

        public ulong encoded_size;               //编码后大小(KB)
        public ulong pre_encoded_size;           //上次统计时编码后大小(KB)  

        public ulong binary_struct_size;         //二进制结构体大小(KB)  
        public double compression_ratio;           //压缩比(%)
        public uint total_msg_delay;             //平均延迟(ms)
        public uint pre_total_msg_delay;         //上次消息的延迟
        public uint msg_avg_delay;               //平均延迟
        public uint delayed_msg_delta;           //前次统计延迟到现在发生的延迟包数
        public uint delayed_msg_count;           //延迟包的总数
        public uint pre_delayed_msg_count;       //上次延迟包的总数
    };
    [StructLayout(LayoutKind.Sequential, Pack = 1)]
    public struct sClientStatus
    {
        public int socket_id;                  //socketid
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 32)]
        public byte[] user_name;               //用户名最长32字节
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 15)]
        public byte[] client_ip;              //客户IP 
        public int duration;                  //持续时间
        public double send_data_flow;         //传输速率(KB/S)
        public long send_bytes;              //总的发送字节数
        public long pre_send_bytes;          //前一次发送字节数
        public int msg_in_queue;            //缓存队列中的数据包个数
        public int product_count;           //订阅产品个数
        public IntPtr product_infos;           //订阅产品信息
    };
    [StructLayout(LayoutKind.Sequential, Pack = 1)]
    public struct sMonitorEvent
    {
        public byte level;                    //事件级别 FATAL=0, ERROR=1,WARN=2,INFO=3,DEBUG=4
        public double date_time;                //事件发生的时间
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 60)]
        public byte[] at_where;             //监控点
        public int event_id;                 //事件ID
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 150)]
        public byte[] what_happened;       //事件内容，如 登录 “连接失败“等 
    };


    /************************************************************************/
    /*                         行情数据消息                           */
    /************************************************************************/

    // 行情数据消息头
    [StructLayout(LayoutKind.Sequential, Pack = 1)]
    public struct sMarketMessageHeader
    {
        public byte place_holder;
    };

    // 沪深L1市场快照
    [StructLayout(LayoutKind.Sequential, Pack = 1)]
    public struct sL1MarketSnapshot
    {
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 16)]
        public byte[] wind_code;              //合约代码
        public int data_time_stamp;              //时间戳
        public double pre_close;              //前收价
        public double open_price;              //当日开盘价
        public double high_price;              //当日最高价
        public double low_price;               //当日最低价
        public double new_price;               //最新价
        public long total_volume;                  //成交量
        public double total_amount;	           //成交额  
        public double bid_price1;              //买1价
        public double bid_price2;              //买2价
        public double bid_price3;              //买3价
        public double bid_price4;              //买4价
        public double bid_price5;              //买5价
        public double ask_price1;              //卖1价
        public double ask_price2;              //卖2价
        public double ask_price3;              //卖3价
        public double ask_price4;              //卖4价
        public double ask_price5;              //卖5价
        public int ask_size1;             //卖1量
        public int ask_size2;             //卖2量
        public int ask_size3;             //卖3量
        public int ask_size4;             //卖4量
        public int ask_size5;             //卖5量
        public int bid_size1;             //买1量
        public int bid_size2;             //买2量
        public int bid_size3;             //买3量
        public int bid_size4;             //买4量
        public int bid_size5;             //买5量	
    };

    // L2市场快照
    [StructLayout(LayoutKind.Sequential, Pack = 1)]
    public struct sL2MarketSnapshot
    {
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 16)]
        public byte[] wind_code;              //合约代码          
        public int data_time_stamp;                 //交易时间，2  
        public double prev_close;                 //前收价，4
        public double open_price;                 //开盘价，5
        public double high_price;                 //最高价，6
        public double low_price;                  //最低价，7
        public double new_price;                  //最新价，3
        public long total_volume;                     //总成交量，8
        public double total_amount;               //总成交额，59
        public double bid_price1;                 //买1价，11
        public double bid_price2;                 //买2价，12            
        public double bid_price3;                 //买3价，13
        public double bid_price4;                 //买4价，14
        public double bid_price5;                 //买5价，15
        public double bid_price6;                 //买6价，16
        public double bid_price7;                 //买7价，17
        public double bid_price8;                 //买8价，18
        public double bid_price9;                 //买9价，19
        public double bid_price10;                //买10价，20
        public double ask_price1;                 //卖1价，21
        public double ask_price2;                 //卖2价，22
        public double ask_price3;                 //卖3价，23
        public double ask_price4;                 //卖4价，24
        public double ask_price5;                 //卖5价，25
        public double ask_price6;                 //卖6价，26
        public double ask_price7;                 //卖7价，27              
        public double ask_price8;                 //卖8价，28
        public double ask_price9;                 //卖9价，29
        public double ask_price10;                //卖10价，30
        public int bid_size1;                //买1量，31
        public int bid_size2;                //买2量，32
        public int bid_size3;                //买3量，33
        public int bid_size4;                //买4量，34
        public int bid_size5;                //买5量，35
        public int bid_size6;                //买6量，36
        public int bid_size7;                //买7量，37
        public int bid_size8;                //买8量，38
        public int bid_size9;                //买9量，39
        public int bid_size10;               //买10量，40
        public int ask_size1;                //卖1量，41
        public int ask_size2;                //卖2量，42
        public int ask_size3;                //卖3量，43
        public int ask_size4;                //卖4量，44
        public int ask_size5;                //卖5量，45
        public int ask_size6;                //卖6量，46
        public int ask_size7;                //卖7量，47
        public int ask_size8;                //卖8量，48
        public int ask_size9;                //卖9量，49
        public int ask_size10;               //卖10量，50
        public long transactions_count;         //交易总笔数, 71
        public double weighted_average_bid_price;  //加权平均委买价格，86，沪L2
        public double weighted_average_ask_price;  //加权平均委卖价格，87，沪L2
        public long l2_total_bid_size;           //委托买入总量，340，沪L2
        public long l2_total_ask_size;         //委卖卖出总量，341，沪L2
        public double peratio1;                   // SZSE 股票：价格/上年每股利润
        // SZSE 债券：每百元应计利息
        public double peratio2;                   // SZSE     股票：价格/本年每股利润
        // SZSE/SSE 债券：到期收益率
        // SZSE/SSE 基金：每百份的IOPV或净值
        // SZSE     权证：溢价率
        public byte trade_status;               // 交易状态 1=正常交易 2=停牌      v1.1
    };


    // 逐笔成交
    [StructLayout(LayoutKind.Sequential, Pack = 1)]
    public struct sL2Transaction
    {
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 16)]
        public byte[] wind_code;              //合约代码
        public int trade_time;                 //交易时间
        public int volume;                  //成交数量
        public double price;                      //成交价格
        public long amount;                //成交金额X sse
        public int set_no;                     //证券集代号 szse  
        public int rec_no;                     //成交序号
        public int buyOrderRecNo;              //买方委托索引 szse
        public int sellOrderRecNo;             //卖方委托索引 szse
        public byte order_kind;                 //成交类别 szse
        public byte function_code;              //成交代码 szse
    };


    // L2委托队列级别量
    [StructLayout(LayoutKind.Sequential, Pack = 1)]
    public struct sL2OrderQueueItem
    {
        public int operator_type;           //委托单操作0-Absent, 1-Add,2-Update,3-Delete
        public int entry_id;  //委托单在队列中的位置Start from 0, No Meaning if
        public int size;                      //委托量
    };

    // L2委托队列级别
    [StructLayout(LayoutKind.Sequential, Pack = 1)]
    public struct sL2OrderQueuePriceLevel
    {
        public int operator_type;       //价格操作符0-Absent,1-Add,2-Update,3-Delete
        public double order_price;                      //价格
        public int total_number;               //当前价位上总委托笔数
        public int orders_number;               //此次发生变化的委托笔数
        public IntPtr order_items;          //委托单个数 等于 orders_count
    };

    // L2委托队列
    [StructLayout(LayoutKind.Sequential, Pack = 1)]
    public struct sL2OrderQueue
    {
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 16)]
        public byte[] wind_code;              //合约代码
        public int date_time_stamp;      //时间戳
        public byte order_side;                 //委托方向 1 = Bid Queue, 2 = Ask Queue, other: Invalid
        public int image_status;         //影像类型 1 = FullImage, 2 = Update, other: Invalid
        public int price_level_count;    //价格个数 Number of PriceLevelItems, should = PriceLevel_items.size();
        public IntPtr price_levels;   //委托价格上的队列详细信息 
    };



    // 深L2逐笔委托
    [StructLayout(LayoutKind.Sequential, Pack = 1)]
    public struct sSZSEL2Order
    {
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 16)]
        public byte[] wind_code;              //合约代码
        public int order_time;                    //委托时间
        public int set_no;                        //证券集代码
        public int rec_no;                        //委托索引
        public int size;                     //委托数量,  股
        public double price;                   //价格
        public byte order_kind;                    //委托类别,  1=委买, 2=委卖, 3=撤单
        public byte function_code;                 //成交代码
    };

    // 中金所L2市场快照
    [StructLayout(LayoutKind.Sequential, Pack = 1)]
    public struct sCFEL2MarketSnapshot
    {
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 16)]
        public byte[] wind_code;              //合约代码
        public int trade_date;                  // 交易日期, yyyymmdd			1
        public int data_time_stamp;                  // 最后修改时间, hhMMss		2
        public double prev_close;                  // 昨收盘					4
        public double open_price;                  // 今开盘					5
        public double high_price;                  // 今最高价					6
        public double low_price;                   // 今最低价					7
        public double new_price;                   // 最新价					3
        public long total_volume;                      // 总成交量					8
        public double total_amount;                // 总成交金额				59
        public double bid_price1;                  // 申买价一				11
        public double bid_price2;                  // 申买价二				12
        public double bid_price3;                  // 申买价三				13
        public double bid_price4;                  // 申买价四				14
        public double bid_price5;                  // 申买价五				15
        public double ask_price1;                  // 申卖价一				21
        public double ask_price2;                  // 申卖价二				22
        public double ask_price3;                  // 申卖价三				23
        public double ask_price4;                  // 申卖价四				24
        public double ask_price5;                  // 申卖价五				25
        public int ask_size1;                 // 申卖量一				41
        public int ask_size2;                 // 申卖量二				42
        public int ask_size3;                 // 申卖量三				43
        public int ask_size4;                 // 申买量四				34
        public int ask_size5;                 // 申卖量五				45
        public int bid_size1;                 // 申买量一				31
        public int bid_size2;                 // 申买量二				32
        public int bid_size3;                 // 申买量三				33
        public int bid_size4;                 // 申卖量四				44
        public int bid_size5;                 // 申买量五				35
        public double pre_settle;                 // 昨结算					75
        public long position;                    // 最新持仓量				76
        public long pre_position;               // 昨持仓量					77
        public double high_limit;                  // 涨停板价					211	
        public double low_limit;                   // 跌停板价					212
    };

    // 期货L1市场快照
    [StructLayout(LayoutKind.Sequential, Pack = 1)]
    public struct sFuturesMarketSnapshot
    {
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 16)]
        public byte[] wind_code;              //合约代码
        public int trade_date;                 //交易日    1
        public int data_time_stamp;                 //最后修改时间 2
        public double pre_close;                 //昨收盘  4
        public double open_price;                 //今开盘  5
        public double high_price;                 //最高价  6
        public double low_price;                  //最低价  7
        public double new_price;                  //最新价  3
        public long total_volume;                     //总成交量  8
        public double total_amount;                     //总成交金额  59    郑商所无
        public double bid_price1;                 //申买价一  11
        public double ask_price1;                 //申卖价一  21
        public int ask_size1;                //申卖量一  41
        public int bid_size1;                //申买量一  31
        public double pre_settle;                //昨结算  75        中金所、郑商所有
        public long pre_position;              //昨持仓量    77   中金所有
        public double settle;                     //今结算  74        郑商所有
        public long position;                   //最新持仓量  76   中金所、郑商所有
        public double average_price;              //均价  79          郑商所有
        public double high_limit;                 //涨停板  211
        public double low_limit;                  //跌停板  212
    };

    //交易所及第三方指数快照
    [StructLayout(LayoutKind.Sequential, Pack = 1)]
    public struct sIndexSnapshot
    {
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 16)]
        public byte[] wind_code;
        public int data_time_stamp;							//交易时间，2
        public double pre_close;										//昨日收盘指数，632
        public double open_price;										//今日开盘指数，5
        public double high_price;										//最高指数，6
        public double low_price;										//最低指数，7
        public double new_price;										//最新指数，3
        public long total_volume;												//总成交量,8
        public double total_amount;									//相应指数的总成交金额，59
    };

}
