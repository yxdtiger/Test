using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices;

namespace feed_net_api
{
    public static class WmdfConstant
    {
        public const sbyte WMDF_CONSTS_8BIT_NIL_VALUE = SByte.MaxValue;
        public const int WMDF_CONSTS_32BIT_NIL_VALUE = Int32.MaxValue;
        public const long WMDF_CONSTS_64BIT_NIL_VALUE = Int64.MaxValue;
        public const double WMDF_CONSTS_DOUBLE_NIL_VALUE = Double.NaN;
        public const string WMDF_CONSTS_STRING_NIL_VALUE = "";

        /************************************************************************/
        /*                         WMDF产品ID                                  */
        /************************************************************************/
        public const short PRODUCT_INVALID_ID = 0;          //无效产品ID
        public const short PRODEUCT_SYSTEM_ID = 1;          //系统消息
        public const short PRODEUCT_MONITOR_ID = 2;         //监控产品
        public const short PRODUCT_SSE_LEVEL1_ID = 10;      //上交所Level1
        public const short PRODUCT_SZSE_LEVEL1_ID = 11;     //深交所Level1
        public const short PRODUCT_SSE_LEVEL2_ID = 12;      //上交所Level2
        public const short PRODUCT_SZSE_LEVEL2_ID = 13;     //深交所Level2

        public const short PRODUCT_CFFE_LEVEL1_ID = 20;     //中金所Level1
        public const short PRODUCT_CFFE_LEVEL2_ID = 21;     //中金所Level2
        public const short PRODUCT_SHFE_LEVEL1_ID = 22;     //上期所Level1
        public const short PRODUCT_DCE_LEVEL1_ID = 23;      //大商所Level1
        public const short PRODUCT_CZCE_LEVEL1_ID = 24;     //郑商所Level1

        public const short PRODUCT_FX_ID = 25;              //外汇

        public const short PRODUCT_FTSE_LEVEL1_ID = 26;     //新华富时


        public const short PRODUCT_SIMULATOR_ID = 2010;     //产品模拟器，用于无行情源时的调试
        public const short PRODUCT_RELAY_BEGIN_ID = 2011;   //Feed Server Relay, 用于FeedServer的级联,起始ID
        public const short PRODUCT_RELAY_END_ID = 2019;     //Feed Server Relay, 用于FeedServer的级联，截止ID

        /************************************************************************/
        /*                         WMDF产品消息定义                        */
        /************************************************************************/
        public const short MESSAGE_INVALID_ID = 00;          //无效消息ID

        public const short SYSTEM_HEARTBEAT_ID = 1;          //心跳消息
        public const short SYSTEM_LOGIN_REQUEST_ID = 2;      //登录请求消息
        public const short SYSTEM_LOGIN_RESPONSE_ID = 3;     //登录响应信息
        public const short SYSTEM_LOGOUT_ID = 4;             //登出通知信息
        public const short SYSTEM_MARKET_DATE_SIGNAL_ID = 5; //市场开始前和结束后发送交易日期信号ID

        public const short MONITOR_TRANSFER_STATUS_ID = 1;   // 网络传输状态消息
        public const short MONITOR_CLIENT_STATUS_ID = 2;     // 监控客户端状态消息
        public const short MONITOR_EVENT_ID = 3;             // 网络事件监控

        public const short SSEL1_MARKETDATA_ID = 1;          //上交所Level1快照

        public const short SZSEL1_MARKETDATA_ID = 1;         //深交所Level1快照

        public const short SSEL2_MARKETDATA_ID = 1;          //上交所Level2快照
        public const short SSEL2_TRANSACTION_ID = 2;         //上交所Level2逐笔成交
        public const short SSEL2_ORDERQUEUE_ID = 3;          //上交所Level2委托队列
        public const short SSEL2_INDEXDATA_ID = 4;	         //上交所Level2指数行情

        public const short SZSEL2_MARKETDATA_ID = 1;         //深交所Level2快照
        public const short SZSEL2_TRANSACTION_ID = 2;        //深交所Level2逐笔成交
        public const short SZSEL2_ORDERQUEUE_ID = 3;         //深交所Level2委托队列
        public const short SZSEL2_SINGLEORDER_ID = 4;        //深交所Level2逐笔委托数据
        public const short SZSEL2_INDEXDATA_ID = 5;	         //深交所Level2指数行情

        public const short CFFEL1_MARKETDATA_ID = 1;         //中金所Level1快照

        public const short CFFEL2_MARKETDATA_ID = 1;         //中金所Level2快照

