#ifndef SSE_FAST_TRANSACTION_3101_TEMPLATE_H
#define SSE_FAST_TRANSACTION_3101_TEMPLATE_H

#include "../common/fast_template.h"

namespace wmdf
{
	class SSEFastTransaction3101Template : public FastTemplate
	{
	public:
		SSEFastTransaction3101Template();
		~SSEFastTransaction3101Template();
		WindFastMessage* DecodeRaw(uint8_t* raw_data,uint32_t len);
	private:
		static const int32_t item_count_ = 9;
	};
}


#endif
