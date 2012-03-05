#include "application_directory.h"
#ifdef _WIN32
#include <windows.h>
#elif linux
#include <unistd.h>
#include <cstring>
#define MAX_PATH 260
#endif

const std::string product_description_file_ = "product.xml";
const std::string user_description_file_ = "user.xml";
const std::string source_description_file_ = "source.xml";
const std::string system_description_file_ = "system.cfg";

wmdf::ApplicationDirectory::ApplicationDirectory(void)
{
}

wmdf::ApplicationDirectory::~ApplicationDirectory(void)
{
}



std::string wmdf::ApplicationDirectory::ApplicationPath()
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

std::string wmdf::ApplicationDirectory::ConfigurationPath()
{
  std::string configurationpath=ApplicationPath()+std::string("configuration//");
  return configurationpath;
}


std::string wmdf::ApplicationDirectory::ProductDescriptionFile()
{
  std::string path=ConfigurationPath()+product_description_file_;
  return path;
}

std::string wmdf::ApplicationDirectory::UserDescriptionFile()
{
  std::string path=ConfigurationPath()+user_description_file_;
  return path;
}

std::string wmdf::ApplicationDirectory::SourceDescriptionFile()
{
  std::string path=ConfigurationPath()+source_description_file_;
  return path;
}

std::string wmdf::ApplicationDirectory::SystemDescriptionFile()
{
  std::string path=ConfigurationPath()+system_description_file_;
  return path;
}

std::string wmdf::ApplicationDirectory::LogPath()
{
  std::string log_path=ApplicationPath()+std::string("log//");
  return log_path;
}
