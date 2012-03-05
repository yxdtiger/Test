#ifndef SSEL1_ADAPTER_H
#define SSEL1_ADAPTER_H


#include "sseszse_adapter.h"


namespace wmdf
{
  class SecurityContainerManager;

  class Ssel1Adapter : public SseSzseAdapter
  {
  public:
    Ssel1Adapter(ProductSource* product_source);
    virtual ~Ssel1Adapter();
  private:
    virtual void LaunchMarketSnapshotL2(SecurityContainerManager* scm);
    virtual void LaunchL2Transactions(SecurityContainerManager* scm);
    virtual void LaunchOrderQueueL2(SecurityContainerManager* scm);
    virtual void LaunchSZSEL2Orders(SecurityContainerManager* scm);
  };
}


#endif
