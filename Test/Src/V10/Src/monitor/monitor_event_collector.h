#ifndef EVENT_POOL_H
#define EVENT_POOL_H

#include <vector>
#include <boost/thread/mutex.hpp>
#include "../include/wmdf_structs.h"
#include "imonitor_event_collector.h"

namespace wmdf
{
  class MonitorEventCollector: public IMonitorEventCollector
  {
  public:
    static MonitorEventCollector* instance();
    virtual void AddEvent(int8_t Level,date_time_t date_time, std::string at_where,int32_t event_id,std::string what_happened);
    void PublishEvents();
  private:
    MonitorEventCollector(void);
    virtual ~MonitorEventCollector(void);
    void Clear();
  private:
    std::vector<MonitorEvent*> events_;
    boost::mutex event_mutex_;
    static const uint32_t max_event_num_ = 1000;
    static MonitorEventCollector* instance_;
  };
}

#endif
