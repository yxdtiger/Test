#include "warning_center.h"

using namespace std;
using namespace wmdf;
wmdf::WarningCenter::WarningCenter(void)
{
}

wmdf::WarningCenter::~WarningCenter(void)
{
  map<std::string,WarningInfo* >::iterator iter = map_warnings_.begin();
  while(iter!=map_warnings_.end())
  {
    if(NULL !=iter->second)
    {
      delete iter->second;
      iter->second = NULL;
    }
    iter++;
  }
}


WarningInfo* wmdf::WarningCenter::GetWarningInfo( std::string key )
{
  map<std::string,WarningInfo* >::iterator iter = map_warnings_.find(key);
  if(iter != map_warnings_.end())
  {
    return iter->second;
  }
  return NULL;
}

void wmdf::WarningCenter::AddWarningInfo( WarningInfo* waringInfo )
{
  map<std::string,WarningInfo* >::iterator iter = map_warnings_.find(waringInfo->key);
  if(iter == map_warnings_.end())
  {
    map_warnings_[waringInfo->key]=waringInfo;
  }
  else
  {
    delete iter->second;
    map_warnings_[waringInfo->key] = waringInfo;
  }
}

void wmdf::WarningCenter::DeleteWarningInfo( std::string key,int point )
{
  map<std::string,WarningInfo* >::iterator iter = map_warnings_.find(key);
  if(iter != map_warnings_.end())
  {
    if(iter->second->monitor_point==point)
    {
      delete iter->second;
      iter->second = NULL;
      map_warnings_.erase(iter);
    }
    else
    {
      iter->second->monitor_point = (StudioMonitorStatus)(iter->second->monitor_point &~point);
    }
  }
}

bool wmdf::WarningCenter::NeedAlarm()
{
  map<std::string,WarningInfo* >::iterator iter = map_warnings_.begin();
  while(iter!=map_warnings_.end())
  {
    if(NULL !=iter->second)
    {
      if(iter->second->warningLevel<LL_INFO)
        return true;
    }
    iter++;
  }
  return false;
}

void wmdf::WarningCenter::DeleteAllWarnings()
{
  map<std::string,WarningInfo* >::iterator iter = map_warnings_.begin();
  while(iter!=map_warnings_.end())
  {
    if(NULL !=iter->second)
    {
      delete iter->second;
      iter->second = NULL;
    }
    iter++;
  }
  map_warnings_.clear();
}
