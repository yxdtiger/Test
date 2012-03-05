#ifndef MONITOR_ENVIRONMENT_H
#define MONITOR_ENVIRONMENT_H
#include <string>
#include "trading_time_manager.h"
#include "monitor_invoke.h"
#include "watch_dog.h"

namespace wmdf
{  
  class MonitorEnvironment
  {
  public:
    void Initialize();
    void Finalize();
    bool IsFileExist(const char * path); 
    static MonitorEnvironment* Instance();
    
    void StartMonitor();
    void EndMonitor();

    void StartMonitorOne(int32_t conn_id);
    void StopMonitorOne(int32_t conn_id);

    bool is_sound_enable() const {return is_sound_enable_;}
    void set_is_sound_enable(bool flag){is_sound_enable_=flag;} 

    TradingTimeManager* trading_time_manager(){return trading_time_manager_;}
    
  private:
    MonitorEnvironment(void);
    virtual ~MonitorEnvironment(void);

    MonitorInvoke* invoker_;
    WatchDog* watch_dog_;
    bool is_sound_enable_;
    TradingTimeManager* trading_time_manager_; 

    static MonitorEnvironment* instance_;
  };
}

#endif
