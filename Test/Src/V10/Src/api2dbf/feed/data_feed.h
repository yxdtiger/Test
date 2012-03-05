#ifndef DATA_FEED_H
#define DATA_FEED_H
#include "../../include/wmdf_api.h"
#include <map>


namespace wmdf
{
  class DataFeed
  {
  public:
    DataFeed(void);
    virtual ~DataFeed(void);
    void Process(MdfRecord* record);    
  private:
    void ProcessSSEL1MarketData(MdfRecord* record);
    void ProcessSZSEL1MarketData(MdfRecord* record);
    void ProcessSSEL2MarketData(MdfRecord* record);
    void ProcessSSEL2Transaction(MdfRecord* record);
    void ProcessSSEL2IndexData(MdfRecord* record);
    void ProcessSZSEL2MarketData(MdfRecord* record);
    void ProcessSZSEL2Transaction(MdfRecord* record);
    void ProcessIndexSnapshot(MdfRecord* record);
    void ProcessCFFEL1MarketData(MdfRecord* record);
    void ProcessDCEMarketData(MdfRecord* record);
    void ProcessSHFEMarketData(MdfRecord* record);
    void ProcessCZCEMarketData(MdfRecord* record);
    void ProcessCFFEL2MarketData(MdfRecord* record);
    void ProcessSZSEL2Order(MdfRecord* record);
    
    
  private:
    bool IsProductExistedInIdMap(int16_t product_id);

  private:
    
    //用来校验单个产品的packetid是否连续。
    std::map<int16_t,uint32_t> packet_id_map_;
  };



}

#endif
