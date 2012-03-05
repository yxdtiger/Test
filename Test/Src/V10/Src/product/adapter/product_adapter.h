#ifndef PRODUCT_ADAPTER_H_
#define PRODUCT_ADAPTER_H_


#include "iproduct_adapter.h"

const int8_t PREFIX = 0x55;
const int8_t SUFFIX = 0x16;
const double WMDF_CONSTS_MAX_FILTER_VALUE = 0.5*1.7976931348623158e+308;	//超过此值的double型数据重置为无效值

namespace wmdf
{
  class ProductAdapter: public IProductAdapter
  {
  public:
    ProductAdapter(ProductSource* product_source);
    virtual ~ProductAdapter();
    virtual void Start();
    virtual void Stop();
    virtual bool is_connected();
    virtual void OnRecvMsg(Message* msg);
    virtual void OnCloseCallback(EventMsg* args);
    virtual void OnEventCallback(EventMsg* args);
  protected:
    bool Connect(const std::string& server_address, const int32_t& server_port, const int32_t retry_times,int32_t timeout);
    virtual uint32_t ParseMessage(const uint32_t length)=0;
    int8_t message_buffer_[1024*1024];
    uint32_t buffer_end_;
  private:
    FeedClient* data_receiver_;
  };

}

#endif
