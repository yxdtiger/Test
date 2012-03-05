#ifndef STEP_TEMPLATE_H
#define STEP_TEMPLATE_H

#include <vector>
#include "../../../include/wmdf_api.h"
#include "step_codec.h"
#include "rawdata_object.h"
#include <string>


#define FIND_TAG(TAG) ((ret=step_decoder_->CheckTag(TAG))&&step_decoder_->Next(ret))

namespace wmdf
{
	class StepTemplate
	{
	public:
		StepTemplate();
		virtual ~StepTemplate();
		virtual WindStepMessage* DecodeRaw(uint8_t* message_start,uint8_t* message_end)=0;
	protected:
		StepDecoder* step_decoder_;
	};
}



#endif
