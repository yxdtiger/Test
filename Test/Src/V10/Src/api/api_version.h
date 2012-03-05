#ifndef API_VERSION_H
#define API_VERSION_H

#include <string>


namespace wmdf
{

#define API_VERSION_MAJOR 1
#define API_VERSION_MINOR 0
#define API_VERSION_PATCH 1


  class ApiVersion
  {
  public:
    ApiVersion(void);
    virtual ~ApiVersion(void);
    static int GetMajor();
    static int GetMinor();
    static int GetPatch();
    static std::string GetVersion();
    static bool IsAtLeast(int major, int minor, int patch);
  };
}

#endif
