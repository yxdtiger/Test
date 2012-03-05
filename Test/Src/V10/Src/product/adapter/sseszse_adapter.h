#ifndef SSESZSE_ADAPTER_H
#define SSESZSE_ADAPTER_H

#include "product_adapter.h"

namespace wmdf
{
  class DataDecoder;
  class SecurityContainerManager;

  class SseSzseAdapter : public ProductAdapter
  {
  public:
    SseSzseAdapter(ProductSource* product_source);
    virtual ~SseSzseAdapter();
  protected:
    virtual bool AtLeastOne(const uint32_t length,uint32_t& first_pos,uint32_t& count);
    virtual uint32_t ParseMessage(uint32_t start_pos,uint32_t count);    
  protected:
    virtual void LaunchMarketSnapshotL2(SecurityContainerManager* scm)=0;
    virtual void LaunchL2Transactions(SecurityContainerManager* scm)=0;
    virtual void LaunchOrderQueueL2(SecurityContainerManager* scm)=0;
    virtual void LaunchSZSEL2Orders(SecurityContainerManager* scm)=0;

  private:
    DataDecoder* data_decoder_;
    uint8_t buf_unzip[1024*512];
  };
}

#endif
