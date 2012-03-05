#include "relay_product_adapter_factory.h"
#include <assert.h>
#include "../product/product_source_manager.h"
#include "../product/product_source.h"
#include "../environment/server_environment.h"
#include "../product/adapter/simulator_adapter.h"
#include "../product/adapter/relay_adapter.h"
#include "../session/dispatching_center.h"

wmdf::RelayProductAdapterFactory::RelayProductAdapterFactory(void)
{
}

wmdf::RelayProductAdapterFactory::~RelayProductAdapterFactory(void)
{
}



void wmdf::RelayProductAdapterFactory::InitializeDispathingCenter()
{
  ProductSourceManager* psm = ServerEnvironment::product_source_manager();
  for (int32_t index=0;index<psm->product_source_count();index++)
  {
    ProductSource* product_source = psm->product_sources(index);

    if (!product_source->enabled()) {continue;}

    //为relay 单独定义了一个产品，发布方式及缓存和普通的产品数据不同。
    if ((product_source->product_id() >= PRODUCT_RELAY_BEGIN_ID) 
      && (product_source->product_id() <= PRODUCT_RELAY_END_ID))
    {
      IProductAdapter* adapter =  new RelayAdapter(product_source);
      ServerEnvironment::dispatching_center()->AddAdapter(adapter);   
    } 
    else if (product_source->product_id() == PRODUCT_SIMULATOR_ID)
    {
      IProductAdapter* adapter = new SimulatorAdapter(product_source);
      ServerEnvironment::dispatching_center()->AddAdapter(adapter);
    }  
  }
}

