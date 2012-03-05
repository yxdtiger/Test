#ifndef SZSE_FAST_RAWDATA_CONVERTER_H
#define SZSE_FAST_RAWDATA_CONVERTER_H

#include "../common/rawdata_object.h"
#include "../../../include/wmdf_structs.h"
#include <map>
#include <string>

namespace wmdf
{
	class SZSEFastRawDataConverter
	{
	public:
		SZSEFastRawDataConverter();
		~SZSEFastRawDataConverter();
		void convertMarketsnapshot(WindFastMessage*,uint32_t &pack_id);
		void convertOrderqueue(WindFastMessage*,uint32_t &pack_id);
		void convertTransaction(WindFastMessage*,uint32_t &pack_id);
		void convertIndexData(WindFastMessage*,uint32_t &pack_id);
		void convertOrders(WindFastMessage*,uint32_t &pack_id);
		void convertTradeStatus(WindFastMessage*);
		int32_t Orderqueue_count(){return num_of_orderqueue_;}

	private:
		std::map<std::string,int8_t> trade_status_;
		static const char* szse_suffix;
		int32_t num_of_orderqueue_;

	};
}

#endif
