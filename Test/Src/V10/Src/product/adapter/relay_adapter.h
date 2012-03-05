#ifndef RELAY_ADAPTER_H
#define RELAY_ADAPTER_H

#include "product_adapter.h"
#include "net_module.h"


namespace wmdf
{
  class MdfMessage;
  class FeedClient;
  class ProductSource;
  class LoginProxy;
  class ProtocolProcessorController;

  class RelayAdapter:public IProductAdapter
  {
  public:
    RelayAdapter(ProductSource* product_source);
    virtual ~RelayAdapter(void);
    virtual void Start();
    virtual void Stop();
    virtual bool is_connected();
    virtual void OnRecvMsg(Message* msg);
    virtual void OnCloseCallback(EventMsg* args);
    virtual void OnEventCallback(EventMsg* args);
    
  private:
    void ProcessLoginResponse( Message* msg );
    bool Connect(const std::string& server_address, const int32_t& server_port, const int32_t retry_times,int32_t timeout,std::string username,std::string password);
  private:
    FeedClient* data_receiver_;
    LoginProxy* login_proxy_;
    ProtocolProcessorController* protocol_processor_controller_;   //协议解码控制器
  };
}

#endif
