#ifndef SZSE_STEP_TEMPLATE_DECODER_H
#define SZSE_STEP_TEMPLATE_DECODER_H


#include "../common/step_template_decoder.h"

namespace wmdf
{
	class StepTemplate;
	class SZSEStepTransactionTemplate;
	class SZSEStepMarketDataTemplate;
	class SZSEStepIndexDataTemplate;
	class SZSEStepOrdersTemplate;

	class SZSEStepTemplateDecoder : public StepTemplateDecoder
	{
	public:
		SZSEStepTemplateDecoder();
		~SZSEStepTemplateDecoder();

    virtual void DecodeMessage(uint8_t* tid,uint8_t* message_start,uint8_t* message_end);

	private:
		SZSEStepTransactionTemplate *szse_step_transaction_template_;
		SZSEStepMarketDataTemplate *szse_step_marketdata_template_;
		SZSEStepIndexDataTemplate *szse_step_indexdata_template_;
		SZSEStepOrdersTemplate *szse_step_orders_template_;
	};
}



#endif
