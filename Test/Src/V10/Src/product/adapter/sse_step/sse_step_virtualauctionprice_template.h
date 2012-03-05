#ifndef SSE_STEP_VIRTUALAUCTIONPRICE_TEMPLATE_H
#define SSE_STEP_VIRTUALAUCTIONPRICE_TEMPLATE_H


#include "../common/step_template.h"


namespace wmdf
{
	class SSEStepVirtualAuctionPriceTemplate : public StepTemplate 
	{
	public:
		SSEStepVirtualAuctionPriceTemplate();
		~SSEStepVirtualAuctionPriceTemplate();
		virtual WindStepMessage* DecodeRaw(uint8_t* message_start,uint8_t* message_end);
	};

}


#endif