        public const short SHFEL1_MARKETDATA_ID = 1;         //上期所Level1快照

        public const short DCEL1_MARKETDATA_ID = 1;          //大商所Level1快照

        public const short CZCEL1_MARKETDATA_ID = 1;         //郑商所Level1快照

        public const short FX_MARKETDATA_ID = 1;             //外汇快照

        public const short FTSE_INDEXDATA_ID = 1;            //新华富时指数快照



    }
    public enum eReturnValue
    {
        INVALID_VALUE = -1,	                 //无效返回值
        SUCCESS = 0,                         //执行成功

        AUTHORITY_VALID = 1,		         //权限有效
        AUTHORITY_INVALID = 2, 		         //权限无效
        SEND_DATA_FAILD = 3,                 //推送数据失败
        USER_NOT_EXIST = 4,                  //用户名不存在
        PASSWORD_INCORRECT = 5,              //密码错误
        IP_UNAUTHORIZED = 6,                 //IP未授权
        LOAD_API_FAILED = 7,                 //加载API失败
        UNLOAD_API_FAILED = 8,               //卸载API失败
        LOGOUT_FAILED = 9,                   //登出失败
        NORMAL_DISCONNECT = 10,              //正常断开
        ABNORMAL_DISCONNECT = 11,            //异常断开
        CONNECTION_ID_INVALID = 12,          //无效的连接ID(0-7)
        CONNECTION_OVERLOAD = 13,            //连接数超过最大限制数（8）
        CONNECTION_FAILED = 14,              //连接失败   
        IDEXPIRED = 15                       //账号过期

    };

    public enum MonitorEventId
    {
        MEI_NOMAL_EVENT = 0,                    //普通事件  
        MEI_CLIENT_CONNECTED = 1,               //客户端连接
        MEI_CLIENT_DISCONNECTED = 2,            //客户端断开连接
        MEI_CLIENT_NOTEXIST = 3,                //客户端连接不存在 
        MEI_AUTHORITY_CONFIRMED = 6,            //权限校验确认
        MEI_AUTHORITY_NONCONFORM = 7,           //权限校验不通过
        MEI_SOURCE_CONNECTED = 8,               //数据源适配连接成功 
        MEI_SOURCE_DISCONNECTED = 9,            //数据源适配断开连接
        MEI_SOURCE_NOTAVAILABLE = 10,           //数据源不存在
        MEI_SOURCE_LOGIN_FAILED = 11,           //登录数据源失败
        MEI_SOURCE_LOGIN_SUCCEEDED = 12,        //登录数据源成功
        MEI_FEED_SERVER_RESET = 13,             //服务端重置  
        MEI_FEED_SERVER_PREMARKETOPEN = 14,     //预开盘
        MEI_FEED_SERVER_MARKETCLOSE = 15        //收盘
    };

    //编码器类型
    public enum CodecType
    {
        CT_UNKNOW = 0,      //未知编码方式     
        CT_FAST = 1,        //fast编码
        CT_BINARY = 2,      //二进制编码
        CT_RAWDATA = 3,     //交易所原始数据
        CT_FEEDHANDLER = 4  //feedhandler格式
    };


    public enum MarketOperation
    {
        MO_OPEN = 1,             //市场开盘
        MO_CLOSE = 2             //市场收盘
    };


    public enum LogoutType
    {
        LT_SERVERNOTIFY         //服务程序通知客户端断开连接
    };


    [Serializable]
    public class Date : IComparable
    {
        public int i; private Date() { }
        public Date(int x) { i = x; }
        public Date(int y, int m, int d) : this(new DateTime(y, m, d)) { }
        public DateTime DateTime() { return i == -int.MaxValue ? System.DateTime.MinValue : i == int.MaxValue ? System.DateTime.MaxValue : new DateTime(i == int.MinValue ? 0L : (long)8.64e11 * i + k); }
        public Date(long x) { i = x == 0L ? int.MinValue : (int)(x / (long)8.64e11) - 730119; }
        public Date(DateTime z) : this(z.Ticks) { }
        public override string ToString() { return i == int.MinValue ? "0" : this.DateTime().ToString("d"); }
        public int ToInt32() { return Convert.ToInt32(ToString()); }
        public override bool Equals(object o) { if (o == null)return false; if (this.GetType() != o.GetType())return false; Date d = (Date)o; return i == d.i; }
        public override int GetHashCode() { return i; }
        public int CompareTo(object o) { if (o == null)return 1; Date other = o as Date; if (other == null)return 1; return i.CompareTo(other.i); }
        long k = (long)8.64e11 * 730119;
    }


