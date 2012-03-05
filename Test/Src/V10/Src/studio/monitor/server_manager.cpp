#include "server_manager.h"

#include "serializer_utils.h"
#include "../../environment/application_directory.h"
#include "../../environment/utils.h"
#include "../../environment/strutils.h"
#include "../../include/wmdf_api.h"
#include "../common/studio_utility.h"



using namespace wmdf;
using namespace std;

wmdf::ServerManager* wmdf::ServerManager::instance_ = NULL;  

wmdf::ServerManager::ServerManager(void)
{  
  net_info_collection_=NULL;
}

wmdf::ServerManager::~ServerManager(void)
{  

}

void wmdf::ServerManager::Initialize()
{
  net_info_collection_ = new NetInfoCollection();
  for(int i=0;i<max_num_of_connections;i++)
  {
    connection_ids_.push_back(false);
  }

  
  string filePath = ApplicationDirectory::ApplicationPath()+std::string("servers.xml");
  if (StudioUtility::IsFileExist(filePath.c_str()))
  {
    SerializerUtils::Deserialize(filePath,net_info_collection_);
    for(int i = 0;i<net_info_collection_->server_info_count();i++)
    {
      net_info_collection_->server_infos(i)->set_server_id(get_next_id());
    }
  }
}

void wmdf::ServerManager::Finalize()
{
  string filePath = ApplicationDirectory::ApplicationPath()+std::string("servers.xml");
  SerializerUtils::Serialize(filePath,net_info_collection_);
  delete net_info_collection_;
  net_info_collection_ = NULL;
  connection_ids_.clear();
}

ServerManager* wmdf::ServerManager::Instance()
{
  if(NULL == instance_)  
    instance_ = new ServerManager();  
  return instance_;
}



int32_t wmdf::ServerManager::get_next_id()
{
  int i = 0;
  while(i<max_num_of_connections)
  {
    if(!connection_ids_[i])
    {
      set_connection_id(i,true);
      return i*2;//分配偶数连接监控端口，行情端口=相应的监控端口+1
    }
    else 
    {
      i++;
    }
  }
  return -1;
}

bool wmdf::ServerManager::delete_server_by_cid( int32_t conn_id )
{
  set_connection_id(conn_id,false);
  return net_info_collection_->delete_server_by_cid(conn_id);
}

void wmdf::ServerManager::add_server_info( ServerNetInfo* conn_info )
{
  conn_info->set_server_id(get_next_id());
  net_info_collection_->add_server_info(conn_info);
}






