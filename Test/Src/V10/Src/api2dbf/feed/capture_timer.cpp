#include "capture_timer.h"
#include "feed_environment.h"
#include "utility.h"
#include "../../include/date_time.h"

using namespace std;
wmdf::CaptureTimer::CaptureTimer(int16_t product_id)
{
  is_working_ = false;
  product_id_ = product_id;
  time_span_  = 3000;
  next_date_ = DateTime::today();
  ProductTarget* target = FeedEnvironment::Instance()->GetTargetInfo(product_id);
  if(NULL != target)
  {
    file_path_ = Utility::SetMktDestPath(product_id,target->dest_dir());
    if(!Utility::IsDirectoryExists(target->dest_dir()))
    {
      printf("Directory: (%s) is not existing.\n",target->dest_dir().c_str());
      if(Utility::CreateDirectories(target->dest_dir()))
      {
        printf("Create directory: (%s) successfully.\n",target->dest_dir().c_str());
      }
      else
      {
        printf("Create directory: (%s) failed.\n",target->dest_dir().c_str());
      }
    }
    time_span_ = target->time_span();
    start_time_ = target->start_time();
    end_time_ = target->end_time();
  }
}

wmdf::CaptureTimer::~CaptureTimer(void)
{
  Stop();
}

void wmdf::CaptureTimer::Start()
{
  if (!is_working_)
  {
    is_working_ = true;
    work_thread_ = new boost::thread(boost::bind(&CaptureTimer::Capture,this));
  }
}

void wmdf::CaptureTimer::Stop()
{
  if(is_working_)
  {
    is_working_=false;
    work_thread_->join();
  }
}

void wmdf::CaptureTimer::Capture()
{
  while (is_working_)
  {
    date_t today = DateTime::today();
    int32_t cur_time = DateTime::time2hms(DateTime::get_current_time());
    if(today==next_date_)
    {
      if(cur_time>=start_time_&&cur_time<=end_time_)
      {
        FeedEnvironment::Instance()->data_control()->OutputMarketData(product_id_,file_path_);
      }
      else if(cur_time>end_time_)
      {
        next_date_ += 1;
        FeedEnvironment::Instance()->data_control()->Clear(product_id_);
      }
    }    
    Utility::SleepTime(time_span_);
  }  
}

