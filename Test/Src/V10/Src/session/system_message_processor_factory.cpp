#include "system_message_processor.h"
#include "system_message_processor_factory.h"
#include "system_message_handler.h"
#include "../environment/server_environment.h"
#include "../product/product_descripter.h"
#include "../product/product_manager.h"
#include "system_message_processor_center.h"


using namespace wmdf;
wmdf::SystemMessageProcessorFactory::SystemMessageProcessorFactory(void)
{
}

wmdf::SystemMessageProcessorFactory::~SystemMessageProcessorFactory(void)
{
}

void wmdf::SystemMessageProcessorFactory::InitSysMessageProcessorCenter(SystemMessageProcessorCenter* sys_msg_processor_center)
{
  ProductDescripter* product = NULL;
  VersionDescripter* version = NULL;
  SystemMessageProcessor* systemMessageProcessor = NULL;
  ProtocolDescription* protocolDescription = NULL;
  for(int i = 0;i<ProductManager::instance()->products_count();i++)
  {
    product = ProductManager::instance()->products(i);
    if(product->product_id()==PRODEUCT_SYSTEM_ID)
    {
      for(int j = 0;j<product->support_versions_count();j++)
      {
        version = product->support_versions(j);
        
        protocolDescription = new ProtocolDescription(
        product->product_id(),version->major_version(),version->minor_version(),CT_BINARY);

        systemMessageProcessor = new SystemMessageProcessor(protocolDescription);		
        CreateSysMessageHandles(systemMessageProcessor);
        sys_msg_processor_center->AddSystemMessageProcessor(systemMessageProcessor);    
      }
    }
  }  
}

void wmdf::SystemMessageProcessorFactory::CreateSysMessageHandles(SystemMessageProcessor* systemMessageProcessor)
{
  switch(systemMessageProcessor->unique_id())
  {
    case SYSTEM_1_0_BINARY:	 
      Create_System_1_0_Handles(systemMessageProcessor);
      break;
    default:
      break;
  }
}


void wmdf::SystemMessageProcessorFactory::Create_System_1_0_Handles(SystemMessageProcessor* systemMessageProcessor)
{
  SystemMessageHandler* handle = new LoginRequest_V_1_0_Handle();
  systemMessageProcessor->AddMessageHandle(SYSTEM_LOGIN_REQUEST_ID,handle);

  handle = new Logout_V_1_0_Handle();
  systemMessageProcessor->AddMessageHandle(SYSTEM_LOGOUT_ID,handle);

  handle = new FetchConfigInfo_V_1_0_Handle();
  systemMessageProcessor->AddMessageHandle(SYSTEM_CONFIG_INFO_REQUEST_ID,handle);

  handle = new UpdateConfigInfo_V_1_0_Handle();
  systemMessageProcessor->AddMessageHandle(SYSTEM_CONFIG_INFO_ID,handle);
}

