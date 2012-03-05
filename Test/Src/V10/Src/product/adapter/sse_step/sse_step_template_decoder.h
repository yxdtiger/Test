#ifndef SSE_STEP_TEMPLATE_DECODER_H
#define SSE_STEP_TEMPLATE_DECODER_H

#include <vector>
#include "../common/step_template_decoder.h"

namespace wmdf
{
	class SSEStepVirtualAuctionPriceTemplate;
	class SSEStepOrderQueueTemplate;
	class SSEStepMarketDataTemplate;
	class SSEStepTransactionTemplate;
	class SSEStepIndexDataTemplate;

	class SSEStepTemplateDecoder : public StepTemplateDecoder
	{
	public:
		SSEStepTemplateDecoder();
		~SSEStepTemplateDecoder();

		virtual void DecodeMessage(uint8_t* tid,uint8_t* message_start,uint8_t* message_end);
	private:
		SSEStepVirtualAuctionPriceTemplate* sse_step_virtualauctionprice_template_;
		SSEStepOrderQueueTemplate* sse_step_orderqueue_template_;
		SSEStepMarketDataTemplate* sse_step_marketdata_template_;
		SSEStepTransactionTemplate* sse_step_transaction_template_;
		SSEStepIndexDataTemplate* sse_step_indexdata_template_;


	};
}


#endif
