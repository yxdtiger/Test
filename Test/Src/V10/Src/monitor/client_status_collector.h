#ifndef CLIENT_STATUS_COLLECTOR_H
#define CLIENT_STATUS_COLLECTOR_H

#include <map>
#include <vector>
#include <boost/thread/mutex.hpp>
#include "../include/wmdf_structs.h"

namespace wmdf
{
  class Subscriber;
  class ClientStatusCollector
  {
  public:
    ClientStatusCollector(void);
    virtual ~ClientStatusCollector(void);
    void CollectClientStatus(time_stamp_t time_span);
    void PrintClientStatus();
    void PublishClientStatus();
  private:
    void HandleSubscriber(Subscriber* subscriber);
    ClientStatus* FetchClientStatus( int32_t socket_id );
    bool IsSocketValid(int32_t socket_id);
  private:
    //socket_id
    std::map<int32_t,ClientStatus*> client_statuses_;
    boost::mutex mutex_;
    time_stamp_t time_span_;  //采集的时间间隔
    std::vector<int32_t> valid_socket_;

  };
}

#endif
