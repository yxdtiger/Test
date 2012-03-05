#include "monitor_adapter.h"
#include "../environment/utils.h"
#include "transfer_status_collector.h"
#include "client_status_collector.h"
#include "../include/date_time.h"
#include "../environment/server_environment.h"
#include "../environment/system_config.h"
#include "monitor_event_collector.h"

wmdf::MonitorAdapter::MonitorAdapter(void)
{
  is_working_ = false;
  transfer_status_collector_ = new TransferStatusCollector();
  client_status_collector_ = new ClientStatusCollector();
}

wmdf::MonitorAdapter::~MonitorAdapter(void)
{
}

void wmdf::MonitorAdapter::Start()
{
  if (!is_working_)
  {
    is_working_ = true;
    work_thread_ = new boost::thread(boost::bind(&MonitorAdapter::Monitor,this));
  }
}

void wmdf::MonitorAdapter::Stop()
{
  if (is_working_)
  {
    is_working_ = false;
    work_thread_->join();
  }
}

void wmdf::MonitorAdapter::Monitor()
{
  time_stamp_t time_span = 0;
  time_stamp_t pre_time = 0;
  while(is_working_)
  {
    pre_time = DateTime::get_current_time();
    Utils::SleepTime(ServerEnvironment::system_config()->monitor_interval());
    time_span = DateTime::calctimespan(DateTime::get_current_time(),pre_time);

    //收集传输状态
    transfer_status_collector_->CollectTransferStatus(time_span);
    transfer_status_collector_->PublishTransferStatus();

    //收集客户端状态
    client_status_collector_->CollectClientStatus(time_span);
    client_status_collector_->PublishClientStatus();

    //发布监控事件
    MonitorEventCollector::instance()->PublishEvents();
  }

}
