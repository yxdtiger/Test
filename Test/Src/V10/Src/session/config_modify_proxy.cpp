#include "config_modify_proxy.h"
#include "../include/wmdf_structs.h"
#include "../protocol/protocol_processor.h"
#include "../protocol/protocol_processor_factory.h"
#include "../product/product_source.h"
#include "../monitor/logger.h"
#include "../protocol/mdf_message.h"
#include "../include/wmdf_api.h"
#include "../environment/utils.h"
#include "feed_client.h"
#include "../include/date_time.h"

wmdf::ConfigModifyProxy::ConfigModifyProxy(void)
{
  ProtocolDescription* pd = new ProtocolDescription(PRODEUCT_SYSTEM_ID, MAJOR_VERSION_SYSTEM_ID,MINOR_VERSION_SYSTEM_ID,CT_BINARY);
  protocol_processor_ = ProtocolProcessorFactory::CreateProtocolCodec(pd);
  update_config_responsed_ = false;
}

wmdf::ConfigModifyProxy::~ConfigModifyProxy(void)
{
  if (NULL != protocol_processor_)
  {
    delete protocol_processor_;
    protocol_processor_ = NULL;
  }
}

bool wmdf::ConfigModifyProxy::ConfigModifyQequest( ProductSource* product_source,FeedClient* feed_client,ConfigInfo* config )
{
  update_config_responsed_ = false;
  DoModifyConfig(product_source,feed_client,config);
  int wait_times = 0;
  while ((!update_config_responsed_)  && (wait_times < 30000))
  {
    wait_times ++;
    Utils::SleepTime(1);
  }
  return update_config_responsed_;
}

void wmdf::ConfigModifyProxy::DoModifyConfig(ProductSource* product_source,FeedClient* feed_client,ConfigInfo* config )
{
  ConfigInfo* req = (ConfigInfo*)malloc(sizeof(ConfigInfo)+config->length);
  memcpy(req,config,sizeof(ConfigInfo)+config->length);
  SharedMdfMessage mdf_message(new MdfMessage());
  mdf_message->set_local_time(DateTime::get_current_time());
  mdf_message->set_socket_id(0);
  mdf_message->set_product_id(PRODEUCT_SYSTEM_ID);
  mdf_message->set_message_id(SYSTEM_CONFIG_INFO_ID);
  mdf_message->set_major_version(MAJOR_VERSION_SYSTEM_ID);
  mdf_message->set_minor_version(MINOR_VERSION_SYSTEM_ID);
  mdf_message->set_record_count(1);
  mdf_message->set_body(req);
  Message* raw_message = protocol_processor_->Encode(mdf_message);
  feed_client->SendMsg(raw_message);  
}

wmdf::MdfMessage* wmdf::ConfigModifyProxy::ProcessResponse( Message* msg )
{
  MdfMessage* mdf_message = NULL;
  if ((msg->product_id == PRODEUCT_SYSTEM_ID) 
    && (msg->message_id ==SYSTEM_UPDATE_CONFIG_RESPONSE_ID))
  {
    mdf_message = protocol_processor_->Decode(msg);
    if (NULL != mdf_message)
    {
      update_config_responsed_ = true;
    }
  }
  return mdf_message;
}


