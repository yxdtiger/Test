#ifndef EVENT_SERVER_H
#define EVENT_SERVER_H

#include <boost/thread.hpp>

namespace wmdf
{
  class TransferStatusCollector;
  class ClientStatusCollector;
  class ServerStatusCollector;
  class MonitorAdapter
  {
  public:
    MonitorAdapter(void);
    virtual ~MonitorAdapter(void);
    void Start();
    void Stop();
    TransferStatusCollector* transfer_status_collector(){return transfer_status_collector_;}
    ClientStatusCollector* client_status_collector(){return client_status_collector_;}
  private:
    void Monitor();
    bool is_working_;
    boost::thread* work_thread_;
    TransferStatusCollector* transfer_status_collector_;
    ClientStatusCollector* client_status_collector_;
  };
}

#endif
