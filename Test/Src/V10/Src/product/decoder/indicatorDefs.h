#ifndef _INDICATORDEFS_H
#define _INDICATORDEFS_H

//#include "level2.h"

const unsigned short IND_TRADEDATE =      0x01;
const unsigned short IND_TRADETIME =      0x02;
const unsigned short IND_NEWVALUE  =      0x03;

const unsigned short IND_PREVCLOSE =      0x04;
const unsigned short IND_TODAYOPEN =      0x05;
const unsigned short IND_TODAYHIGH =      0x06;
const unsigned short IND_TODAYLOW  =      0x07;

const unsigned short IND_TOTALVOLUME =    0x08U;  
const unsigned short IND_DELTAAMOUNT =    0x09U;
const unsigned short IND_DELTAVOLUME =    0x0AU;
const unsigned short IND_TOTALAMOUNT =    0x3BU;    //59

const unsigned short IND_BIDPRICE1  =     0x0B; 
const unsigned short IND_BIDPRICE2  =     0x0C;
const unsigned short IND_BIDPRICE3  =     0x0D;
const unsigned short IND_BIDPRICE4  =     0x0E;
const unsigned short IND_BIDPRICE5  =     0x0F;
const unsigned short IND_BIDPRICE6  =     0x10;
const unsigned short IND_BIDPRICE7  =     0x11;
const unsigned short IND_BIDPRICE8  =     0x12;
const unsigned short IND_BIDPRICE9  =     0x13;
const unsigned short IND_BIDPRICE10 =     0x14;

const unsigned short IND_ASKPRICE1  =     0x15;
const unsigned short IND_ASKPRICE2  =     0x16;
const unsigned short IND_ASKPRICE3  =     0x17;
const unsigned short IND_ASKPRICE4  =     0x18;
const unsigned short IND_ASKPRICE5  =     0x19;
const unsigned short IND_ASKPRICE6  =     0x1A;
const unsigned short IND_ASKPRICE7  =     0x1B;
const unsigned short IND_ASKPRICE8  =     0x1C;
const unsigned short IND_ASKPRICE9  =     0x1D;
const unsigned short IND_ASKPRICE10 =     0x1E;

const unsigned short IND_BIDVOLUME1   =     0x1FU;
const unsigned short IND_BIDVOLUME2   =     0x20U;
const unsigned short IND_BIDVOLUME3   =     0x21U;
const unsigned short IND_BIDVOLUME4   =     0x22U;
const unsigned short IND_BIDVOLUME5   =     0x23U;
const unsigned short IND_BIDVOLUME6   =     0x24U;
const unsigned short IND_BIDVOLUME7   =     0x25U;
const unsigned short IND_BIDVOLUME8   =     0x26U;
const unsigned short IND_BIDVOLUME9   =     0x27U;
const unsigned short IND_BIDVOLUME10  =     0x28U;

const unsigned short IND_ASKVOLUME1   =     0x29U;
const unsigned short IND_ASKVOLUME2   =     0x2AU;
const unsigned short IND_ASKVOLUME3   =     0x2BU;
const unsigned short IND_ASKVOLUME4   =     0x2CU;
const unsigned short IND_ASKVOLUME5   =     0x2DU;
const unsigned short IND_ASKVOLUME6   =     0x2EU;
const unsigned short IND_ASKVOLUME7   =     0x2FU;
const unsigned short IND_ASKVOLUME8   =     0x30U;
const unsigned short IND_ASKVOLUME9   =     0x31U;
const unsigned short IND_ASKVOLUME10  =     0x32U;

const unsigned short IND_BESTASK      =     0x33;           // 最优卖
const unsigned short IND_BESTBID      =     0x34;           // 最优买
const unsigned short IND_SPREADPRICE  =     0x35;           // 买卖价差

const unsigned short IND_TOTALBIDVOLUME =   0x36;           // 总买盘
const unsigned short IND_TOTALASKVOLUME =   0x37;           // 总卖盘
const unsigned short IND_DEALDIRECTION  =   0x38;           // 成交方向

const unsigned short IND_BROKERQUEUEASK  =  0x39;           // 卖盘经纪商队列
const unsigned short IND_BROKERQUEUEBID  =  0x3A;           // 买盘经纪商队列

