#ifndef SSE_FAST_INDEXDATA_1113_TEMPLATE_H
#define SSE_FAST_INDEXDATA_1113_TEMPLATE_H

#include "../common/fast_template.h"

namespace wmdf
{
	class SSEFastIndexData1113Template : public FastTemplate
	{
	public:
		SSEFastIndexData1113Template();
		~SSEFastIndexData1113Template();
		WindFastMessage* DecodeRaw(uint8_t* raw_data,uint32_t len);
	private:
		static const int32_t item_count_ = 11;
	};
}

#endif
