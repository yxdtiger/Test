#ifndef SSE_STEP_MARKETDATA_TEMPLATE_H
#define SSE_STEP_MARKETDATA_TEMPLATE_H

#include "../common/step_template.h"

namespace wmdf
{
	class SSEStepMarketDataTemplate : public StepTemplate
	{
	public:
		SSEStepMarketDataTemplate();
		~SSEStepMarketDataTemplate();
		virtual WindStepMessage* DecodeRaw(uint8_t* message_start,uint8_t* message_end);
	};

}

#endif