const unsigned short IND_BIDORDERS1    =    0x3D;
const unsigned short IND_BIDORDERS2    =    0x3E;
const unsigned short IND_BIDORDERS3    =    0x3F;
const unsigned short IND_BIDORDERS4    =    0x40;
const unsigned short IND_BIDORDERS5    =    0x41;
const unsigned short IND_ASKORDERS1    =    0x42;
const unsigned short IND_ASKORDERS2    =    0x43;
const unsigned short IND_ASKORDERS3    =    0x44;
const unsigned short IND_ASKORDERS4    =    0x45;
const unsigned short IND_ASKORDERS5    =    0x46;

const unsigned short IND_TRANSACTIONS_COUNT  =  0x47;   //71   成交总笔数
const unsigned short IND_LOTSIZE       =    0x48;
const unsigned short IND_DEALPRICE     =    0x49;

const unsigned short IND_SETTLE         =   0x4A;   // 结算价
const unsigned short IND_PREVSETTLE     =   0x4B;   // 前结算价
const unsigned short IND_POSITION       =   0x4C;   // 持仓量
const unsigned short IND_PREVPOSITION   =   0x4D;   // 前持仓量
const unsigned short IND_POSITIONCHANGE =   0x4E;   // 持仓量变化

const unsigned short IND_AVERAGEPRICE    =  0x4F;   // 均价
const unsigned short IND_CHANGE          =  0x50;   // 涨跌
const unsigned short IND_CHANGERANGE     =  0x51;   // 涨跌幅
const unsigned short IND_CHANGERANGE5MIN =  0x52;   // 5分钟涨跌幅

const unsigned short IND_LATEST_VALUE    =  0x54;   // 最近价格，停牌显示最后交易价格

//过滤掉
const unsigned short IND_L2_TRANSCATION_REVERT       =  0x59;   // Level2逐笔成交还原, 89

//逐笔成交（沪深都有）
const unsigned short IND_L2_TRANSACTIONS             =  0x5A;   // Level2, 逐笔成交序列, 90

//买卖队列指标：
const unsigned short IND_L2_ORDER_QUEUE_OFFER        =  0x58;   // 88 深交所买卖队列 Side=2
const unsigned short IND_L2_ORDER_QUEUE              =  0x5B;   // 91 买卖队列（沪深都有 沪市Side=1/2，深市Side=1） 最优买卖价上前50名队列,   

const unsigned short IND_CURRENT_TRANSACTIONS_COUNT  =  0x5C;   // 现手成交笔数, SZSE

//逐笔委托（上交所没有、深交所有）
const unsigned short IND_L2_ORDERS                   =	0x5D;

// formalized indicators
const unsigned short IND_PROTO_INDEX     =  0x70;
const unsigned short IND_COUNT           =  0x71;
const unsigned short IND_DELTA_NEWVALUE  =  0x72;  // 和最新价之间的差值
const unsigned short IND_DELTA_PREV      =  0x73;  // 和前一个值之间的差值
const unsigned short IND_DELTA_DAYS      =  0x74;  // 和前一个日期之间的天数差

const unsigned short IND_WINDCODE        =  0x77U; // WindCode

const unsigned short IND_START_FLAG      =  0x78U;
const unsigned short IND_END_FLAG        =  0x79U;

const unsigned short IND_INITDATA        =  0x7DU;    // 初始化标价
const unsigned short IND_MODIFYTIME      =  0x7FU;    //127

const unsigned short IND_PRICEUNIT       =  0x88U;   
const unsigned short IND_CURRENCY        =  0x89U;

const unsigned short IND_MARKETTIME_OPEN        =  0x8AU;
const unsigned short IND_MARKETTIME_SUSPEND     =  0x8BU;
const unsigned short IND_MARKETTIME_RESUME      =  0x8CU;
const unsigned short IND_MARKETTIME_CLOSE       =  0x8DU;
const unsigned short IND_MARKETTIME_PRE         =  0x8EU;
const unsigned short IND_MARKETTIME_AFTER       =  0x8FU;

// 静态及计算指标

const unsigned short IND_STATCALC           =  0x9F;    // 159

// 特殊指标

const unsigned short IND_SHORTNAME_CHS      =  0x83;    // 131，简称(简体中文)
const unsigned short IND_SHORTNAME_CHT      =  0x84;    // 132，简称(繁体中文)
const unsigned short IND_SHORTNAME_ENG      =  0x85;    // 133, 简称(英文)

const unsigned short IND_QEDATA_SERIES      =  0x95;    // 149

// for ind_qedata_series
const unsigned short IND_QEDATA_INDEX_CIRCULATIONS        =  0x01;
const unsigned short IND_QEDATA_INDEX_CAPITALCIRCULATION  =  0x02;
const unsigned short IND_QEDATA_INDEX_NETPROFIT           =  0x03;
const unsigned short IND_QEDATA_INDEX_NETVALUEPERSHARE    =  0x04;




