#ifndef RAWDATA_ADAPTER_FACTORY_H
#define RAWDATA_ADAPTER_FACTORY_H

#include "../../../include/wmdf_api.h"


namespace wmdf
{
  class ProductSource;
  class IProductAdapter;

	class RawdataAdapterFactory
	{
	public:
		RawdataAdapterFactory();
		~RawdataAdapterFactory();
		static IProductAdapter* CreateSZSERawdataAdapter(ProductSource* product_source, date_t date);
		static IProductAdapter* CreateSSERawdataAdapter(ProductSource* product_source,date_t date);
	private:
	};
}


#endif
