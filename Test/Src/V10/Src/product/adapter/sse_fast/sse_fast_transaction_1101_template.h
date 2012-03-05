#ifndef SSE_FAST_TRANSACTION_1101_TEMPLATE_H
#define SSE_FAST_TRANSACTION_1101_TEMPLATE_H

#include "../common/fast_template.h"

namespace wmdf
{
	class SSEFastTransaction1101Template : public FastTemplate
	{
	public:
		SSEFastTransaction1101Template();
		~SSEFastTransaction1101Template();
		WindFastMessage* DecodeRaw(uint8_t* raw_data,uint32_t len );
	private:
		static const int32_t item_count_ = 8;
	};
}


#endif
