#include "utils.h"
#include "../protocol/mdf_message.h"
#include <fstream>
#include "../include/wmdf_internal_api.h"
#ifdef WIN32
  #include <windows.h>
#else
  #include <pthread.h>
  #include <unistd.h>
#endif

wmdf::Utils::Utils(void)
{
}

wmdf::Utils::~Utils(void)
{
}

wmdf::Serializer* wmdf::Utils::CreateSerializer( const string& file_name )
{
  return new XmlFileSerializer(file_name);
}

void wmdf::Utils::Deserialize( const string& file_name,Persistable* persistable )
{
  Serializer* serializer= CreateSerializer(file_name);
  serializer->Deserialization(persistable);
  delete serializer;
  serializer = NULL;
}

void wmdf::Utils::Deserialize( const char* xml ,Persistable* persistable)
{
  XmlStringSerializer* serializer= new XmlStringSerializer();
  serializer->SetXml(xml);
  serializer->Deserialization(persistable);
  delete serializer;
  serializer = NULL;
}

void wmdf::Utils::Serialize( const string& file_name,Persistable* persistable )
{
  Serializer* serializer= CreateSerializer(file_name);
  serializer->Serialization(persistable);
  delete serializer;
  serializer = NULL;
}

std::string wmdf::Utils::Serialize( Persistable* persistable )
{
  XmlStringSerializer* serializer = new XmlStringSerializer();
  serializer->Serialization(persistable);
  string xml = serializer->GetXml();
  delete serializer;
  return xml;
}

void wmdf::Utils::SleepTime( int millsecond )
{
#ifdef WIN32
    Sleep(millsecond);
#else
    usleep(1000*millsecond);
#endif

}

void wmdf::Utils::DestroyMDFRecord(MdfRecord*& mdfrecord)
{
  if (NULL != mdfrecord->header)
  {
    delete mdfrecord->header;
    mdfrecord->header = NULL;
  }
  if(NULL != mdfrecord->body)
  {
    switch(mdfrecord->product_id)
    {
    case PRODEUCT_SYSTEM_ID:
      DestroySystemMessage(mdfrecord);
      break;
    case PRODEUCT_MONITOR_ID:
      DestroyMonitorMessage(mdfrecord);
      break;
    case PRODUCT_SSE_LEVEL1_ID:
    case PRODUCT_SZSE_LEVEL1_ID:
    case PRODUCT_SSE_LEVEL2_ID:
    case PRODUCT_SZSE_LEVEL2_ID:
    case PRODUCT_CFFE_LEVEL1_ID:
    case PRODUCT_CFFE_LEVEL2_ID:
    case PRODUCT_SHFE_LEVEL1_ID:
    case PRODUCT_DCE_LEVEL1_ID:
    case PRODUCT_CZCE_LEVEL1_ID:
    case PRODUCT_FX_ID:
    case PRODUCT_FTSE_LEVEL1_ID:
      DestroyMarketMessage(mdfrecord);
      break;
    default:
      if (NULL != mdfrecord->body)
      {
        delete [] mdfrecord->body;
        mdfrecord->body = NULL;
      }
      break;
    }
  }
  delete mdfrecord;
  mdfrecord = NULL;
}

void wmdf::Utils::DestroyLoginRequest(LoginRequest*& request)
{
  if (request->product_count > 0)
  {
    delete[] request->product;
    request->product = NULL;
  }
  delete request;
  request = NULL;
}

void wmdf::Utils::DestroyLoginResponse(LoginResponse*& response)
{
  if(NULL!=response)
  {
    if(NULL!=response->serverinfo)
    {
      delete[] response->serverinfo;
      response->serverinfo=NULL;
    }
    if(NULL!=response->product_status)
    {
      for(int i=0;i<response->product_count;i++)
      {
        if(NULL!=response->product_status[i].message_info)
        {
          delete[] response->product_status[i].message_info;
          response->product_status[i].message_info = NULL;
        }
      }
      delete[] response->product_status;
      response->product_status = NULL;
    }
    delete response;
    response = NULL;
  }
}

void wmdf::Utils::DestroySystemMessage(MdfRecord*& mdfrecord)
{
  if(mdfrecord->message_id == SYSTEM_HEARTBEAT_ID)
  {
    //TODO(心跳消息暂时没定义...)
  }
  else if(mdfrecord->message_id == SYSTEM_LOGIN_REQUEST_ID)
  {      
    LoginRequest* request = (LoginRequest*)mdfrecord->body;
    DestroyLoginRequest(request);
  }
  else if(mdfrecord->message_id == SYSTEM_LOGIN_RESPONSE_ID)
  {
    LoginResponse* response = (LoginResponse*)mdfrecord->body;
    DestroyLoginResponse(response);
  }
  else if(mdfrecord->message_id == SYSTEM_LOGOUT_ID)
  {
    Logout* logout = (Logout*)mdfrecord->body;
    delete logout;
  }
  else if(mdfrecord->message_id == SYSTEM_CONFIG_INFO_REQUEST_ID)
  {
    ConfigInfoRequset* request = (ConfigInfoRequset*)mdfrecord->body;
    delete request;
  }
  else if(mdfrecord->message_id == SYSTEM_CONFIG_INFO_ID)
  {
    ConfigInfo* config_info = (ConfigInfo*)mdfrecord->body;
    free(config_info);
  }
  mdfrecord->body = NULL;
}

