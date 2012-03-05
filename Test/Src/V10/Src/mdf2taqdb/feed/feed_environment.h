#ifndef FEED_ENVIRONMENT_H
#define FEED_ENVIRONMENT_H
#include "feed_info_manager.h"
#include <string>
#include <map>
namespace wmdf
{
  class FeedEnvironment
  {
  public:    
    static FeedEnvironment* Instance();
    void Initialize();
    bool IsFileExist(const char * path);
    static std::string ApplicationPath();
    FeedInfoManager* product_target_manager() {return product_target_manager_;}
  private:
    void PrintServerInfo();
    FeedEnvironment(void);
    virtual ~FeedEnvironment(void);
    static FeedEnvironment* instance_;
    FeedInfoManager* product_target_manager_;
  };
}

#endif