    //初始化API时所需参数
    [StructLayout(LayoutKind.Sequential, Pack = 1)]
    public struct OpenArgs
    {
        public byte connection_id;         //0-31(最多支持32个连接)
        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 32)]
        public string user_name;           //用户名最长32字节

        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 16)]
        public string password;            //密码最长16字节
        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 15)]
        public string server_addr;         //服务器地址
        public uint server_port;           //服务器端口
        public CodecType codec_type;       //协议编码器类型
        public int marktet_open_time;      //开市时间
        public int markt_close_time;       //收市时间
    };



    [StructLayout(LayoutKind.Sequential, Pack = 1)]
    public struct BackupSourceArgs
    {
        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 32)]
        public string user_name;         //用户名最长32字节
        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 16)]
        public string password;          //密码最长16字节
        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 15)]
        public string server_addr;       //服务器地址
        public uint server_port;          //服务器端口
    };



    //初始化API时所需参数
    [StructLayout(LayoutKind.Sequential, Pack = 1)]
    public struct OpenExArgs
    {
        public byte connection_id;         //0-31(最多支持32个连接)
        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 32)]
        public string user_name;          //用户名最长32字节

        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 16)]
        public string password;           //密码最长16字节
        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 15)]
        public string server_addr;        //服务器地址
        public uint server_port;          //服务器端口
        public CodecType codec_type;      //协议编码器类型
        public int marktet_open_time;     //开市时间
        public int markt_close_time;      //收市时间
        public int retry_times;           //切换备源前重连次数
        public int timeout;               //连接超时设置
        public int backup_source_count;   //备源个数
        public IntPtr backup_sources;     //备源配置
    };

    [StructLayout(LayoutKind.Sequential, Pack = 1)]
    public struct CloseArgs
    {
        public byte connection_id;         //0-31(最多支持32个连接)
    };

    [StructLayout(LayoutKind.Sequential, Pack = 1)]
    public struct APIVersionArgs
    {
        public uint major;               //API主版本号
        public uint minor;               //API副版本号
        public uint patch;               //API补丁版本号
    };

    [StructLayout(LayoutKind.Sequential, Pack = 1)]
    public struct ServerInfoArgs
    {
        byte connection_id;               //0-31(最多支持32个连接)
        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 32)]
        public string user_name;          //用户名最长32字节

        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 16)]
        public string password;           //密码最长16字节
        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 15)]
        public string server_addr;        //服务器地址
        public uint server_port;          //服务器端口
    };

    [StructLayout(LayoutKind.Sequential, Pack = 1)]
    public struct FilterArgs
    {
        public byte connection_id;         //0-31(最多支持32个连接)
        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 3000)]
        public string filter_masks;        //windcode过滤字符串,以逗号分隔，如600000.SH,600001.SH
    };

    [StructLayout(LayoutKind.Sequential, Pack = 1)]
    public struct MdfRecord
    {
        public int local_time;           // 本地时间
        public uint packet_num;          // 包号
        public short product_id;         // 产品ID
        public short message_id;         // 消息ID
        public byte major_version;       // 主版本
        public byte minor_version;       // 副版本号
        public IntPtr header;            // 产品包头
        public int record_count;         // 数据记录条数
        public IntPtr body;              // 数据记录
    };

    [StructLayout(LayoutKind.Sequential, Pack = 1)]
    public struct RecordArgs
    {
        public byte connection_id;           //0-31(最多支持32个连接)
        public IntPtr record;                //数据记录 
        public IntPtr state;                 //回调对象句柄
    };

    [StructLayout(LayoutKind.Sequential, Pack = 1)]
    public struct EventArgs
    {
        public byte connection_id;            //0-31(最多支持32个连接)
        public byte level;                    //事件级别 FATAL=0, ERROR=1,WARN=2,INFO=3,DEBUG=4
        public double date_time;              //事件发生的时间
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 60)]
        public byte[] at_where;               //监控点
        public int event_id;                  //事件ID
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 150)]
        public byte[] what_happened;          //事件内容，如 登录 “连接失败“等 
        public IntPtr state;                  //回调对象句柄
    };

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void RecordCallback(ref RecordArgs args);

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void FeedEventCallback(ref EventArgs args);

    [StructLayout(LayoutKind.Sequential, Pack = 1)]
    public struct CallbackArgs
    {
        public byte connection_id;                  //0-31(最多支持32个连接)
        public RecordCallback record_callback;      //数据回调
        public FeedEventCallback event_callback;    //事件回调
        public IntPtr state;
    };

}