void wmdf::Utils::DestroyMonitorMessage(MdfRecord*& mdfrecord)
{
  //深交所的委托队列 SZSEL2_ORDERQUEUE_ID 与上交所的相同
  if(mdfrecord->message_id==MONITOR_CLIENT_STATUS_ID) 
  {
    ClientStatus* client_status = (ClientStatus*)mdfrecord->body;
    for(int i=0;i<mdfrecord->record_count;i++)
    {
      if(NULL!=client_status[i].product_infos)
      {
        delete[] client_status[i].product_infos;
        client_status[i].product_infos = NULL;
      }              
    }
    delete[] client_status;
  }
  else
  {
    delete[] mdfrecord->body;
  }
  mdfrecord->body = NULL;
}

void wmdf::Utils::DestroyMarketMessage(MdfRecord*& mdfrecord)
{
  //深交所的委托队列 SZSEL2_ORDERQUEUE_ID 与上交所的相同
  if(mdfrecord->message_id==SSEL2_ORDERQUEUE_ID) 
  {
    L2OrderQueue* orderQueueL2 = (L2OrderQueue*)mdfrecord->body;
    for(int i=0;i<mdfrecord->record_count;i++)
    {
      for(int j=0;j<orderQueueL2[i].price_level_count;j++)
      {
        delete[] orderQueueL2[i].price_levels[j].order_items;
      }
      delete[] orderQueueL2[i].price_levels;
    }
    delete[] orderQueueL2;
  }
  else
  {
    delete[] mdfrecord->body;
  }
  mdfrecord->body = NULL;
}

void wmdf::Utils::DestroyClientStatus( ClientStatus*& client_status )
{
  if(NULL!=client_status)
  { 
    if(NULL!=client_status->product_infos)
    {
      delete[] client_status->product_infos;
      client_status->product_infos = NULL;
    }
    delete client_status;
    client_status = NULL;
  }
}

int8_t wmdf::Utils::GetMajorVersion( int16_t product_id )
{
  switch(product_id)
  {
  case PRODEUCT_SYSTEM_ID:
    return MAJOR_VERSION_SYSTEM_ID;
  case PRODEUCT_MONITOR_ID:
    return MAJOR_VERSION_MONITOR_ID;
  case PRODUCT_SSE_LEVEL1_ID:
    return MAJOR_VERSION_SSE_LEVEL1_ID;
  case PRODUCT_SZSE_LEVEL1_ID:
    return MAJOR_VERSION_SZSE_LEVEL1_ID;
  case PRODUCT_SSE_LEVEL2_ID:
    return MAJOR_VERSION_SSE_LEVEL2_ID;
  case PRODUCT_SZSE_LEVEL2_ID:
    return MAJOR_VERSION_SZSE_LEVEL2_ID;
  case PRODUCT_CFFE_LEVEL1_ID:
    return MAJOR_VERSION_CFFE_LEVEL1_ID;
  case PRODUCT_CFFE_LEVEL2_ID:
    return MAJOR_VERSION_CFFE_LEVEL2_ID;
  case PRODUCT_SHFE_LEVEL1_ID:
    return MAJOR_VERSION_SHFE_LEVEL1_ID;
  case PRODUCT_DCE_LEVEL1_ID:
    return MAJOR_VERSION_DCE_LEVEL1_ID;
  case PRODUCT_CZCE_LEVEL1_ID:
    return MAJOR_VERSION_CZCE_LEVEL1_ID;
  case PRODUCT_FX_ID:
    return MAJOR_VERSION_FX_ID;
  case PRODUCT_FTSE_LEVEL1_ID:
    return MAJOR_VERSION_FTSE_ID;
  default:
    return MAJOR_VERSION_SYSTEM_ID;
  }
}

int8_t wmdf::Utils::GetMinorVersion( int16_t product_id )
{
  switch(product_id)
  {
  case PRODEUCT_SYSTEM_ID:
    return MINOR_VERSION_SYSTEM_ID;
  case PRODEUCT_MONITOR_ID:
    return MINOR_VERSION_MONITOR_ID;
  case PRODUCT_SSE_LEVEL1_ID:
    return MINOR_VERSION_SSE_LEVEL1_ID;
  case PRODUCT_SZSE_LEVEL1_ID:
    return MINOR_VERSION_SZSE_LEVEL1_ID;
  case PRODUCT_SSE_LEVEL2_ID:
    return MINOR_VERSION_SSE_LEVEL2_ID;
  case PRODUCT_SZSE_LEVEL2_ID:
    return MINOR_VERSION_SZSE_LEVEL2_ID;
  case PRODUCT_CFFE_LEVEL1_ID:
    return MINOR_VERSION_CFFE_LEVEL1_ID;
  case PRODUCT_CFFE_LEVEL2_ID:
    return MINOR_VERSION_CFFE_LEVEL2_ID;
  case PRODUCT_SHFE_LEVEL1_ID:
    return MINOR_VERSION_SHFE_LEVEL1_ID;
  case PRODUCT_DCE_LEVEL1_ID:
    return MINOR_VERSION_DCE_LEVEL1_ID;
  case PRODUCT_CZCE_LEVEL1_ID:
    return MINOR_VERSION_CZCE_LEVEL1_ID;
  case PRODUCT_FX_ID:
    return MINOR_VERSION_FX_ID;
  case PRODUCT_FTSE_LEVEL1_ID:
    return MINOR_VERSION_FTSE_ID;
  default:
    return MINOR_VERSION_SYSTEM_ID;
  }
}
