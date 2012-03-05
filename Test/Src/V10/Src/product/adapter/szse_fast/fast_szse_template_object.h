#pragma once

#include "../../../include/wmdf_api.h"

#define  SZSE_TRANSACTION_STR "UA202"
#define  SZSE_TRANSACTION_TYPE 100


#define  SZSE_INDEX_STR "UA104"
#define  SZSE_INDEX_TYPE 102

#define  SZSE_STOCKINFO_STR "UA101"
#define  SZSE_STOCKINFO_TYPE 103

#define  SZSE_ORDERS_STR "UA201"
#define  SZSE_ORDERS_TYPE 104

#define  SZSE_SNAPSHOT_STR "UA103"
#define  SZSE_SNAPSHOT_TYPE 101

// id=454;
typedef struct SecurityAltIDs_101
{
	// id=455;
	uint8_t SecurityAltID[32];
	// id=456;
	uint8_t SecurityAltIDSource[32];
}stSecurityAltIDs_101,*ptrSecurityAltIDs_101;

// id=136;
typedef struct MiscFees_101
{
	// id=139;
	uint8_t MiscFeeType[32];
	// id=891,operation=copy;
	uint32_t MiscFeeBasis;
	// id=137,decimalPlaces=6;
	double MiscFeeAmt;
}stMiscFees_101,*ptrMiscFees_101;

// id=8522;
typedef struct IndicesParticipated_101
{
	// id=8528;
	uint8_t participatingIndexID[32];
}stIndicesParticipated_101,*ptrIndicesParticipated_101;

// 以上是sequence结构体定义，以下是template结构体定义;
typedef struct SZSEL2_StockInfo_101 
{
  uint32_t RecNo;
  uint8_t	SecurityID[32];
  uint8_t	Symbol[32];
  uint8_t	SecurityDesc[32];
  uint8_t	UnderlyingSecurityID[32];
  uint8_t	SecurityIDSource[32];
  // SecurityAltIDs循环次数;
  int32_t nSecurityAltIDsNum;
  ptrSecurityAltIDs_101 pSecurityAltIDs;
  uint8_t	IndustryClassification[32];
  uint8_t	Currency[32];
  double	ShareFaceValue;
  int64_t	OutstandingShare;
  int64_t	PublicFloatShareQuantity;
  double	PreviousYearProfitPerShare;
  double	CurrentYearProfitPerShare;
  double	NAV;
  // MiscFees循环次数;
  int32_t nMiscFeesNum;
  ptrMiscFees_101	pMiscFees;
  uint32_t	IssueDate;
  uint8_t	BondType[32];
  double	CouponRate;
  double	ConversionPrice;
  uint8_t	AmericanOrEuropean[32];
  uint8_t	CallOrPut[32];
  uint8_t	WarrantClearingType[32];
  double	CVRatio;
  uint32_t	ConversionBeginDate;
  uint32_t	ConversionEndDate;
  uint32_t	InterestAccrualDate;
  uint32_t	MaturityDate;
  uint32_t	RoundLot;
  uint32_t	BidLotSize;
  uint32_t	AskLotSize;
  uint32_t	MaxFloor;
  uint8_t	TradeMethod[32];
  double	PriceTickSize;
  uint8_t	PriceLimitType[32];
  double	AuctionPriceLimit;
  double	ContinuousTradePriceLimit;
  double	DailyPriceUpLimit;
  double	DailyPriceLowLimit;
  double	DailyPriceUpLimit2;
  double	DailyPriceLowLimit2;
  double	ContractMultiplier;
  double	GageRate;
  uint8_t	CrdBuyUnderlying[32];
  uint8_t	CrdSellUnderlying[32];
  // IndicesParticipated循环次数;
  int32_t nIndicesParticipatedNum;
  ptrIndicesParticipated_101	pIndicesParticipated;
  uint8_t	MarketMakerFlag[32];
  uint8_t	SecurityExchange[32];
  uint8_t	CFICode[32];
  uint8_t	SecuritySubType[32];
  uint8_t	SecurityProperties[32];
  uint32_t	SecurityTradingStatus;
  uint8_t	CorporateAction[32];
  uint8_t	TradingMechanism[32];
  uint8_t	CrdPriceCheckType[32];
  uint8_t	NetVotingFlag[32];
  uint8_t	ShrstrurefmFlag[32];
  uint8_t	OfferingFlag[32];
}stTemplate_StockInfo_101,*ptrTemplate_StockInfo_101;

