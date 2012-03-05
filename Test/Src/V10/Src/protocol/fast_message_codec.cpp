#include "fast_message_codec.h"

using namespace wmdf;

FastMessageCodec::FastMessageCodec()
{
	fastCodec = new FastCodec();
}


FastMessageCodec::~FastMessageCodec(void)
{
  delete fastCodec;
  fastCodec=NULL;

  if(mainPMap!=NULL)
  {
	  free(mainPMap);
	  mainPMap=NULL;
  }

  if(main_DefaultValue!=NULL)
  {
	  free(main_DefaultValue);
	  main_DefaultValue =NULL;
  }
}

void FastMessageCodec::DequeueBuffer(uint8_t* buffer)
{
  fastCodec->buffer.GetBuffer(buffer);
}
