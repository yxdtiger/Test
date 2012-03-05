#ifndef EVENT_LISTENER_H
#define EVENT_LISTENER_H

namespace wmdf
{
  class FeedEvent;
  class EventListener
  {
  public:
    EventListener(void);
    virtual ~EventListener(void);
    virtual void HandleEvent(FeedEvent* feed_event)=0;
  };
}

#endif