typedef struct SZSEL2_StockStatus_102 
{
  uint8_t	SecurityID[32];
  uint8_t	SecurityPreName[32];
  uint8_t	TradingPhaseCode[32];
  uint8_t	CrdBuyStatus[32];
  uint8_t	CrdSellStatus[32];
  uint8_t	SubScribeStatus[32];
  uint8_t	RedemptionStatus[32];
  uint8_t	WarrantCreated[32];
  uint8_t	WarrantDropped[32];
}stTemplate_StockStatus_102,*ptrTemplate_StockStatus_102;



// 103嵌套sequence;
typedef struct Orders_103
{
	// id=38;
	uint32_t OrderQty;
}stOrders_103,*ptrOrders_103;

// id=10068 / id=10069;
typedef struct BidOfferPriceLevel_103
{
	// id=132 / id=133,decimalPlaces=3;
	double BidorOfferPx;
	// id=134 / id=135;
	int64_t BidorOfferSize;
	// id=10067;
	uint32_t NumOrders;
	// Sequence循环次数;
	int32_t nSequenceLength;
	ptrOrders_103 pChildSequence;
}stBidOfferPriceLevel_103,*ptrBidOfferPriceLevel_103;


typedef struct SZSEL2_Snapshot_103 
{
	int32_t	DataTimeStamp;
	uint8_t	SecurityID[32];
	double	PreClosePx;
	double	OpenPx;
	double	HighPx;
	double	LowPx;
	double	LastPx;
	int64_t	NumTrades;
	int64_t	TotalVolumeTrade;
	double	TotalValueTrade;
	int64_t	TotalLongPosition;
	double	PERatio1;
	double	PERatio2;
	uint8_t	EndOfDayMaker[32];
	int64_t	TotalOfferQty;
	double	WeightedAvgOfferPx;
	// OfferPriceLevel循环次数;
	int32_t nOfferPriceLevelNum;
	ptrBidOfferPriceLevel_103 pOfferPriceLevel;
	int64_t TotalBidQty;
	double	WeightedAvgBidPx;
	// BidPriceLevel循环次数;
	int32_t nBidPriceLevelNum;
	ptrBidOfferPriceLevel_103 pBidPriceLevel;
}stTemplate_Snapshot_103,*ptrTemplate_Snapshot_103;

typedef struct SZSEL2_Index_104 
{
	int32_t	DataTimeStamp;
	uint8_t	SecurityID[32];
	double	CloseIndex;
	double	OpenIndex;
	double	HighIndex;
	double	LowIndex;
	double	LastIndex;
	double	TurnOver;
	int64_t	NumTrades;
	int64_t	TotalVolumeTraded;
	uint8_t	EndOfDayMaker[32];
}stTemplate_Index_104,*ptrTemplate_Index_104;

typedef struct SZSEL2_Order_201 
{
	uint32_t	SetNo;
	uint32_t	RecNo;
	uint8_t	SecurityID[32];
	double	Price;
	uint32_t	OrderQty;
	uint8_t	OrderKind[32];
	uint8_t	FunctionCode[32];
	int32_t	OrderEntryTime;
}stTemplate_Order_201,*ptrTemplate_Order_201;

typedef struct SZSEL2_Trade_202 
{
	uint32_t	SetNo;
	uint32_t	RecNo;
	uint32_t	BuyOrderRecNo;
	uint32_t	SellOrderRecNo;
	uint8_t	SecurityID[32];
	double	Price;
	uint32_t	TradeQty;
	uint8_t	OrderKind[32];
	uint8_t	FunctionCode[32];
	int32_t	TradeTime;
}stTemplate_Trade_202,*ptrTemplate_Trade_202;

