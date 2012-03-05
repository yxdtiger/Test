#ifndef SZSE_STEP_INDEXDATA_TEMPLATE_H
#define SZSE_STEP_INDEXDATA_TEMPLATE_H


#include "../common/step_template.h"
#include <string>
#include <map>

namespace wmdf
{
	class SZSEStepIndexDataTemplate : public StepTemplate
	{
	public:
		SZSEStepIndexDataTemplate();
		~SZSEStepIndexDataTemplate();
		virtual WindStepMessage* DecodeRaw(uint8_t* message_start,uint8_t* message_end);
	private:
		std::map<std::string,int32_t> time_map_;
		bool has_before925_;
	};

}


#endif



