#ifndef PRODUCT_ADAPTER_FACTORY_H
#define PRODUCT_ADAPTER_FACTORY_H


#include "../include/date_time.h"


namespace wmdf
{
  class ProductSource;
  class IProductAdapter;

  class ProductAdapterFactory
  {
  public:
    ProductAdapterFactory(void);
    virtual ~ProductAdapterFactory(void);
    static void InitializeDispathingCenter();//在系统初始化时调用，初始化最新版本的adapter
    static IProductAdapter* CreateDispatcher(ProductSource* product_source,date_t date);
  private:
    
  };
}

#endif
