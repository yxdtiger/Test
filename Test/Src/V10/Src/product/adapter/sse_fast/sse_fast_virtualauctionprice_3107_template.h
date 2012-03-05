#ifndef SSE_FAST_VIRTUALAUCTIONPRICE_3107_TEMPLATE_H
#define SSE_FAST_VIRTUALAUCTIONPRICE_3107_TEMPLATE_H

#include "../common/fast_template.h"

namespace wmdf
{
	class SSEFastVirtualAuctionPrice3107Template : public FastTemplate
	{
	public:
		SSEFastVirtualAuctionPrice3107Template();
		~SSEFastVirtualAuctionPrice3107Template();
		WindFastMessage* DecodeRaw(uint8_t* raw_data,uint32_t len );
	private:
		static const int32_t item_count_ = 8;
	};
}


#endif
