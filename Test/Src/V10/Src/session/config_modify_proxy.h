#ifndef CONFIG_MODIFY_PROXY_H
#define CONFIG_MODIFY_PROXY_H

#include "net_module.h"
#include <string>
#include "../include/wmdf_internal_api.h"

namespace wmdf
{
  class ProtocolProcessor;
  class ProductSource;
  class FeedClient;
  class MdfMessage;

  class ConfigModifyProxy
  {
  public:
    ConfigModifyProxy(void);
    virtual ~ConfigModifyProxy(void);
    bool ConfigModifyQequest(ProductSource* product_source,FeedClient* feed_client,ConfigInfo* config);
    MdfMessage* ProcessResponse(Message* msg);
  private:
    void DoModifyConfig(ProductSource* product_source,FeedClient* feed_client,ConfigInfo* config);
    volatile bool update_config_responsed_; //配置信息请求是否被响应
    ProtocolProcessor* protocol_processor_;
    
  };
}

#endif
