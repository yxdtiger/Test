#include "fast_template.h"
#include "fast_decoder.h"

wmdf::FastTemplate::FastTemplate( int32_t item_count )
{
  fast_decoder_ = new FastDecoder(item_count);
}

wmdf::FastTemplate::~FastTemplate()
{
  if (NULL != fast_decoder_)
  {
	  delete fast_decoder_;
    fast_decoder_ = NULL;
  }
}

