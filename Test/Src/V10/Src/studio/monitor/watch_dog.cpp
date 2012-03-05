#include "watch_dog.h"
#include "server_manager.h"
#include "model_id_factory.h"
#include "monitor_environment.h"
#include "../../environment/utils.h"
#include "../../environment/strutils.h"
#include "../../environment/splitter.h"
#include "../main_frame/StdAfx.h"
#include "../main_frame/MainFrm.h"

using namespace wmdf;
wmdf::WatchDog::WatchDog(int time_span)
{
  is_working_ = false;
  time_span_ = time_span;
  pre_collection = NULL;
  cur_collection = NULL;
}

wmdf::WatchDog::~WatchDog(void)
{
  Stop();
}

void wmdf::WatchDog::StartWork()
{
  if (!is_working_)
  {
    is_working_ = true;
    work_thread_ = new boost::thread(boost::bind(&WatchDog::DoWatch,this));
  }
}

void wmdf::WatchDog::DoWatch()
{
  while (is_working_)
  {
    Alarm();
    Utils::SleepTime(time_span_);
  }  
}

void wmdf::WatchDog::Stop()
{
  if(is_working_)
  {
    is_working_=false;
    work_thread_->join();
  }
}

void wmdf::WatchDog::Alarm()
{
  if(MonitorEnvironment::Instance()->is_sound_enable()&&ServerManager::Instance()->net_info_collection()->NeedAlarm())
    AfxGetApp()->m_pMainWnd->SendMessage(WM_MONITOR_WARNING);
} 

