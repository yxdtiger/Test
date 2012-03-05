#include "system_message_handler.h"
#include "../environment/server_environment.h"
#include "../environment/application_directory.h"
#include "../include/wmdf_api.h"
#include "../include/wmdf_structs.h"
#include "../include/wmdf_internal_api.h"
#include "../usermanager/user.h"
#include "../usermanager/authenticator.h"
#include "../usermanager/authorized_product.h"
#include "../usermanager/user_manager.h"
#include "../include/date_time.h"
#include "feed_server.h"
#include "subscriber.h"
#include "subscriber_manager.h"
#include "../monitor/logger.h"
#include "../protocol/protocol_processor.h"
#include "../environment/utils.h"
#include "../product/product_manager.h"
#include "../product/product_source_manager.h"
#include "../environment/system_config.h"


wmdf::SystemMessageHandler::SystemMessageHandler(void)
{
}

wmdf::SystemMessageHandler::~SystemMessageHandler(void)
{
}

wmdf::SharedMdfMessage wmdf::SystemMessageHandler::CreateMdfMessage(SharedMdfMessage message, void* record, int16_t message_id)
{
  SharedMdfMessage mdf_message;
  mdf_message = SharedMdfMessage(new MdfMessage());
  mdf_message->set_local_time(DateTime::get_current_time());
  mdf_message->set_major_version(message->major_version());
  mdf_message->set_minor_version(message->minor_version());
  mdf_message->set_socket_id(message->socket_id());
  mdf_message->set_product_id(message->product_id());
  mdf_message->set_message_id(message_id);
  mdf_message->set_header(NULL);  //need not set the header,it will be encode in protocol processor;
  mdf_message->set_record_count(1);
  mdf_message->set_body(record);  
  return mdf_message;
}

void wmdf::SystemMessageHandler::SendResponseMessage( SharedMdfMessage mdf_message, ProtocolProcessor* processor, FeedServer* feed_server )
{
  if (NULL != mdf_message)
  {
    //这个Message 不用释放，由网络模块释放。
    Message* net_message = processor->Encode(mdf_message);
    feed_server->SendMsg(mdf_message->socket_id(),net_message);
    Utils::SleepTime(500);  //等待500ms在发送数据
  }
}

wmdf::LoginRequest_V_1_0_Handle::LoginRequest_V_1_0_Handle( void )
{

}

wmdf::LoginRequest_V_1_0_Handle::~LoginRequest_V_1_0_Handle( void )
{

}

void wmdf::LoginRequest_V_1_0_Handle::ProcessMessage(SharedMdfMessage message,FeedServer* feed_server,ProtocolProcessor* processor)
{
  
  LoginRequest* request = (LoginRequest*)message->body();
  LoginResponse* response = new LoginResponse();

  string client_ip = feed_server->subscriber_manager()->GetIpAddress(message->socket_id());
  eReturnValue ret_code = ServerEnvironment::authenticator()->Check(message,client_ip);

  if(ret_code==AUTHORITY_VALID)
  {
    string str((char*)request->user_name);
    
    User* user = ServerEnvironment::user_manager()->FindUser(str);
    response->ack_type=AUTHORITY_VALID;
    response->user_id = user->user_id();
    response->server_count = 1;//todo(xdyuan) 根据服务端信息创建服务器列表
    response->serverinfo=new ServerInfo();
    string ipaddr("10.100.1.100");
    memcpy(response->serverinfo->server_addr,ipaddr.c_str(),ipaddr.length());
    response->serverinfo->server_port = 6000;
    response->product_count = request->product_count;
    if (response->product_count == 0)
    {
      response->ack_type = AUTHORITY_INVALID;
      response->product_status = NULL;
    }
    else
    {
      response->product_status = new ProductStatus[request->product_count];
      for(int i=0;i<request->product_count;i++)
      {
        response->product_status[i].product_id = request->product[i].product_id;
        response->product_status[i].major_version = request->product[i].major_version;
        response->product_status[i].minor_version = request->product[i].minor_version;
        response->product_status[i].codec = request->product[i].codec;
        AuthorizedProduct* product = user->FindAuthorizedProduct(request->product[i].product_id);
        if(NULL != product)
        {
          response->product_status[i].product_status = AUTHORITY_VALID; 
          response->product_status[i].message_count = product->message_count();
          response->product_status[i].message_info = new MessageInfo[product->message_count()];
          for(int j=0;j<product->message_count();j++)
          {
            response->product_status[i].message_info[j].message_id = product->message_id(j);
            response->product_status[i].message_info[j].message_status = AUTHORITY_VALID;
          }
        }
        else
        {
          response->product_status[i].product_status = AUTHORITY_INVALID;
          response->product_status[i].message_count = 0; 
          response->product_status[i].message_info = NULL;
        }
      }
    }
  }
  else
  {
    response->ack_type=ret_code;
    response->user_id = -1;
    response->server_count = 0;
    response->product_count = 0;
    response->product_status = NULL;
  }
  if(response->ack_type == AUTHORITY_VALID)
  {
    Logger::instance()->WriteFormatEventLog(LL_INFO,MEI_AUTHORITY_CONFIRMED,"login request","user:%s, ip:%s logined",(char*)request->user_name,client_ip.c_str());
  }
  else
  {
    Logger::instance()->WriteFormatEventLog(LL_ERROR,MEI_AUTHORITY_NONCONFORM,"subscriber manager",
      "authority is invalid . user: %s, ip: %s \n",(char*)request->user_name,client_ip.c_str());  
  }
 
	//AddSubscriber(response);
	SharedMdfMessage mdf_message = CreateMdfMessage(message,response,SYSTEM_LOGIN_RESPONSE_ID);
  SendResponseMessage(mdf_message, processor, feed_server);
  feed_server->subscriber_manager()->ProcessLoginResponse(mdf_message);
}

