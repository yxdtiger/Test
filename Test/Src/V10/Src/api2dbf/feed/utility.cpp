#include "utility.h"
#include <boost/filesystem.hpp>
#include <boost/filesystem/operations.hpp>


namespace bfs=boost::filesystem;

#ifdef _WIN32
#include <windows.h>
#else
#include <pthread.h>
#include <unistd.h>
#endif

wmdf::Utility::Utility(void)
{
}

wmdf::Utility::~Utility(void)
{
}

void wmdf::Utility::SleepTime( int millsecond )
{
#ifdef _WIN32
  Sleep(millsecond);
#else
  usleep(1000*millsecond);
#endif

}

std::string wmdf::Utility::SetMktDestPath( int16_t product_id,std::string dir)
{  
  std::string file_path = "";
  switch(product_id)
  {
  case PRODUCT_SSE_LEVEL1_ID:
    file_path = dir+"/ssel1_mkt.dbf";
    break;
  case PRODUCT_SSE_LEVEL2_ID:
    file_path = dir+"/sse_mkt.dbf";
    break;
  case PRODUCT_SZSE_LEVEL1_ID:
    file_path = dir+"/szsel1_mkt.dbf";
    break; 
  case PRODUCT_SZSE_LEVEL2_ID:
    file_path = dir+"/szse_mkt.dbf";
    break; 
  case PRODUCT_CFFE_LEVEL1_ID:
    file_path = dir+"/cffel1_mkt.dbf";
    break;
  case PRODUCT_CFFE_LEVEL2_ID:
    file_path = dir+"/cffel2_mkt.dbf";
    break;
  case PRODUCT_SHFE_LEVEL1_ID:
    file_path = dir+"/shfe_mkt.dbf";
    break;
  case PRODUCT_DCE_LEVEL1_ID:
    file_path = dir+"/dce_mkt.dbf";
    break;
  case PRODUCT_CZCE_LEVEL1_ID:
    file_path = dir+"/czce_mkt.dbf";
    break;
  case PRODUCT_FTSE_LEVEL1_ID:
    file_path = dir+"/ftse_mkt.dbf";
    break;
  default:
    break; 
  }
  return file_path;
}

bool wmdf::Utility::IsDirectoryExists( const std::string& path )
{
  return bfs::exists(bfs::path(path));
}


bool wmdf::Utility::CreateDirectories( const std::string& path )
{
  return bfs::create_directories(bfs::path(path));
}
