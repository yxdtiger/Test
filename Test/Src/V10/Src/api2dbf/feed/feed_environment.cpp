#include "feed_environment.h"
#include "../serialization/serializer_utils.h"

#ifdef _WIN32
#include <windows.h>
#elif linux
#include <unistd.h>
#define MAX_PATH 260
#endif

#ifdef _WIN32
#include <io.h>
#elif __linux__
#include <sys/io.h>
#endif


using namespace std;
using namespace wmdf;

wmdf::FeedEnvironment* wmdf::FeedEnvironment::instance_ = NULL;  

wmdf::FeedEnvironment::FeedEnvironment(void)
{
  product_target_manager_ = new FeedInfoManager();
  market_data_control_ = new MarketDataControl();
}

wmdf::FeedEnvironment::~FeedEnvironment(void)
{
  delete product_target_manager_;
  delete market_data_control_;
}

bool wmdf::FeedEnvironment::IsFileExist(const char * path)
{
#ifdef WIN32
  return _access(path,0)==0;
#elif __linux__
  return access(path,0)==0;
#endif
}

void wmdf::FeedEnvironment::PrintServerInfo()
{
  printf("**********************************************************************\n");
  printf("* Wind Market Data Feed To DBF  Copyright (C) 2011                   *\n");
  printf("* Developers:Wind MDS Team                                           *\n");
  printf("**********************************************************************\n");
  printf("\n");
}

void wmdf::FeedEnvironment::Initialize()
{
  PrintServerInfo();
  string filePath = ApplicationPath()+std::string("api2dbf.xml");
  if (IsFileExist(filePath.c_str()))
  {
    SerializerUtils::Deserialize(filePath,product_target_manager_);
    for(int i =0;i<product_target_manager_->feed_info_count();i++)
    {
      for(int j=0;j<product_target_manager_->feed_infos(i)->target_info_count();j++)
      {
        product_targets_.insert(make_pair(product_target_manager_->feed_infos(i)->targets(j)->product_id(),product_target_manager_->feed_infos(i)->targets(j)));         
      }
    }
  }
}

void wmdf::FeedEnvironment::Finalize()
{
  if(instance_!=NULL)
  {
    delete instance_;
    instance_=NULL;
  }
}

ProductTarget* wmdf::FeedEnvironment::GetTargetInfo( int16_t product_id )
{
  std::map<int16_t, ProductTarget*>::iterator iter = product_targets_.find(product_id);
  if (iter != product_targets_.end())
  {
    return iter->second;
  }
  return NULL;
}





wmdf::FeedEnvironment* wmdf::FeedEnvironment::Instance()
{
  if(NULL == instance_)  
    instance_ = new FeedEnvironment();  
  return instance_;
}

std::string wmdf::FeedEnvironment::ApplicationPath()
{
  char g_path[MAX_PATH] = {0};
  char path[256]={0};

#ifdef WIN32
  GetModuleFileNameA(NULL, (char*)path, 256);
#elif linux
  int count=readlink("/proc/self/exe",path,MAX_PATH);
  path[count]='\0';
#endif

  std::string _exe_path = std::string(path);
  char *tmp = (char*)_exe_path.c_str();
#ifdef WIN32
  while(strchr(tmp,'\\') != NULL)		//截取全路径名
  { 
    tmp = strchr(tmp,'\\') + 1; 
  }
#elif linux
  while(strchr(tmp,'/') != NULL)		//截取全路径名
  { 
    tmp = strchr(tmp,'/') + 1; 
  }
#endif

  memcpy(g_path, _exe_path.c_str(), _exe_path.size() - strlen(tmp) );
  std::string applicationpath=std::string(g_path);
  return applicationpath;
}




