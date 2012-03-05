#ifndef SZSEL1_ADAPTER_H
#define SZSEL1_ADAPTER_H


#include "sseszse_adapter.h"


namespace wmdf
{
  class Szsel1Adapter : public SseSzseAdapter
  {
  public:
    Szsel1Adapter(ProductSource* product_source);
    virtual ~Szsel1Adapter();
  private:
    virtual void LaunchMarketSnapshotL2(SecurityContainerManager* scm);
    virtual void LaunchL2Transactions(SecurityContainerManager* scm);
    virtual void LaunchOrderQueueL2(SecurityContainerManager* scm);
    virtual void LaunchSZSEL2Orders(SecurityContainerManager* scm);
  };
}


#endif
