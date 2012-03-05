#ifndef _FeedHandler_Protocol_Define_Header_20110429_
#define _FeedHandler_Protocol_Define_Header_20110429_

#ifdef _WINDOWS

#else	//Linux
typedef int __int32;
typedef long long __int64;
#endif

#pragma once

#include <vector>

#pragma pack(push)  // push current alignment to stack
#pragma pack(1)		// force compiler to use byte alligned structures

// 指标类型定义(Part I)
#define WN_VAL_TYPE_NULL              0     // undefined type
#define WN_VAL_TYPE_STRUCT			  1		// structure data block
#define WN_VAL_TYPE_UINT32			  2		// unsigned int32
#define WN_VAL_TYPE_INT32			  3		// int32
#define WN_VAL_TYPE_UINT64			  4		// unsigned int64
#define WN_VAL_TYPE_INT64			  5		// int64
#define WN_VAL_TYPE_DOUBLE			  6		// double
#define WN_VAL_TYPE_STRING			  7		// string


// 结构体类型定义(Part II)
enum SequenceTypeFlag
{
	SEQUENCE_VALUE_NULL = 0,
	// 以下为上交所L2结构体类型
	SEQUENCE_BidPriceLevel_3102,
	SEQUENCE_OfferPriceLevel_3102,
	SEQUENCE_Rankings_3103,
	SEQUENCE_Rankings_3104,
	SEQUENCE_Rankings_3105,
	SEQUENCE_Rankings_3106,
	SEQUENCE_PriceLevel_3111,
	SEQUENCE_BidLevels_3112,
	SEQUENCE_OfferLevels_3112,
	SEQUENCE_BidPriceLevel_5102,
	SEQUENCE_OfferPriceLevel_5102,
	SEQUENCE_BidOfferLevels_3202,
	// 以下为深交所L2结构体类型
	SEQUENCE_SecurityAltIDs_101,
	SEQUENCE_MiscFees_101,
	SEQUENCE_IndicesParticipated_101,
	SEQUENCE_BidOfferPriceLevel_103
};

typedef struct tagPacketHeader
{
	unsigned int unPacketSize;		//数据包大小
	unsigned char ucWindCode[16];	//Wind Code
	unsigned int unIndexNum;		//指标数量
}PacketHeader,*ptrPacketHeader;

/*
// 复杂指标（结构体）的数据头
typedef struct _WnStructHead
{
	unsigned int	unDataLen;		//复杂数据长度（不包含结构体自身长度）
	unsigned char	ucStructType;	//结构类型(ref. Part II)
	unsigned short	usElementNum;	//元素个数
}WnStHead,*ptrWnStHead;

typedef struct _WnStruct
{
	ptrWnStHead		ptrStHead;		//复杂指标管理包头
	unsigned char *	pBinary;		//复杂指标数据流
}WnStruct,*ptrWnStruct;
*/

typedef struct _WnStruct
{
	unsigned char	ucStructType;	//结构类型(ref. Part II)
	void*  pStructData;
}WnStruct,*ptrWnStruct;

typedef struct _WnString
{
	unsigned int	unStringLen;
	unsigned char * pString;		//复杂指标数据流
}WnString,*ptrWnString;

// union to hold a value of several data type
typedef union _WnValue
{
	unsigned __int32	ui32;			// unsigned int32
	unsigned __int64	ui64;			// unsigned int64
	__int64				i64;			// __int64
	__int32				i32;			// __int32
	double				d;				// double

	ptrWnStruct	ptrStruct;			// 复杂指标数据流
	ptrWnString ptrString;			// 字符串

} WnValue,*ptrWnValue;

//存放指标的结构体
typedef struct _WnField
{
	unsigned short	usFieldId;		// field ID defined by FeedHandler Group
	unsigned char   ucValueType;	// value type(ref. Part I)
	ptrWnValue		ptrValue;		// union to hold the actual value
} WnField,*ptrWnField;

typedef struct _SecurityData
{
	char szWindCode[16];			//Wind Code
	std::vector<ptrWnField> vecField;	//所有指标
}WnSecurityData,*ptrWnSecurityData;

typedef std::vector<ptrWnSecurityData> vecFeedHandlerData;

typedef struct tagFeedHandlerHeader
{
	//固定部分
	unsigned char	ucPrefix[2];	//开始符号0x5555
	unsigned char	ucVersion;		//版本信息 最高位表示是否压缩(0-未压缩；1-压缩)

	//与版本相关部分
	unsigned int	unLocalTime;	//本机时间（HHMMSSXXX，精确到ms）
	unsigned int	unDataTimeStamp;//数据包时间戳（HHMMSSXXX，精确到ms）
	unsigned int	unPacketNo;		//数据包编号[1..M]
	unsigned char	ucMachineNo;	//行情源所在的机器编号
	unsigned char	ucChannel;		//行情源通道编号[1..N]
	unsigned int	unCodeTotal;	//Wind证券代码数量

	unsigned char	ucCheckSum;		//校验位
	unsigned int	unDataLen;		//数据部分长度（纯包体部分）
}FH_HEADER;

typedef struct tagFeedHandlerTail
{
	unsigned char	ucSuffix[2];		//结束符号0x1616
}FH_TAIL;

enum ENUM_COMMAND
{
	E_WN_SET_COMPRESS_FLAG = 1, //设置压缩标志
};

typedef struct tagFHCommand
{
	int nCommand;   
	int nContent;
}FH_COMMAND;



//新添加定义放在该行前面!!!
#pragma pack(pop)

#endif
