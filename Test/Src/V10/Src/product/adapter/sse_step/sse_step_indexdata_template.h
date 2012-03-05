#ifndef SSE_STEP_INDEXDATA_TEMPLATE_H
#define SSE_STEP_INDEXDATA_TEMPLATE_H


#include "../common/step_template.h"


namespace wmdf
{
	class SSEStepIndexDataTemplate : public StepTemplate 
	{
	public:
		SSEStepIndexDataTemplate();
		~SSEStepIndexDataTemplate();
		virtual WindStepMessage* DecodeRaw(uint8_t* message_start,uint8_t* message_end);
	};

}


#endif



