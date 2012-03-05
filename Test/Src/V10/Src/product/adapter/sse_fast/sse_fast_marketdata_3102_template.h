#ifndef SSE_FAST_MARKETDATA_3102_TEMPLATE_H
#define SSE_FAST_MARKETDATA_3102_TEMPLATE_H

#include "../common/fast_template.h"

namespace wmdf
{
	class SSEFastMarketData3102Template : public FastTemplate
	{
	public:
		SSEFastMarketData3102Template();
		~SSEFastMarketData3102Template();
		WindFastMessage* DecodeRaw(uint8_t* raw_data,uint32_t len);
	private:
		static const int32_t item_count_ = 26;
	};
}


#endif
