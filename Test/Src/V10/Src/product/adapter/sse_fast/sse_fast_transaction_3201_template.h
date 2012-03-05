#ifndef SSE_FAST_TRANSACTION_3201_TEMPLATE_H
#define SSE_FAST_TRANSACTION_3201_TEMPLATE_H

#include "../common/fast_template.h"

namespace wmdf
{
	class SSEFastTransaction3201Template : public FastTemplate
	{
	public:
		SSEFastTransaction3201Template();
		~SSEFastTransaction3201Template();
		WindFastMessage* DecodeRaw(uint8_t* raw_data,uint32_t len );
	private:
		static const int32_t item_count_ = 9;
	};
}


#endif
