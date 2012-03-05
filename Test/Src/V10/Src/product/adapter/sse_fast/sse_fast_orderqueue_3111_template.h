#ifndef SSE_FAST_ORDERQUEUE_3111_TEMPLATE_H
#define SSE_FAST_ORDERQUEUE_3111_TEMPLATE_H

#include "../common/fast_template.h"

namespace wmdf
{
	class SSEFastOrderQueue3111Template : public FastTemplate
	{
	public:
		SSEFastOrderQueue3111Template();
		~SSEFastOrderQueue3111Template();
		WindFastMessage* DecodeRaw(uint8_t* raw_data,uint32_t len );
	private:
		static const int32_t item_count_ = 7;
	};
}

#endif
