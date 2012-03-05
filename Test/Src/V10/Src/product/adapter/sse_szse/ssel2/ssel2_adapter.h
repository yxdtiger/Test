#ifndef SSEL2_ADAPTER_H
#define SSEL2_ADAPTER_H

#include "../sseszse_adapter.h"


namespace wmdf
{
  class ProductSource;
  class SecurityContainerManager;

  class SSEL2Adapter : public SSESZSEAdapter
  {
  public:
    SSEL2Adapter(ProductSource* product_source);
    virtual ~SSEL2Adapter();
  private:
    virtual void LaunchMarketSnapshotL2(SecurityContainerManager* scm);
    virtual void LaunchL2Transactions(SecurityContainerManager* scm);
    virtual void LaunchOrderQueueL2(SecurityContainerManager* scm);
    virtual void LaunchSZSEL2Orders(SecurityContainerManager* scm);
//    MarketSnapshotContainer* market_snapshot_container_;
  };
}


#endif
