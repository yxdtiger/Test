#ifndef FEED_ENVIRONMENT_H
#define FEED_ENVIRONMENT_H
#include "feed_info_manager.h"
#include "market_snapshot_container.h"
#include <string>
#include <map>
namespace wmdf
{
  class FeedEnvironment
  {
  public:    
    static FeedEnvironment* Instance();
    void Initialize();
    void Finalize();
    bool IsFileExist(const char * path);
    static std::string ApplicationPath();
    FeedInfoManager* product_target_manager() {return product_target_manager_;}
    ProductTarget* GetTargetInfo(int16_t product_id);
    
    MarketDataControl* data_control(){return market_data_control_;}
  private:
    FeedEnvironment(void);
    virtual ~FeedEnvironment(void);
    void PrintServerInfo();
    FeedInfoManager* product_target_manager_;
    std::map<int16_t,ProductTarget*> product_targets_;
    
    MarketDataControl* market_data_control_; 
    static FeedEnvironment* instance_;
  };
}

#endif
