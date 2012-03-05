#pragma once

#pragma pack(push)
#pragma pack(1)

// windid = 600;
typedef struct BidPriceLevel_3102
{
	BidPriceLevel_3102 *pNext;
	// id=134,decimalPlaces=3;
	double	BidSize;
	// id=132,decimalPlaces=3;
	double BidPx;
}stBidPriceLevel_3102,*ptrBidPriceLevel_3102;

// windid = 601;
typedef struct OfferPriceLevel_3102
{
	OfferPriceLevel_3102 *pNext;
	// id=135,decimalPlaces=3;
	double	OfferSize;
	// id=133,decimalPlaces=3;
	double OfferPx;
}stOfferPriceLevel_3102,*ptrOfferPriceLevel_3102;

// windid = 602;
typedef struct Rankings_3103
{
	Rankings_3103 *pNext;
	// id=10046;
	__int32 Ranking;
	// id=48;
	char SecurityID[32];
	// id=10047,decimalPlaces=3;
	double TotalOrderQty;
	// id=387,decimalPlaces=3;
	double TotalVolumeTraded;
	// id=8504,decimalPlaces=5;
	double TotalValueTraded;
}stRankings_3103,*ptrRankings_3103;

// 3104Ç¶Ì×sequence;
typedef struct TotalValueTradedRankings_3104
{
	TotalValueTradedRankings_3104 *pNext;
	// id=10046;
	__int32 Ranking;
	// id=48;
	char SecurityID[32];
	// id=8504,decimalPlaces=5;
	double TotalValueTraded;
}stTotalValueTradedRankings_3104,*ptrTotalValueTradedRankings_3104;

// windid = 603;
typedef struct Rankings_3104
{
	Rankings_3104 *pNext;
	// id=10050,operation=increment;
	__int32 Ranking;
	// id=10049;
	char IndustryCode[32];
	// id=10053,decimalPlaces=5;
	double IndustryTotalValueTraded;
	ptrTotalValueTradedRankings_3104 pChildSequence;
}stRankings_3104,*ptrRankings_3104;

// windid = 604;
typedef struct Rankings_3105
{
	Rankings_3105 *pNext;
	// id=10046;
	__int32 Ranking;
	// id=48;
	char SecurityID[32];
	// id=8500;
	__int32 OrderEntryTime;
	// id=53,decimalPlaces=3;
	double Quantity;
	// id=44,decimalPlaces=3;
	double Price;
}stRankings_3105,*ptrRankings_3105;

// windid = 605;
typedef struct Rankings_3106
{
	Rankings_3106 *pNext;
	// id=10046;
	__int32 Ranking;
	// id=48;
	char SecurityID[32];
	// id=10054,decimalPlaces=3;
	double TotalWithdrawnQty;
}stRankings_3106,*ptrRankings_3106;

// 3111Ç¶Ì×sequence;
typedef struct NoOrders_3111
{// 3111ÖÐsequenceÄÚÇ¶µÄsequence;
	NoOrders_3111 *pNext;
	// id=10148,present=optional,operation=default;
	__int32 OrderQueueOperator;
	// id=10149,present=optional,operation=default;
	__int32 OrderQueueOperatorEntryID;
	// id=38,presence=optional,decimalPlaces=3,operation=default;
	double OrderQty;
}stNoOrders_3111,*ptrNoOrders_3111;

// windid = 606;
typedef struct PriceLevel_3111
{
	PriceLevel_3111 *pNext;
	// id=10147,presence=optional,operation=default;
	__int32 PriceLevelOperator;
	// id=44,decimalPlace=3;
	double Price;
	// id=10067;
	__int32 NumOrders;
	ptrNoOrders_3111 pChildSequence;
}stPriceLevel_3111,*ptrPriceLevel_3111;

// windid = 607;
typedef struct BidLevels_3112
{
	BidLevels_3112 *pNext;
	// id=10145,presence=optional,operation=default;
	__int32 Operator;
	// id=44,presence=optional,decimalPlaces=3,operation=default;
	double Price;
	// id=38,presence=optional,decimalPlaces=3,operation=default;
	double OrderQty;
	// id=10067,presence=optional,operation=default;
	__int32 NumOrders;
}stBidLevels_3112,*ptrBidLevels_3112;

// windid = 608;
typedef struct OfferLevels_3112
{
	OfferLevels_3112 *pNext;
	// id=10145,presence=optional,operation=default;
	__int32 Operator;
	// id=44,presence=optional,decimalPlaces=3,operation=default;
	double Price;
	// id=38,presence=optional,decimalPlaces=3,operation=default;
	double OrderQty;
	// id=10067,presence=optional,operation=default;
	__int32 NumOrders;
}stOfferLevels_3112,*ptrOfferLevels_3112;

// windid = 609;
typedef struct BidPriceLevel_5102
{
	BidPriceLevel_5102 *pNext;
	// id=134;
	__int64 BidSize;
	// id=132,decimalPlaces=3;
	double BidPx;
}stBidPriceLevel_5102,*ptrBidPriceLevel_5102;

// windid = 610;
typedef struct OfferPriceLevel_5102
{
	OfferPriceLevel_5102 *pNext;
	// id=135;
	__int64 OfferSize;
	// id=133,decimalPlaces=3;
	double OfferPx;
}stOfferPriceLevel_5102,*ptrOfferPriceLevel_5102;

// windid = 611/612;
typedef struct BidOfferLevels_3202
{
	BidOfferLevels_3202 *pNext;
	// id=10147,presence=optional,operation=default;
	__int32 PriceLevelOperator;
	// id=44,decimalPlace=3;
	double Price;
	// id=39,presence=optional,decimalPlace=3;
	double OrderQty;
	// id=10067;
	__int32 NumOrders;
	ptrNoOrders_3111 pChildSequence;
}stBidOfferLevels_3202,*ptrBidOfferLevels_3202;

#pragma pack(pop)