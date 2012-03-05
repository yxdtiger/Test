#ifndef APPLICATION_DIRECTORY_H
#define APPLICATION_DIRECTORY_H

#include <string>

namespace wmdf
{
  class ApplicationDirectory
  {
   
  public:
    ApplicationDirectory(void);
    virtual ~ApplicationDirectory(void);
    static std::string ApplicationPath();
    static std::string ConfigurationPath();
    static std::string LogPath();
    static std::string ProductDescriptionFile();
    static std::string UserDescriptionFile();
    static std::string SourceDescriptionFile();
    static std::string SystemDescriptionFile();
  };
}

#endif
