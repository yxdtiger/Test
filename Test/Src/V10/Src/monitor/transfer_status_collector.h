#ifndef TRANSFER_STATUS_LISTENER_H
#define TRANSFER_STATUS_COLLECTOR_H

#include <map>
#include <boost/thread/mutex.hpp>
#include "../include/wmdf_api.h"
#include "../include/wmdf_structs.h"


namespace wmdf
{
  class TransferStatusCollector
  {
  public:
    TransferStatusCollector(void);
    virtual ~TransferStatusCollector(void);
    void CollectTransferStatus(time_stamp_t time_span);
    void PrintTransferStatus();
    void PublishTransferStatus();
  private:
    TransferDataStat* FetchTransferDataStat( int16_t product_id );
    TransferDataStat* FindTransferDataStat( int16_t product_id );

    std::map<int16_t,TransferDataStat*> transferStat_;
    boost::mutex mutex_;
  };
}

#endif
