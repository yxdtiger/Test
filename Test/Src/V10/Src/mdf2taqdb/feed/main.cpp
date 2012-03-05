#include "../../include/wmdf_api.h"
#include <iostream>
#include <assert.h>
#include "gava_feed.h"
#include "feed_environment.h"
#include "../../monitor/logger.h"

using namespace std;
using namespace wmdf;


int main(int argc, char* argv[])
{
  FeedEnvironment::Instance()->Initialize(); 
  Logger::instance()->StartFileAppender();
  wmdf_initialize();  

  for(int i=0;i<FeedEnvironment::Instance()->product_target_manager()->feed_info_count();i++)
  {
    FeedInfo* feed_info = FeedEnvironment::Instance()->product_target_manager()->feed_infos(i);
    GavaFeed *gf = new GavaFeed(feed_info);  
    gf->Start();
  }

  int input =1;
  
  while(input!=0)
  {
    std::cin>>input;
  }

  return 0;
}