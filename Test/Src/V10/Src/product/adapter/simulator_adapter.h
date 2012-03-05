#ifndef SIMULATOR_ADAPTER_H
#define SIMULATOR_ADAPTER_H

#include <vector>
#include <map>
#include <string>
#include "iproduct_adapter.h"
#include "../../include/wmdf_structs.h"
#include <boost/thread.hpp>


namespace wmdf
{
  class MdfMessage;

  class SimulatorAdapter: public IProductAdapter
  {
  public:
    SimulatorAdapter(ProductSource* product_source);
    virtual ~SimulatorAdapter(void);
    virtual void Start();
    virtual void Stop();
    virtual bool is_connected();
    void InitData();
  private:
    void ProduceMDFMessage(); 
    void ProcessTrans(L2Transaction* trans);
    void GenerateMarketData();
    void GenerateTransData();
    void GenerateOrderQueueData();
    void GenerateFullImageOrderQueue(L2OrderQueue* orderQueue);
    void GenerateUpdateOrderQueue(L2OrderQueue* orderQueue);
    void GenerateAddDeleteOrderQueue(L2OrderQueue* orderQueue);
    
    boost::thread* work_thread_;
    std::vector<std::string> codes;
    std::map<std::string,L2MarketSnapshot*> markets;
  };
}

#endif
