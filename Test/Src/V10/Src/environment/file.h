#ifndef FILE_H
#define FILE_H

#include <string>
#include "../include/wmdf_api.h"

namespace wmdf
{
  class File
  {
  public:
    File(void);
    virtual ~File(void);
    static bool Exists(const std::string& path);
    static bool Remove(const std::string& path);
    static void Copy(const std::string& sourceFileName, const std::string& destFileName);
    static void Copy(const std::string& sourceFileName, const std::string& destFileName, bool overwrite);
		static int64_t FileSize(const std::string& file_name);
  };
}

#endif
