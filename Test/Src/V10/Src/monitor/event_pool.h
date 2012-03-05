#ifndef EVENT_POOL_H
#define EVENT_POOL_H

#include <vector>
#include <list>
#include <boost/thread/mutex.hpp>
#include <boost/thread.hpp>

namespace wmdf
{
  class EventListener;
  class FeedEvent;

  class EventPool
  {
  public:
    EventPool(void);
    virtual ~EventPool(void);
    void AddEvent(FeedEvent* feed_event);
    void RegisterListener(EventListener* listener);
    void UnRegisterListener(EventListener* listener);
    void UnRegisterAllListener();
    void BeginListen();
    void EndListen();
    bool IsMonitoring();
  protected:
    void ProcessEvent();
  private:
    std::vector<EventListener*> listeners_;
    std::list<FeedEvent*> events_;
    std::vector<FeedEvent*> event_copies_;
    boost::mutex listener_mutex_;
    boost::mutex event_mutex_;
    boost::thread* work_thread_;
    volatile bool isworking_;
  };
}

#endif
