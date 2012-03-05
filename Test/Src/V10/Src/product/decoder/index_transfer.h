#ifndef INDEX_TRANSFER_H
#define INDEX_TRANSFER_H

#include "../../include/wmdf_structs.h"
#include "old_struct.h"
#include "../../include/wmdf_api.h"
#include <vector>


namespace wmdf
{
#define WMDF_VAL_TYPE_NULL              0     // undefined type
#define WMDF_VAL_TYPE_CHAR              1     // signed character
#define WMDF_VAL_TYPE_BYTE              2     // unsigned character
#define WMDF_VAL_TYPE_SHORT             3     // signed short (2 bytes)
#define WMDF_VAL_TYPE_WORD              4     // unsigned short (2 bytes)
#define WMDF_VAL_TYPE_INT               5     // signed integer (4 bytes)
#define WMDF_VAL_TYPE_UINT              6     // unsigned integer (4 bytes)
#define WMDF_VAL_TYPE_LONG              7     // signed long
#define WMDF_VAL_TYPE_ULONG             8     // unsigned long
#define WMDF_VAL_TYPE_FLOAT             9     // float
#define WMDF_VAL_TYPE_UINT64			      10
#define WMDF_VAL_TYPE_DOUBLE            11    // double
  //#define WN_VAL_TYPE_STRING            12    // pointer to zero terminated string
  //#define WN_VAL_TYPE_FRAC              13    // EAG fractional representation (see below)
  //#define WN_VAL_TYPE_BINARY            14    // binary data block
#define WMDF_VAL_TYPE_STRUCT			      15	// structure data block

#define WMDF_VAL_TYPE_IGNORE			      16	//暂时不处理的指标--Debug only

/*
 * definitions for the different value types(Part II)
 */
#define WN_ST_TYPE_NULL						0		// undefined type
#define WN_ST_TYPE_BROKERASK				1		// Broker Queue Ask
#define WN_ST_TYPE_BROKERBID				2		// Broker Queue Bid
#define WN_ST_TYPE_L2_TRANSACTIONS			3		//逐笔成交
#define WN_ST_TYPE_L2_ORDERS				4		//逐笔委托
#define WN_ST_TYPE_L2_ORDER_QUEUE			5		//买卖队列2
#define WN_ST_TYPE_L2_ORDER_QUEUE_OFFER     6		//买卖队列

#define WN_ST_TYPE_SECURITY_SHORT_NAME		7		//证券代码简称

  class IndexTransfer
  {
  public:
    IndexTransfer();
    virtual ~IndexTransfer();
    void GetMarketSnapShot(L2MarketSnapshot* marketsnapshot);
    void GetL2OrderQueue(L2OrderQueue* oq );
    void GetL2Transactions(L2Transaction* transaction);
    void GetSZSEL2Orders(SZSEL2Order* orders);
    void GetStreamData();
  private:
    uint8_t windcode_[16];
    std::vector<Old_WnField*> wnfields_;
    WnOrderQueue* orderqueue_;
    L2_TransactionItem* transaction_;
    L2_OrderItem* orderitem_;
  };
}

#endif
