#include "step_template.h"



wmdf::StepTemplate::StepTemplate()
{
  step_decoder_ = new StepDecoder();
}


wmdf::StepTemplate::~StepTemplate()
{
	delete step_decoder_;
}