// hqaf

const unsigned short IND_HQAF               =  0x96;    // 150
const unsigned short IND_HQAF_TODAY         =  0x97;    // 151

const unsigned short IND_CIRCULATIONS       =  0x98;    // 152

const unsigned short IND_L2_TRANSACTIONS_TOTALQTY   =  0x9B;  // 逐笔成交累计成交量

// stock related
const unsigned short IND_PREVCLOSE5         =  0xA0;    // 160
const unsigned short IND_PREVCLOSE10        =  0xA1;
const unsigned short IND_PREVCLOSE20        =  0xA2;
const unsigned short IND_PREVCLOSE60        =  0xA3;
const unsigned short IND_PREVCLOSE120       =  0xA4;
const unsigned short IND_PREVCLOSE250       =  0xA5;
const unsigned short IND_PREVCLOSELASTYEAR  =  0xA6;
const unsigned short IND_IPOPRICE           =  0xA7;
const unsigned short IND_IPODATE            =  0xA8;
const unsigned short IND_LAST5AV            =  0xA9;
const unsigned short IND_LAST30AV           =  0xAA;

const unsigned short IND_LISTEDSTOCK        =  0xAB;
const unsigned short IND_CAPITALSTOCK       =  0xAC;
const unsigned short IND_NETPROFIT          =  0xAD;

const unsigned short IND_EPS_TTM            =  0xAE;
const unsigned short IND_EPS_LYR            =  0xAF;
const unsigned short IND_EPS_PROJECTED      =  0xB0;

const unsigned short IND_NETASSET           =  0xB1;
const unsigned short IND_NETASSETPERSHARE   =  0xB2;
const unsigned short IND_PB                 =  0xB5;

const unsigned short IND_RATINGS            =  0xB8;
const unsigned short IND_RATINGSINST        =  0xB9;

const unsigned short IND_INDUSTRY           =  0xB7;

const unsigned short IND_CHANGEHANDRATE     =  0xBB;
const unsigned short IND_LIANGBI            =  0xBC;
const unsigned short IND_WEIBI              =  0xBD;
const unsigned short IND_FLUCTUATION        =  0xBE;

const unsigned short IND_RANGE5             =  0xBF;
const unsigned short IND_RANGE10            =  0xC0;
const unsigned short IND_RANGE20            =  0xC1;
const unsigned short IND_RANGE60            =  0xC2;
const unsigned short IND_RANGE120           =  0xC3;
const unsigned short IND_RANGE250           =  0xC4;
const unsigned short IND_RANGELASTYEAR      =  0xC5;

const unsigned short IND_CAPITALMARKETVALUE =  0xC6;
const unsigned short IND_LISTEDMARKETVALUE  =  0xC7;

const unsigned short IND_HIGH52WEEK         =  0xC8;
const unsigned short IND_LOW52WEEK          =  0xC9;
const unsigned short IND_SUSPENSIONFLAG     =  0xCA;
const unsigned short IND_RANGEAFFACTOR      =  0xCB;

const unsigned short IND_PE_TTM             =  0xCD;
const unsigned short IND_PE_LYR             =  0xCE;
const unsigned short IND_PE_PROJECTED       =  0xCF;

const unsigned short IND_HIGHLIMIT          =  0xD3;
const unsigned short IND_LOWLIMIT           =  0xD4;

const unsigned short IND_INDEX_UP_TOTAL     =  0xD9;   // 指数成分上涨加数
const unsigned short IND_INDEX_DOWN_TOTAL   =  0xDA;   // 指数成分下跌加数
const unsigned short IND_INDEX_SAME_TOTAL   =  0xDB;   // 指数成分平盘加数

// fund related indicators
const unsigned short IND_NETVALUE              =  0xE6;   // 最新净值
const unsigned short IND_FORWARDDISCOUNT       =  0xE7;   // 贴水率
const unsigned short IND_SHARES                =  0xE8;   // 发行份额

