#include "../../include/wmdf_api.h"
#include <iostream>
#include <assert.h>
#include "data_feed.h"
#include "capture_timer.h"
#include "feed_environment.h"

using namespace std;
using namespace wmdf;



void CallBackFunc(RecordArgs* recordArgs)
{  
  if(NULL!=recordArgs)
  {
    DataFeed* gf = (DataFeed*)recordArgs->state;
    if(NULL != gf) 
      gf->Process(recordArgs->record);
    wmdf_release_record(recordArgs);
  }  
}


int main(int argc, char* argv[])
{
  FeedEnvironment::Instance()->Initialize(); 

  wmdf_initialize();  
  for(int i=0;i<FeedEnvironment::Instance()->product_target_manager()->feed_info_count();i++)
  {
    for(int j=0;j<FeedEnvironment::Instance()->product_target_manager()->feed_infos(i)->target_info_count();j++)
    {
      CaptureTimer* timer = new CaptureTimer(FeedEnvironment::Instance()->product_target_manager()->feed_infos(i)->targets(j)->product_id());
      timer->Start();   
    }

    DataFeed *gf = new DataFeed();  
    CallbackArgs callback_args ={0};
    callback_args.connection_id=i;
    callback_args.record_callback =CallBackFunc;
    callback_args.state=gf;
    wmdf_set_callback(&callback_args);
    
    OpenArgs conn_args;
    conn_args.codec_type = (CodecType)FeedEnvironment::Instance()->product_target_manager()->feed_infos(i)->codec_type();
    conn_args.connection_id = i;
    strcpy((char*)conn_args.user_name,FeedEnvironment::Instance()->product_target_manager()->feed_infos(i)->user_name().c_str());
    strcpy((char*)conn_args.password,FeedEnvironment::Instance()->product_target_manager()->feed_infos(i)->password().c_str());
    strcpy((char*)conn_args.server_addr,FeedEnvironment::Instance()->product_target_manager()->feed_infos(i)->source_ip().c_str());
    conn_args.server_port=FeedEnvironment::Instance()->product_target_manager()->feed_infos(i)->source_port();
    conn_args.marktet_open_time = 00000;
    conn_args.markt_close_time = 235959;    
    if (wmdf_open_connection(&conn_args)==SUCCESS)
    {
      printf("succeed to login feed server.\n");
    }
    else
    {
      printf("failed to login feed server.\n");
    }
  }

  int input =1;
  
  while(input!=0)
  {
    std::cin>>input;
  }

  return 0;
}