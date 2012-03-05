#ifndef SWZS_ADAPTER_H
#define SWZS_ADAPTER_H
#include "dbf_adapter.h"

namespace wmdf
{
#define SWZS_WINDCODE_INDEX 0
#define SWZS_PRECLOSE_INDEX 2
#define SWZS_OPEN_INDEX 3
#define SWZS_TOTALAMOUNT_INDEX 4
#define SWZS_HIGH_INDEX 5
#define SWZS_LOW_INDEX 6
#define SWZS_NEW_INDEX 7
#define SWZS_TOTALVOLUME_INDEX 10
#define SWZS_FIELD_NUMBER 22

	class SWIndexAdapter:public DBFAdapter
	{
	public:
		SWIndexAdapter(ProductSource* product_source);
		virtual ~SWIndexAdapter(void);

		virtual wmdf::MdfMessage* CreateMDFMessage(int index,int count);
		virtual void* FetchARecord();
	private:
		const static char *swzs_suffix_;
	};
}

#endif
