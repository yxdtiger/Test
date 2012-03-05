#ifndef MONITOR_STATE_H
#define MONITOR_STATE_H

namespace wmdf
{
  class MonitorState
  {
  public:
    MonitorState(void);
    virtual ~MonitorState(void);
    bool monitoring() { return monitoring_;} 
    void set_monitoring(bool monitoring);
    bool logging() { return logging_;} 
    void set_logging(bool logging);

  private:
    volatile bool monitoring_;
    volatile bool logging_;
  };
}

#endif
