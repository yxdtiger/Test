#ifndef SSE_FAST_MARKETDATA_3202_TEMPLATE_H
#define SSE_FAST_MARKETDATA_3202_TEMPLATE_H

#include "../common/fast_template.h"

namespace wmdf
{
	class SSEFastMarketData3202Template : public FastTemplate
	{
	public:
		SSEFastMarketData3202Template();
		~SSEFastMarketData3202Template();
		WindFastMessage* DecodeRaw(uint8_t* raw_data,uint32_t len);
	private:
		static const int32_t item_count_ = 27;
	};
}


#endif
