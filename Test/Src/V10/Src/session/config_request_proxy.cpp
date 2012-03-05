#include "config_request_proxy.h"
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

wmdf::ConfigRequestProxy::ConfigRequestProxy(void)
{
  config_info_responsed_ = false;
  ProtocolDescription* pd = new ProtocolDescription(PRODEUCT_SYSTEM_ID, MAJOR_VERSION_SYSTEM_ID,MINOR_VERSION_SYSTEM_ID,CT_BINARY);
  protocol_processor_ = ProtocolProcessorFactory::CreateProtocolCodec(pd);
}

wmdf::ConfigRequestProxy::~ConfigRequestProxy(void)
{
  if (NULL != protocol_processor_)
  {
    delete protocol_processor_;
    protocol_processor_ = NULL;
  }
}

bool wmdf::ConfigRequestProxy::RequestConfigQequest( ProductSource* product_source,FeedClient* feed_client,ConfigFileType type )
{
  config_info_responsed_ = false;
  DoRequestConfig(product_source,feed_client,type);
  int wait_times = 0;
  //waiting for login flag until timeout(30s)
  while ((!config_info_responsed_)  && (wait_times < 30000))
  {
    wait_times ++;
    Utils::SleepTime(1);
  }
  return config_info_responsed_;
}

void wmdf::ConfigRequestProxy::DoRequestConfig(ProductSource* product_source,FeedClient* feed_client,ConfigFileType type)
{
  ConfigInfoRequset* req = new ConfigInfoRequset();
  memset(req,0,sizeof(ConfigInfoRequset));
  req->config_type = type;
  SharedMdfMessage mdf_message(new MdfMessage());
  mdf_message->set_local_time(DateTime::get_current_time());
  mdf_message->set_socket_id(0);
  mdf_message->set_product_id(PRODEUCT_SYSTEM_ID);
  mdf_message->set_message_id(SYSTEM_CONFIG_INFO_REQUEST_ID);
  mdf_message->set_major_version(MAJOR_VERSION_SYSTEM_ID);
  mdf_message->set_minor_version(MINOR_VERSION_SYSTEM_ID);
  mdf_message->set_record_count(1);
  mdf_message->set_body(req);
  Message* raw_message = protocol_processor_->Encode(mdf_message);
  feed_client->SendMsg(raw_message);  
}


wmdf::MdfMessage* wmdf::ConfigRequestProxy::ProcessResponse( Message* msg )
{
  MdfMessage* mdf_message = NULL;
  if ((msg->product_id == PRODEUCT_SYSTEM_ID) 
    && (msg->message_id ==SYSTEM_CONFIG_INFO_ID))
  {
    mdf_message = protocol_processor_->Decode(msg);
    if (NULL != mdf_message)
    {
      config_info_responsed_ = true;
    }
  }
  return mdf_message;
}
