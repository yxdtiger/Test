#ifndef SSEL2_ADAPTER_H
#define SSEL2_ADAPTER_H

#include "sseszse_adapter.h"


namespace wmdf
{
  class ProductSource;
  class SecurityContainerManager;
//  class MarketSnapshotContainer;

  class Ssel2Adapter : public SseSzseAdapter
  {
  public:
    Ssel2Adapter(ProductSource* product_source);
    virtual ~Ssel2Adapter();
  private:
    virtual void LaunchMarketSnapshotL2(SecurityContainerManager* scm);
    virtual void LaunchL2Transactions(SecurityContainerManager* scm);
    virtual void LaunchOrderQueueL2(SecurityContainerManager* scm);
    virtual void LaunchSZSEL2Orders(SecurityContainerManager* scm);
//    MarketSnapshotContainer* market_snapshot_container_;
  };
}


#endif
