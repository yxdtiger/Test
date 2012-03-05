#include "login_proxy.h"
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

wmdf::LoginProxy::LoginProxy(void)
{
  login_responsed_ = false;
  login_flag_ = false;
  ProtocolDescription* pd = new ProtocolDescription(PRODEUCT_SYSTEM_ID, MAJOR_VERSION_SYSTEM_ID,MINOR_VERSION_SYSTEM_ID,CT_BINARY);
  protocol_processor_ = ProtocolProcessorFactory::CreateProtocolCodec(pd);
}

wmdf::LoginProxy::~LoginProxy(void)
{
  if (NULL != protocol_processor_)
  {
    delete protocol_processor_;
    protocol_processor_ = NULL;
  }
}

bool wmdf::LoginProxy::RequestLogin( ProductSource* product_source,FeedClient* feed_client,std::string username,std::string password )
{
  login_responsed_ = false;
  login_flag_ = false;
  DoRequestLogin(product_source,feed_client,username,password);
  int wait_times = 0;
  //waiting for login flag until timeout(30s)
  while ((!login_responsed_)  && (wait_times < 30000))
  {
    wait_times ++;
    Utils::SleepTime(1);
  }
  return login_flag_;  
}

void wmdf::LoginProxy::DoRequestLogin(ProductSource* product_source,FeedClient* feed_client,std::string username,std::string password)
{
  LoginRequest* req = new LoginRequest();
  memset(req,0,sizeof(LoginRequest));
  memcpy(req->user_name,username.c_str(),username.length());
  memcpy(req->password,password.c_str(),password.length());
  req->product_count = product_source->subscribed_products_count();
  req->product = new ProductInfo[req->product_count];
  for (int32_t index = 0; index< product_source->subscribed_products_count(); index++)
  {
    req->product[index].product_id = product_source->subscribed_products(index)->product_id();      
    req->product[index].major_version = product_source->subscribed_products(index)->major_version();
    req->product[index].minor_version = product_source->subscribed_products(index)->minor_version();
    req->product[index].codec = product_source->subscribed_products(index)->codec_type();
  }

  SharedMdfMessage mdf_message(new MdfMessage());
  mdf_message->set_local_time(DateTime::get_current_time());
  mdf_message->set_socket_id(0);
  mdf_message->set_product_id(PRODEUCT_SYSTEM_ID);
  mdf_message->set_message_id(SYSTEM_LOGIN_REQUEST_ID);
  mdf_message->set_major_version(MAJOR_VERSION_SYSTEM_ID);
  mdf_message->set_minor_version(MINOR_VERSION_SYSTEM_ID);
  mdf_message->set_record_count(1);
  mdf_message->set_body(req);
  Message* raw_message = protocol_processor_->Encode(mdf_message);
  feed_client->SendMsg(raw_message);  
}


wmdf::MdfMessage* wmdf::LoginProxy::ResponseLogin( Message* msg )
{
  if ((msg->product_id == PRODEUCT_SYSTEM_ID) 
    && (msg->message_id ==SYSTEM_LOGIN_RESPONSE_ID))
  {
    MdfMessage* mdf_message = protocol_processor_->Decode(msg);
    if (NULL != mdf_message)
    {
      LoginResponse* login_resp = (LoginResponse*)mdf_message->body();
      login_flag_= login_resp->ack_type==AUTHORITY_VALID;
    }
    login_responsed_ = true;
    return mdf_message;
  }
  return NULL;
}
