#ifndef LOGIN_PROXY_H
#define LOGIN_PROXY_H

#include "net_module.h"
#include <string>

namespace wmdf
{
  class ProtocolProcessor;
  class ProductSource;
  class FeedClient;
  class MdfMessage;

  class LoginProxy
  {
  public:
    LoginProxy(void);
    virtual ~LoginProxy(void);
    bool RequestLogin(ProductSource* product_source,FeedClient* feed_client,std::string username,std::string password);
    MdfMessage* ResponseLogin(Message* msg);
  private:
    void DoRequestLogin(ProductSource* product_source,FeedClient* feed_client,std::string username,std::string password);
    volatile bool login_flag_; // 登录成功标记
    volatile bool login_responsed_; //是否响应登录请求
    ProtocolProcessor* protocol_processor_;
    
  };
}

#endif
