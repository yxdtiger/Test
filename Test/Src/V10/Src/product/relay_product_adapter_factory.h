#ifndef RELAY_PRODUCT_ADAPTER_FACTORY_H
#define RELAY_PRODUCT_ADAPTER_FACTORY_H

namespace wmdf
{
  class RelayProductAdapterFactory
  {
  public:
    RelayProductAdapterFactory(void);
    virtual ~RelayProductAdapterFactory(void);
    static void InitializeDispathingCenter();
  };
}

#endif
