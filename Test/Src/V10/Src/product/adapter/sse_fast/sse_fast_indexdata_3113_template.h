#ifndef SSE_FAST_INDEXDATA_3113_TEMPLATE_H
#define SSE_FAST_INDEXDATA_3113_TEMPLATE_H

#include "../common/fast_template.h"

namespace wmdf
{
	class SSEFastIndexData3113Template : public FastTemplate
	{
	public:
		SSEFastIndexData3113Template();
		~SSEFastIndexData3113Template();
		WindFastMessage* DecodeRaw( uint8_t* raw_data,uint32_t len);
	private:
		static const int32_t item_count_ = 12;
	};
}


#endif
