#ifndef IEVENT_MONITOR_H
#define IEVENT_MONITOR_H

#include "../include/wmdf_api.h"
#include "../include/date_time.h"

namespace wmdf
{
  class IMonitorEventCollector
  {
  public:
    IMonitorEventCollector(void);
    virtual ~IMonitorEventCollector(void);
    virtual void AddEvent(int8_t Level,date_time_t date_time, std::string at_where,int32_t event_id,std::string what_happened) = 0;
  };
}

#endif
