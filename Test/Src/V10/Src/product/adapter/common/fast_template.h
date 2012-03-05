#ifndef FAST_TEMPLATE_H
#define FAST_TEMPLATE_H

#include "../../../include/wmdf_api.h"
#include "../common/fast_decoder.h"
#include  "../../../environment/utils.h"

namespace wmdf
{
  struct WindFastMessage;
	class FastTemplate
	{
	public:
		FastTemplate(int32_t item_count);
		virtual ~FastTemplate();
		virtual WindFastMessage* DecodeRaw(uint8_t* raw_data,uint32_t len)=0;
	protected:
		FastDecoder* fast_decoder_;
	};
}




#endif
