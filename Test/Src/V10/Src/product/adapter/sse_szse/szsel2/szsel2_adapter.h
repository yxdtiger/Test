#ifndef SZSEL2_ADAPTER_H
#define SZSEL2_ADAPTER_H


#include "../sseszse_adapter.h"


namespace wmdf
{

  class SZSEL2Adapter : public SSESZSEAdapter
  {
  public:
    SZSEL2Adapter(ProductSource* product_source);
    virtual ~SZSEL2Adapter();
  private:
    virtual void LaunchMarketSnapshotL2(SecurityContainerManager* scm);
    virtual void LaunchL2Transactions(SecurityContainerManager* scm);
    virtual void LaunchOrderQueueL2(SecurityContainerManager* scm);
    virtual void LaunchSZSEL2Orders(SecurityContainerManager* scm);
  };
}



#endif
