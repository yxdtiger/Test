#ifndef SSE_MARKETDATA_3202_MERGER_H
#define SSE_MARKETDATA_3202_MERGER_H

#include <map>
#include <string>
#include <vector>
#include "../../../include/wmdf_structs.h"
#include "../../../environment/utils.h"
#include "../common/rawdata_object.h"

namespace wmdf
{

  typedef enum ePriceOperator
  {
    EPO_ADD = 1,
    EPO_UPDATE = 2,
    EPO_DELETE = 3
  }ePriceOperator;

  typedef enum eImageStatus
  {
    IS_FULL = 1,
    IS_UPDATE = 2
  }eImageStatus;

  typedef struct sPriceLevel
  {
    double price;
    int32_t size;
//     bool operator < (const sPriceLevel& rpl ) const //ÉýÐòÅÅÐòÊ±±ØÐëÐ´µÄº¯Êý
//     { 
//       return price < rpl.price; 
//     }
//     bool operator > (const sPriceLevel& rpl ) const //½µÐòÅÅÐòÊ±±ØÐëÐ´µÄº¯Êý
//     { 
//       return price > rpl.price; 
//     }
  };

  typedef struct MarketDepth
  {
    std::vector<sPriceLevel*> bid_price_levels_;
    std::vector<sPriceLevel*> ask_price_levels_;
  };


  class SSE_MarketData_3202_Merger
  {
  public:
    SSE_MarketData_3202_Merger(void);
    virtual ~SSE_MarketData_3202_Merger(void);
    void ProcessMarketSnapshot(MarketData_3202* marketdata_3202,L2MarketSnapshot* out);
    int32_t order_queue_count() {return order_queue_count_;}
    void set_order_queue_count(int32_t count){order_queue_count_ = count;}
    void Clear();
  private:
    void ProcessMarketSnapshot(MarketData_3202* src,MarketDepth& dst,L2MarketSnapshot* out); 
    void ProcessFullImage(MarketData_3202* src,MarketDepth& dst,L2MarketSnapshot* out);
    void ProcessUpdateImage(MarketData_3202* src,MarketDepth& dst,L2MarketSnapshot* out);
    void ProcessUpdatePriceLevel(PriceLevel_3202* src, std::vector<sPriceLevel*>& dst);
    void ClearBidMarketDepth(MarketDepth& market_depth);
    void ClearAskMarketDepth(MarketDepth& market_depth);
    std::map<std::string,MarketDepth*> market_depths_;
    int32_t order_queue_count_;
    
  };
}

#endif
