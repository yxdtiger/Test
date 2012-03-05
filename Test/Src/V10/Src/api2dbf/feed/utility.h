#ifndef UTILITY_H
#define UTILITY_H
#include "../../include/wmdf_api.h"
#include <string>

namespace wmdf
{
  class Utility
  {
  public:
    Utility(void);
    virtual ~Utility(void);
    static void SleepTime(int millsecond);
    static bool IsDirectoryExists(const std::string& path);
    static bool CreateDirectories(const std::string& path);
    static std::string SetMktDestPath(int16_t product_id,std::string dir);
  };
}

#endif
