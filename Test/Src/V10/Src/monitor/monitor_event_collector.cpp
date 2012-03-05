#include "monitor_event_collector.h"
#include "../environment/utils.h"
#include "../protocol/mdf_message.h"
#include "../include/date_time.h"
#include "../session/message_publisher.h"

wmdf::MonitorEventCollector* wmdf::MonitorEventCollector::instance_ = new wmdf::MonitorEventCollector();

wmdf::MonitorEventCollector::MonitorEventCollector(void)
{
}

wmdf::MonitorEventCollector::~MonitorEventCollector(void)
{
  Clear();
}

void wmdf::MonitorEventCollector::AddEvent( int8_t level,date_time_t date_time, std::string at_where,int32_t event_id,std::string what_happened )
{
  boost::mutex::scoped_lock lock(event_mutex_);
  MonitorEvent* monitor_event = new MonitorEvent();
  memset(monitor_event,0,sizeof(MonitorEvent));
  monitor_event->level  = level;
  monitor_event->date_time = date_time;
  monitor_event->event_id = event_id;
  uint32_t at_where_len = at_where.length() < 60 ? at_where.length() :59;
  memcpy(monitor_event->at_where,at_where.c_str(),at_where_len);
  uint32_t what_happened_len = what_happened.length()  < 150 ? what_happened.length() :149;
  memcpy(monitor_event->what_happened,what_happened.c_str(),what_happened_len);

  if (events_.size() > max_event_num_)
  {
    Clear();
  }
  events_.push_back(monitor_event);
}

void wmdf::MonitorEventCollector::PublishEvents()
{
  boost::mutex::scoped_lock lock(event_mutex_);
  if (events_.size() <= 0) return;
  MonitorEvent* monitor_events = new MonitorEvent[events_.size()];
  for (uint32_t index=0;index<events_.size();index++)
  {
    memcpy(&monitor_events[index],events_[index],sizeof(MonitorEvent));
  }
  MdfMessage* message = new MdfMessage();
  message->set_local_time(DateTime::get_current_time());
  message->set_packet_num(0);
  message->set_socket_id(0);
  message->set_product_id(PRODEUCT_MONITOR_ID);
  message->set_message_id(MONITOR_EVENT_ID);
  message->set_major_version(MAJOR_VERSION_MONITOR_ID);
  message->set_minor_version(MINOR_VERSION_MONITOR_ID);
  message->set_header(NULL);
  message->set_record_count((int32_t)events_.size());
  message->set_body(monitor_events);

  SharedMdfMessage shared_mdf_message(message);
  MessagePublisher::PublishMonitorMessage(shared_mdf_message);
  Clear();
}

wmdf::MonitorEventCollector* wmdf::MonitorEventCollector::instance()
{
  return instance_;
}

void wmdf::MonitorEventCollector::Clear()
{
  for (uint32_t index=0;index<events_.size();index++)
  {
    delete events_[index];
    events_[index] = NULL;
  }
  events_.clear();
}
