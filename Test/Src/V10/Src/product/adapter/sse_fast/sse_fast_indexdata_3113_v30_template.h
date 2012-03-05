#ifndef SSE_FAST_INDEXDATA_3113_V30_TEMPLATE_H
#define SSE_FAST_INDEXDATA_3113_V30_TEMPLATE_H

#include "../common/fast_template.h"
#include <map>

namespace wmdf
{
	class SSEFastIndexData3113_V30Template : public FastTemplate
	{
	public:
		SSEFastIndexData3113_V30Template();
		~SSEFastIndexData3113_V30Template();
		WindFastMessage* DecodeRaw( uint8_t* raw_data,uint32_t len);
	private:
		std::map<std::string,int64_t> price_map_;
		static const int32_t item_count_ = 13;
	};
}


#endif
