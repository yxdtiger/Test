#ifndef WATCH_DOG_H
#define WATCH_DOG_H
#include <boost/thread.hpp>
#include "net_info_collection.h"
#include "../common/studio_utility.h"

namespace wmdf
{
  class WatchDog
  {
  public:
    WatchDog(int time_span);
    virtual ~WatchDog(void);
    void StartWork();
    void Stop();
    bool IsWorking(){return is_working_;}
  protected:
    void DoWatch();
    void Alarm();
    
    
  private:
    NetInfoCollection* pre_collection;
    NetInfoCollection* cur_collection;
    int time_span_;
    bool is_working_;    
    boost::thread* work_thread_;
  };
}

#endif
