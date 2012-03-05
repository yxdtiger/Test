#include "adapter_guard.h"
#include "../../environment/server_environment.h"
#include "../../session/dispatching_center.h"
#include <boost/bind.hpp>
#include "../../environment/utils.h"
#include "../../include/date_time.h"
#include "iproduct_adapter.h"
#include "../../monitor/logger.h"

wmdf::AdapterGuard* wmdf::AdapterGuard::instance_ = new wmdf::AdapterGuard();

wmdf::AdapterGuard::AdapterGuard(void)
{
  isworking_ = false;
}

wmdf::AdapterGuard::~AdapterGuard(void)
{
  Shutdown();
}

void wmdf::AdapterGuard::Startup()
{
  if (!isworking_)
  {
    isworking_ = true;
    work_thread_ = new boost::thread(boost::bind(&AdapterGuard::Guard,this));
  }
}

void wmdf::AdapterGuard::Shutdown()
{
  if (isworking_)
  {
    isworking_ = false;
    work_thread_->join();
  }
  if (work_thread_ != NULL)
  {
    delete work_thread_;
    work_thread_ = NULL;
  }
}

void wmdf::AdapterGuard::Guard()
{
  while(isworking_)
  {
    //检查Adapter工作情况
    GuardAdapters();
    Utils::SleepTime(1000);
  }
}

void wmdf::AdapterGuard::GuardAdapters()
{
  for (int32_t index=0;index<ServerEnvironment::dispatching_center()->adapters_count();index++)
  {
    IProductAdapter* adapter = ServerEnvironment::dispatching_center()->adapters(index);
    int32_t curr_time = DateTime::time2hms(DateTime::get_current_time());
    if (adapter->product_source()->enabled())
    {

      if (adapter->is_connected() && (adapter->last_heart_beat_time() > 0))
      {
        time_stamp_t curr_time = DateTime::get_current_time();
        time_stamp_t time_span = curr_time - adapter->last_heart_beat_time();

        //如果超过60S接收不到心跳包则断开连接
        if (time_span > 60000)
        {
           Logger::instance()->WriteFormatLog(LL_WARN,"heartbeat (%d, %d)."
             ,DateTime::time2hms(curr_time),DateTime::time2hms(adapter->last_heart_beat_time()));
           if(adapter->receive_size()==adapter->pre_receive_size())
             adapter->Stop();
           else
             adapter->set_pre_receive_size(adapter->receive_size());
        }
      }


      //在交易时间之内，如发现断链，则发起重连
      if (adapter->is_market_time(curr_time))
      {
        if (!adapter->is_connected())
        {
          adapter->Stop();
          Utils::SleepTime(100);
          adapter->Start();
          Utils::SleepTime(100);
        }
      }
      //不在交易时间之内，断开与源的连接
      else
      {
        if (adapter->is_connected())
        {
          adapter->Stop();
          Utils::SleepTime(100);
        }
      }
    }
  }
}

wmdf::AdapterGuard* wmdf::AdapterGuard::instance()
{
  return instance_;
}

