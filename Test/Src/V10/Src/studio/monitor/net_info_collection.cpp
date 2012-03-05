#include "net_info_collection.h"

using namespace wmdf;
wmdf::NetInfoCollection::NetInfoCollection(void)
{
}

wmdf::NetInfoCollection::~NetInfoCollection(void)
{
  for(int32_t i = 0; i < server_info_count(); ++i)
  {
    if(NULL!=server_infos(i))
      delete server_infos(i);
  }
  servers_.clear();
}

bool wmdf::NetInfoCollection::LoadState( Memento* m )
{
  int child_num=m->GetChildCount();
  for(int i=0;i<child_num;i++)
  {
    Memento* sub_m=m->GetChild(i);
    ServerNetInfo* pt=new ServerNetInfo();
    pt->LoadState(sub_m);
    add_server_info(pt);
  }
  return true;
}

bool wmdf::NetInfoCollection::SaveState( Memento* m )
{
  m->SetName("Servers");
  for(uint32_t i=0;i<servers_.size();i++)
  {
    Memento* sub_m=m->CreateChild("ServerInfo");
    servers_[i]->SaveState(sub_m);
  }
  return true;
}

void wmdf::NetInfoCollection::add_server_info( ServerNetInfo* conn_info )
{
  servers_.push_back(conn_info);
}

bool wmdf::NetInfoCollection::delete_server_by_cid( int32_t conn_id )
{
  std::vector<ServerNetInfo*>::iterator it = servers_.begin();
  while (it != servers_.end())
  {
    if((*it)->server_id()==conn_id)
    {
      delete (*it);
      servers_.erase(it);
      return true;
    }
    it++;
  }
  return false;
}

ServerNetInfo* wmdf::NetInfoCollection::find_server_by_cid( int32_t conn_id )
{
  for(int32_t i = 0; i < server_info_count(); ++i)
  {
    if(server_infos(i)->server_id() == conn_id)
      return server_infos(i);
  }
  return NULL;
}

NetInfoCollection* wmdf::NetInfoCollection::Copy()
{
  NetInfoCollection* collection = new NetInfoCollection();
  for (int i = 0;i<servers_.size();i++)
  {
    collection->add_server_info(servers_[i]->Copy());
  }
  return collection;
}

void wmdf::NetInfoCollection::RestAllModels()
{
  for (int i = 0;i<servers_.size();i++)
  {
    servers_[i]->RestAllModels();
  }
}

bool wmdf::NetInfoCollection::NeedAlarm()
{
  for (int i = 0;i<servers_.size();i++)
  {
    if(servers_[i]->NeedAlarm())
    {
      return true;
    }
  }
  return false;
}

void wmdf::NetInfoCollection::ClearWarningStatus()
{
  for (int i = 0;i<servers_.size();i++)
  {
    servers_[i]->ClearWarningStatus();
  }
}


