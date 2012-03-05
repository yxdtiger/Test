#include "model_id_factory.h"
#include "monitor_environment.h"
#include "../../environment/strutils.h"


wmdf::ModelIDFactory::ModelIDFactory(void)
{
}

wmdf::ModelIDFactory::~ModelIDFactory(void)
{
}
//ServerID-SourceIP:SourcePort
std::string wmdf::ModelIDFactory::GetServer2SourcePort(int32_t conn_id,const char* source_ip,int32_t source_port)
{
  return StrUtils::formatstring(30,"%d-S%s:%d",conn_id,source_ip,source_port);
}

//ServerID-ClientIP
std::string wmdf::ModelIDFactory::GetClientTitle(int32_t conn_id ,const char* client_ip,int32_t socket_id )
{
  return StrUtils::formatstring(25,"%d-C%s_%d",conn_id,client_ip,socket_id);
}

//ServerID-ClientSocketID
std::string wmdf::ModelIDFactory::GetServer2ClientSocket(int32_t conn_id ,int32_t socket_id )
{
  return StrUtils::formatstring(15,"%d-ID%d",conn_id,socket_id);
}
