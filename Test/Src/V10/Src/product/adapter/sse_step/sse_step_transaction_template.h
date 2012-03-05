#ifndef SSE_STEP_TRANSACTION_TEMPLATE_H
#define SSE_STEP_TRANSACTION_TEMPLATE_H

#include "../common/step_template.h"


namespace wmdf
{
	class SSEStepTransactionTemplate : public StepTemplate 
	{
	public:
		SSEStepTransactionTemplate();
		~SSEStepTransactionTemplate();
		virtual WindStepMessage* DecodeRaw(uint8_t* message_start,uint8_t* message_end);
	};
}

#endif
