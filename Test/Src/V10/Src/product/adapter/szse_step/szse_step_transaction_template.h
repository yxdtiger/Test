#ifndef SZSE_STEP_TRANSACTION_TEMPLATE_H
#define SZSE_STEP_TRANSACTION_TEMPLATE_H

#include "../common/step_template.h"


namespace wmdf
{
	class SZSEStepTransactionTemplate : public StepTemplate 
	{
	public:
		SZSEStepTransactionTemplate();
		~SZSEStepTransactionTemplate();
		virtual WindStepMessage* DecodeRaw(uint8_t* message_start,uint8_t* message_end);
	};
}

#endif
