#ifndef ADAPTER_GUARD_H
#define ADAPTER_GUARD_H

#include <boost/thread.hpp>

namespace wmdf
{
  class AdapterGuard
  {
  public:
    static AdapterGuard* instance();
    void Startup();
    void Shutdown();
  private:
    AdapterGuard(void);
    virtual ~AdapterGuard(void);
    void Guard();
  private:
    void GuardAdapters();
  private:
    boost::thread* work_thread_;
    volatile bool isworking_;
    static AdapterGuard* instance_;     
  };
}

#endif
