#include "feed_environment.h"
#include "../serialization/serializer_utils.h"
#include "../include/lclientapi.h"

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
#include <cstdio>
#include <cstring>
#endif


using namespace std;

wmdf::FeedEnvironment* wmdf::FeedEnvironment::instance_ = NULL;

wmdf::FeedEnvironment::FeedEnvironment(void)
{
  wmdf::FeedEnvironment::product_target_manager_ = new FeedInfoManager();
}

wmdf::FeedEnvironment::~FeedEnvironment(void)
{
}

void wmdf::FeedEnvironment::PrintServerInfo()
{
  APIVersionArgs version;
  wmdf_get_api_version(&version);
  printf("**********************************************************************\n");
  printf("* Wind Market Data Feed Capturer Copyright (C) 2011                  *\n");
  printf("* Developers:Wind MDS Team                                           *\n");
  printf("* FeedAPI version:%d.%d.%d                                              *\n",version.major,version.minor,version.patch);
  printf("**********************************************************************\n");
  printf("\n");

}

bool wmdf::FeedEnvironment::IsFileExist(const char * path)
{
#ifdef WIN32
  return _access(path,0)==0;
#elif __linux__
  return access(path,0)==0;
#endif
}


void wmdf::FeedEnvironment::Initialize()
{
  PrintServerInfo();
  string filePath = ApplicationPath()+std::string("feedinfos.xml");
  if (IsFileExist(filePath.c_str()))
  {
    SerializerUtils::Deserialize(filePath,product_target_manager_);
  }
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