wmdf::Logout_V_1_0_Handle::Logout_V_1_0_Handle( void )
{
  
}

wmdf::Logout_V_1_0_Handle::~Logout_V_1_0_Handle( void )
{

}

void wmdf::Logout_V_1_0_Handle::ProcessMessage(SharedMdfMessage message,FeedServer* feed_server,ProtocolProcessor* processor)
{
  
}

wmdf::FetchConfigInfo_V_1_0_Handle::FetchConfigInfo_V_1_0_Handle( void )
{

}

wmdf::FetchConfigInfo_V_1_0_Handle::~FetchConfigInfo_V_1_0_Handle( void )
{

}

void wmdf::FetchConfigInfo_V_1_0_Handle::ProcessMessage( SharedMdfMessage message,FeedServer* feed_server ,ProtocolProcessor* processor)
{
  std::string xml ="";
  Subscriber* subscriber = feed_server->subscriber_manager()->FindSubscriber(message->socket_id());  
  ConfigInfoRequset* request = (ConfigInfoRequset*)message->body();
  if(SUCCESS==ServerEnvironment::authenticator()->Check(subscriber->user_id(),PRODEUCT_SYSTEM_ID,SYSTEM_CONFIG_INFO_REQUEST_ID))
  {
    switch(request->config_type)
    {
    case CFT_SYSTEM:
      xml = Utils::Serialize(ServerEnvironment::system_config());
      break;
    case CFT_PRODUCT:
      xml = Utils::Serialize(ServerEnvironment::product_manager());
      break;
    case CFT_SOURCE:
      xml = Utils::Serialize(ServerEnvironment::product_source_manager());
      break;
    case CFT_USER:
      xml = Utils::Serialize(ServerEnvironment::user_manager());
      break;
    default:
      break;
    }
  }
  ConfigInfo* config_info = (ConfigInfo*)malloc(sizeof(ConfigInfo) + xml.size());
  memset(config_info,0,sizeof(ConfigInfo) + xml.size());
  config_info->config_type = request->config_type;
  config_info->length = xml.size();
  memcpy((char*)config_info->bytes,xml.c_str(),xml.size());

  SharedMdfMessage mdf_message = CreateMdfMessage(message,config_info,SYSTEM_CONFIG_INFO_ID);
  SendResponseMessage(mdf_message, processor, feed_server);
}

wmdf::UpdateConfigInfo_V_1_0_Handle::UpdateConfigInfo_V_1_0_Handle( void )
{

}

wmdf::UpdateConfigInfo_V_1_0_Handle::~UpdateConfigInfo_V_1_0_Handle( void )
{

}

void wmdf::UpdateConfigInfo_V_1_0_Handle::ProcessMessage( SharedMdfMessage message,FeedServer* feed_server ,ProtocolProcessor* processor)
{
  std::string xml ="";
  Subscriber* subscriber = feed_server->subscriber_manager()->FindSubscriber(message->socket_id());  
  ConfigInfo* config_info = (ConfigInfo*)message->body();
  UpdateConfigResponse* back_info = new UpdateConfigResponse;
  memset(back_info,0,sizeof(UpdateConfigResponse));
  back_info->config_type = config_info->config_type;
  back_info->return_value = ServerEnvironment::authenticator()->Check(subscriber->user_id(),PRODEUCT_SYSTEM_ID,SYSTEM_CONFIG_INFO_ID);
  if(SUCCESS==back_info->return_value)
  {
    xml = string(config_info->bytes);
    switch(config_info->config_type)
    {
    case CFT_SYSTEM:
      Utils::Deserialize((char*)config_info->bytes,ServerEnvironment::system_config());
      Utils::Serialize(ApplicationDirectory::SystemDescriptionFile(),ServerEnvironment::system_config());
      Logger::instance()->WriteLog(LL_INFO,"System config file updated.");
      break;
    case CFT_PRODUCT:
      Utils::Deserialize((char*)config_info->bytes,ServerEnvironment::product_manager());
      Utils::Serialize(ApplicationDirectory::ProductDescriptionFile(),ServerEnvironment::product_manager());
      Logger::instance()->WriteLog(LL_INFO,"Product config file updated.");
      break;
    case CFT_SOURCE:
      Utils::Deserialize((char*)config_info->bytes,ServerEnvironment::product_source_manager());
      Utils::Serialize(ApplicationDirectory::SourceDescriptionFile(),ServerEnvironment::product_source_manager());
      Logger::instance()->WriteLog(LL_INFO,"Source config file updated.");
      break;
    case CFT_USER:
      Utils::Deserialize((char*)config_info->bytes,ServerEnvironment::user_manager());
      Utils::Serialize(ApplicationDirectory::UserDescriptionFile(),ServerEnvironment::user_manager());
      Logger::instance()->WriteLog(LL_INFO,"User config file updated.");
      break;
    default:
      break;
    }
  }
  SharedMdfMessage mdf_message = CreateMdfMessage(message,back_info,SYSTEM_UPDATE_CONFIG_RESPONSE_ID);
  SendResponseMessage(mdf_message, processor, feed_server);
  
}
