#ifndef DIRECTORY_H
#define DIRECTORY_H

#include <string>
#include <vector>

namespace wmdf
{
  class Directory
  {
  public:
    Directory(void);
    virtual ~Directory(void);
    static bool Exists(const std::string& path);
		static bool IsDirectory(const std::string &path);
    static bool Remove(const std::string& path);
    static void RemoveAll(const std::string& path);
    static bool CreateDirectory(const std::string& path);
    static bool CreateDirectories(const std::string& path);
    static void GetAllFileOfPath(const std::string& path,std::vector<std::string>& files);
  };
}

#endif
