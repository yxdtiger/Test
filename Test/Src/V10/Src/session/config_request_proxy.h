#ifndef CONFIG_REQUEST_PROXY_H
#define CONFIG_REQUEST_PROXY_H

#include "net_module.h"
#include <string>
#include "../include/wmdf_internal_api.h"


namespace wmdf
{
  class ProtocolProcessor;
  class ProductSource;
  class FeedClient;
  class MdfMessage;
  class ConfigRequestProxy
  {
  public:
    ConfigRequestProxy(void);
    virtual ~ConfigRequestProxy(void);
    bool RequestConfigQequest(ProductSource* product_source,FeedClient* feed_client,ConfigFileType type);
    MdfMessage* ProcessResponse(Message* msg);
  private:
    void DoRequestConfig(ProductSource* product_source,FeedClient* feed_client,ConfigFileType type);
    volatile bool config_info_responsed_; //配置信息请求是否被响应
    ProtocolProcessor* protocol_processor_;
    
  };
}

#endif
