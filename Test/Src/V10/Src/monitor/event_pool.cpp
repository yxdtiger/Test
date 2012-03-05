#include "event_pool.h"
#include "event_listener.h"
#include "feed_event.h"
#include <boost/bind.hpp>
#include "../environment/utils.h"

wmdf::EventPool::EventPool(void)
{
  isworking_ = false;
}

wmdf::EventPool::~EventPool(void)
{
  EndListen();
  listeners_.clear();
}

void wmdf::EventPool::AddEvent( FeedEvent* feed_event )
{
  boost::mutex::scoped_lock lock(event_mutex_);
  events_.push_back(feed_event);
}

void wmdf::EventPool::RegisterListener( EventListener* listener )
{
  boost::mutex::scoped_lock lock(listener_mutex_);
  listeners_.push_back(listener);
}

void wmdf::EventPool::UnRegisterListener( EventListener* listener )
{
  boost::mutex::scoped_lock lock(listener_mutex_);
  std::vector<EventListener*>::iterator iter = listeners_.begin();
  while(iter != listeners_.end())
  {
    if (*iter == listener)
    {
      listeners_.erase(iter);
      return;
    }
    iter++;
  }
}


void wmdf::EventPool::UnRegisterAllListener()
{
  listeners_.clear();
}


void wmdf::EventPool::ProcessEvent()
{
  while(isworking_)
  {
    if (events_.size() > 0)
    {
      {
        boost::mutex::scoped_lock lock(event_mutex_);
        event_copies_.clear();
        while (events_.size()>0)
        {
          FeedEvent* feed_event = events_.front();
          events_.pop_front();
          event_copies_.push_back(feed_event);
        }        
        events_.clear();
      }
      if (event_copies_.size() > 0)
      {
        boost::mutex::scoped_lock lock(listener_mutex_);
        vector<FeedEvent*>::iterator iter = event_copies_.begin();
        while(iter != event_copies_.end())
        {
          for (uint32_t index=0;index< listeners_.size();index++)
          {
            listeners_[index]->HandleEvent(*iter);
          }
          delete *iter;
          iter++;
        }
        event_copies_.clear();
      }
    }
    else
    {
      Utils::SleepTime(1);
    }
  }
}

void wmdf::EventPool::BeginListen()
{
  if (!isworking_)
  {
    isworking_ = true;
    work_thread_ = new boost::thread(boost::bind(&EventPool::ProcessEvent,this));
  }
}

void wmdf::EventPool::EndListen()
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

bool wmdf::EventPool::IsMonitoring()
{
  return isworking_;
}