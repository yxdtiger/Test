#include "product_adapter_factory.h"
#include <assert.h>
#include "../product/product_source_manager.h"
#include "../product/product_source.h"
#include "../environment/server_environment.h"
#include "../product/adapter/adapter_guard.h"
#include "../product/adapter/sse_szse/ssel1/ssel1_adapter.h"
#include "../product/adapter/sse_szse/szsel1/szsel1_adapter.h"
#include "../product/adapter/sse_szse/ssel2/ssel2_adapter.h"
#include "../product/adapter/sse_szse/szsel2/szsel2_adapter.h"
#include "../product/adapter/cffe_adapter.h"
#include "../product/adapter/shfe_adapter.h"
#include "../product/adapter/czce_adapter.h"
#include "../product/adapter/dce_adapter.h"
#include "../product/adapter/fx_adapter.h"
#include "../product/adapter/ftse_adapter.h"
#include "../product/adapter/simulator_adapter.h"
#include "../product/adapter/relay_adapter.h"
#include "../product/adapter/swzs_adapter.h"
#include "../product/adapter/csindex/csindex_adapter.h"
#include "../product/adapter/ciq/ciq_adapter.h"
#include "../product/adapter/windindex/windindex_adapter.h"
#include "../product/adapter/common/rawdata_adapter_factory.h"
#include "../session/dispatching_center.h"


wmdf::ProductAdapterFactory::ProductAdapterFactory(void)
{
}

wmdf::ProductAdapterFactory::~ProductAdapterFactory(void)
{
}

void wmdf::ProductAdapterFactory::InitializeDispathingCenter()
{
  ProductSourceManager* psm = ServerEnvironment::product_source_manager();
  for (int32_t index=0;index<psm->product_source_count();index++)
  {
    ProductSource* product_source = psm->product_sources(index);
    if (!product_source->enabled()) {continue;}
		IProductAdapter* adapter = CreateDispatcher(product_source,DateTime::today());
    assert(adapter != NULL);
    ServerEnvironment::dispatching_center()->AddAdapter(adapter);
  }
}

wmdf::IProductAdapter* wmdf::ProductAdapterFactory::CreateDispatcher( ProductSource* product_source,date_t date )
{
  //为relay 单独定义了一个产品，发布方式及缓存和普通的产品数据不同。
  if ((product_source->product_id() >= PRODUCT_RELAY_BEGIN_ID)
    && (product_source->product_id() <= PRODUCT_RELAY_END_ID))
  {
    return new RelayAdapter(product_source);
  }
  else
  {
    switch(product_source->product_id())
    {
      //基础传输产品
      //case PRODEUCT_TRANSFER_ID:
      //监控产品
      //case PRODEUCT_MONITOR_ID:
      //  break;
      //上交所Level1 深交所Level1
    case PRODUCT_SSE_LEVEL1_ID:
      return new Ssel1Adapter(product_source);
    case PRODUCT_SZSE_LEVEL1_ID:
      return new SZSEL1Adapter(product_source);
      ////上交所Level2
    case PRODUCT_SSE_LEVEL2_ID:
      {
        if (product_source->codec_type()==CT_RAWDATA)
					return RawdataAdapterFactory::CreateSSERawdataAdapter(product_source,date);
        else if (product_source->codec_type()==CT_FEEDHANDLER)
          return new SSEL2Adapter(product_source);
        else return NULL;
      }
      //深交所Level2
    case PRODUCT_SZSE_LEVEL2_ID:
			{
				if(product_source->codec_type()==CT_FEEDHANDLER)
					return new SZSEL2Adapter(product_source);
				if(product_source->codec_type()==CT_RAWDATA)
					return RawdataAdapterFactory::CreateSZSERawdataAdapter(product_source,date);
				else return NULL;
			}
      //中金所Level1
    case PRODUCT_CFFE_LEVEL1_ID:
      return new CffeL2Adapter(product_source);
      //中金所Level2
    case PRODUCT_CFFE_LEVEL2_ID:
      return new CffeL2Adapter(product_source);
      //上期所Level1
    case PRODUCT_SHFE_LEVEL1_ID:
      return new ShfeAdapter(product_source);
      //大商所Level1
    case PRODUCT_DCE_LEVEL1_ID:
      return new DceAdapter(product_source);
      //郑商所Level1
    case PRODUCT_CZCE_LEVEL1_ID:
      return new CzceAdapter(product_source);
    case PRODUCT_FX_ID:
      return new FxAdapter(product_source);
    case PRODUCT_FTSE_LEVEL1_ID:
      return new FTSEAdapter(product_source);
    case PRODUCT_SIMULATOR_ID:
      return new SimulatorAdapter(product_source);
		case PRODUCT_SWINDEX_LEVEL1_ID:
			return new SWIndexAdapter(product_source);
		case PRODUCT_CSINDEX_LEVEL1_ID:
			return new CSIndexAdapter(product_source);
		case PRODUCT_CIQINDEX_LEVEL1_ID:
			return new CIQAdapter(product_source);
		case PRODUCT_WINDINDEX_LEVEL1_ID:
			return new WindIndexAdapter(product_source);
    default:
      return NULL;
    }
  }
  return NULL;
}