const unsigned short IND_GROWTHRATETHISYEAR    =  0xEB;
const unsigned short IND_GROWTHRATEFROMDAY1    =  0xEC;
const unsigned short IND_GROWTHRATELASTWEEK    =  0XED;
const unsigned short IND_GROWTHRATELASTMONTH   =  0xEE;
const unsigned short IND_GROWTHRATELASTSEASON  =  0xEF;
const unsigned short IND_GROWTHRATELAST6MONTHS =  0xF0;
const unsigned short IND_GROWTHRATELASTYEAR    =  0xF1;
const unsigned short IND_AVERAGEYEARLYYIELD    =  0xF2;
const unsigned short IND_ACCUMULATEDNETVALUE   =  0xF3;
const unsigned short IND_ACCUMULATEDBONUS      =  0xF4;
const unsigned short IND_MANAGEMENTCOMPANY     =  0xF5;
const unsigned short IND_LASTIOPV              =  0xF6;

// bond related
const unsigned short IND_FP_NEWPRICE           =  0x101;
const unsigned short IND_NP_NEWPRICE           =  0x102;
const unsigned short IND_YTM_NEWPRICE          =  0x103;
const unsigned short IND_ACCRUEDINTEREST       =  0x104;
const unsigned short IND_YTM                   =  0x105;          //261
const unsigned short IND_REMAININGYEARS        =  0x106;
const unsigned short IND_MACAULAYDURATION      =  0x107;
const unsigned short IND_MODIFIEDDURATION      =  0x108;
const unsigned short IND_CONVEXITY             =  0x109;
const unsigned short IND_LONGTERMRATING        =  0x10A;
const unsigned short IND_SHORTTERMRATING       =  0x10B;
const unsigned short IND_MARKET                =  0x10C;

// convertible bond related
const unsigned short IND_CONVERSIONPRICE       =  0x10E;
const unsigned short IND_CONVERSIONRATIO       =  0x10F;
const unsigned short IND_CONVERSIONVALUE       =  0x110;
const unsigned short IND_OVERLOWRATIO          =  0x111;	
const unsigned short IND_ARBITRAGESPACE        =  0x112;
const unsigned short IND_REMAININGCIRCULATION  =  0x113;

// warrant related
const unsigned short IND_THEORETICALPRICE      =  0x118;
const unsigned short IND_IMPLIEDVOLATILITY     =  0x119;
const unsigned short IND_EXERCISEPRICE         =  0x11A;
const unsigned short IND_INTRINSICVALUE        =  0x11B;
const unsigned short IND_TIMEVALUE             =  0x11C;
const unsigned short IND_WARRANTOVERLOWRATIO   =  0x11D;
const unsigned short IND_INOUT                 =  0x11E;
const unsigned short IND_LEVELTIMES            =  0x11F;
const unsigned short IND_ACTUALLEVELTIMES      =  0x120;
const unsigned short IND_DELTA                 =  0x121;
const unsigned short IND_GAMA                  =  0x122;
const unsigned short IND_VEGA                  =  0x123;
const unsigned short IND_THETA                 =  0x124;
const unsigned short IND_RHO                   =  0x125;
const unsigned short IND_WARRANTTYPE           =  0x126;
const unsigned short IND_WARRANTEXERCISETYPE   =  0x127;
const unsigned short IND_WARRANTEXERCISEWAY    =  0x128;
const unsigned short IND_WARRANTEXERCISERATIO  =  0x129;
const unsigned short IND_WARRANTEXERCISEDATE   =  0x12A;
const unsigned short IND_WARRANTSTOCKPRICE     =  0x12B;
const unsigned short IND_WARRANTSTOCKRANGE     =  0x12C;
const unsigned short IND_REMAININGDAYS         =  0x12D;

const unsigned short IND_VBID_TIME             =  0x140;
const unsigned short IND_VASK_TIME             =  0x141;

const unsigned short IND_L2_STREAMPACKET	     =  0x146;    //326

const unsigned short IND_L2_TOTALBIDQTY        =  0x154;  // 委托买入总量

const unsigned short IND_L2_TOTALOFFERQTY	     =  0x155;  // 委托卖出总量

const unsigned short IND_STREAM				         =  0x15A;
const unsigned short IND_L2_MONEYFLOW_NETIN_RATIO = 0x15D;

const unsigned short IND_MAXIMUM_NUM           =  0x300;



const unsigned short IND_TRADING_PHRASE_CODE = 0x93;     //147   证券竞价状态
const unsigned short IND_SECURITY_PRE_NAME   = 0x94;     //148   证券简称前缀

const unsigned short IND_IOPV                = 0xFD;      //253
const unsigned short IND_WEIGHTED_AVERAGE_BEST_BID = 86;    //加权平均委买价格
const unsigned short IND_WEIGHTED_AVERAGE_BEST_ASK = 87;     //加权平均委卖价格

#endif  // _INDICATORDEFS_H
