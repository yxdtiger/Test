#include "directory.h"
#include <boost/filesystem.hpp>
#include <boost/filesystem/operations.hpp>

namespace bfs=boost::filesystem;

wmdf::Directory::Directory(void)
{
}

wmdf::Directory::~Directory(void)
{
}

bool wmdf::Directory::Exists( const std::string& path )
{
  return bfs::exists(bfs::path(path));
}

bool wmdf::Directory::Remove( const std::string& path )
{
  return bfs::remove(bfs::path(path));
}

void wmdf::Directory::RemoveAll( const std::string& path)
{
  bfs::remove_all(bfs::path(path));
}

bool wmdf::Directory::CreateDirectory( const std::string& path )
{
  return bfs::create_directory(bfs::path(path));
}

bool wmdf::Directory::CreateDirectories( const std::string& path )
{
  return bfs::create_directories(bfs::path(path));
}

void wmdf::Directory::GetAllFileOfPath( const std::string& path,std::vector<std::string>& files )
{
  bfs::path file_path(path);
  if (!bfs::exists(file_path)) return;

  if(bfs::is_directory(file_path))//可能为目录
  {
     bfs::directory_iterator end_itr; // 缺省构造生成一个结束迭代器
    for ( bfs::directory_iterator iter( file_path );iter != end_itr;++iter )
    {
      if(bfs::is_directory(iter->path()))//可能为目录
      {
        GetAllFileOfPath(iter->path().string(),files);
      }
      else
      {
        files.push_back(iter->path().string());
      }
    }
  }
  else
  {
    files.push_back(file_path.string());
  }
}

bool wmdf::Directory::IsDirectory( const std::string &path )
{
	bfs::path file_path(path);
	
	if(bfs::is_directory(file_path))
		return true;
	else
		return false;
}

