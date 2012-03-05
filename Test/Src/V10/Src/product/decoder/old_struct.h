#include <vector>
#include "../../include/wmdf_api.h"

#pragma pack(push)
#pragma pack(1)


struct L2_OrderItem            // 深圳Level2, 逐笔委托
{
  int64_t	OrderNumber;   // 委托编号，单只股票自增长
  int64_t	RecNo;         // 委托索引, TradeIndex
  double	OrderPrice;
  int64_t	OrderQty;      // 委托数量,  股
  int64_t	OrderTime;     // 委托时间,  HHMMSSXX
  int64_t	OrderType;    // 委托类别,  1=委买, 2=委卖, 3=撤单
};

/*

struct L2_Transaction_RevertDataItem
{
  unsigned int	RevertIndex;        // 单比成交编号
  unsigned int	MSTime;             // 成交时间毫秒级
  unsigned char	TradeSide;         // 1=买方主动, 2=卖方主动, 0=方向未知
  unsigned int	TradePrice;         // 逐笔成交价格
  unsigned int	TradeVolume;        // 逐笔成交量，股数
  unsigned int	BidIndexRef;        // 此刻对应委买编号
  unsigned int	BidVol;             // 此刻对应买单大小，股数
  unsigned int	OfferIndexRef;      // 此刻对应委卖编号
  unsigned int	OfferVol;           // 此刻对应卖单大小，股数
  unsigned char	BuyTradeFlag;      // 主买大单连续成交标识 0 = 开始 1= 结束 2= 中间单 3 = 单笔
  unsigned char	SellTradeFlag;     // 主卖大单连续成交标识 0 = 开始 1= 结束 2= 中间单 3 = 单笔
};

typedef std::vector<L2_Transaction_RevertDataItem> L2_Transaction_RevertData;
*/


struct L2_TransactionItem      // Level2逐笔成交
{
  int64_t	TradeNumber;   // 成交编号，从0开始累计
  int64_t	TradeIndex;    // 成交序号，交易所发布
  int64_t	TradeTime;     // 成交时间, HHMMSSXX,   xx 精确为百分之一秒
  double TradePrice;
  int64_t	TradeQty;      // 成交数量
  int64_t TradeMoney;
};

struct L2_OrderQueueItem
{
  unsigned char PriceLevelOperator;			//1-Add,2-Update,3-Delete
  unsigned int  OrderQueueOperatorEntryID;	// Start from 0, No Meaning if
  unsigned int  OrderQty;
};


struct L2_OrderQueue_PriceLevel
{
  unsigned char		PriceLevelOperator;	//1-Add,2-Update,3-Delete
  unsigned int		Price;
  unsigned int		NumOfOrders;		// 当前价位上总委托笔数
  unsigned int		NoOrders;			// 此次发生变化的委托笔数
  L2_OrderQueueItem* Orders;				// size = NoOrders
};


struct L2_OrderQueue
{
  unsigned int  DateTimeStamp;  // 163025
  unsigned char Side;           // 1 = Bid Queue, 2 = Ask Queue, other: Invalid
  unsigned char ImageStatus;    // 1 = FullImage, 2 = Update, other: Invalid
  unsigned int  NoPriceLevel;   // Number of PriceLevelItems, should = PriceLevel_items.size();
  L2_OrderQueue_PriceLevel* PriceLevel;   //
};

typedef union _WnValue
{
  signed char    c;      // signed character
  unsigned char  b;      // unsigned character
  signed short   s;      // signed short
  unsigned short w;      // unsigned short
  signed long    l;      // signed long
  unsigned long  u;      // unsigned int
  long long		   i;	   // __int64
  float          f;      // float
  double         d;      // double
} WnValue;


typedef struct _Old_WnField
{
  unsigned short    nFieldId;      // field ID defined by WIND
  unsigned char    bValueType;    // value type(ref. Part I)
  WnValue Value;         // union to hold the actual value
} Old_WnField;



// 买卖队列
typedef struct _WnItem
{
  unsigned char PriceLevelOperator;
  unsigned int  OrderQueueOperatorEntryID;   // Start from 0, No Meaning if
  unsigned int  OrderQty;
}WnItem;

typedef std::vector<WnItem> L2_OrderQueueItems;

typedef struct _WnPriceLevel
{
  unsigned char      PriceLevelOperator;
  double				Price;
  unsigned int       NumOfOrders;    // 当前价位上总委托笔数
  unsigned int       NoOrders;       // 此次发生变化的委托笔数
  L2_OrderQueueItems Orders;         // size = NoOrders
}WnPriceLevel;

typedef struct _WnOrderQueue
{
  unsigned int  DateTimeStamp;		// 163025
  unsigned char Side;					// 1 = Bid Queue, 2 = Ask Queue, other: Invalid
  unsigned char ImageStatus;			// 1 = FullImage, 2 = Update, other: Invalid
  unsigned int  NoPriceLevel;			// Number of PriceLevelItems, should = PriceLevel_items.size();
  std::vector<WnPriceLevel> PriceLevel;	//
}WnOrderQueue;



#pragma pack(pop)
