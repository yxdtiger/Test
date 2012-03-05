#ifndef SZSE_STEP_ORDERS_TEMPLATE_H
#define SZSE_STEP_ORDERS_TEMPLATE_H


#include "../common/step_template.h"


namespace wmdf
{
	class SZSEStepOrdersTemplate : public StepTemplate 
	{
	public:
		SZSEStepOrdersTemplate();
		~SZSEStepOrdersTemplate();
		virtual WindStepMessage* DecodeRaw(uint8_t* message_start,uint8_t* message_end);
	};

}

#endif



