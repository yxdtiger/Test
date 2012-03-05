#ifndef SSE_STEP_ORDERQUEUE_TEMPLATE_H
#define SSE_STEP_ORDERQUEUE_TEMPLATE_H

#include "../common/step_template.h"

namespace wmdf
{
	class SSEStepOrderQueueTemplate : public StepTemplate 
	{
	public:
		SSEStepOrderQueueTemplate();
		~SSEStepOrderQueueTemplate();
		virtual WindStepMessage* DecodeRaw(uint8_t* message_start,uint8_t* message_end);
	};
}

#endif



