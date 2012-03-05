#include "file.h"
#include <boost/filesystem/operations.hpp>

namespace bfs=boost::filesystem;

wmdf::File::File(void)
{
}

wmdf::File::~File(void)
{
}

bool wmdf::File::Exists( const std::string& path )
{
  return bfs::exists(bfs::path(path));
}

bool wmdf::File::Remove( const std::string& path )
{
  return bfs::remove(bfs::path(path));
}

void wmdf::File::Copy( const std::string& sourceFileName, const std::string& destFileName )
{

}

void wmdf::File::Copy( const std::string& sourceFileName, const std::string& destFileName, bool overwrite )
{

}

int64_t wmdf::File::FileSize( const std::string& file_name )
{
	return bfs::file_size(bfs::path(file_name));
}
