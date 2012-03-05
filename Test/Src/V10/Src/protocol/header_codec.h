#ifndef HEADER_CODEC_H
#define HEADER_CODEC_H
#include <stdlib.h>
#include "binary_codec.h"



namespace wmdf
{
  class HeaderCodec
  {
  public:
		HeaderCodec():binaryCodec(new BinaryCodec())
    {
			header_ = NULL;
    }
    virtual ~HeaderCodec(void)
    {
      if(NULL!=binaryCodec)
      {
        delete binaryCodec;
        binaryCodec=NULL;
      }
      if (NULL != header_)
      {
        delete header_;
        header_ = NULL;
      }
    }
    void DequeueBuffer(uint8_t* buffer) 
    {
      binaryCodec->buffer.GetBuffer(buffer);
    }

    void set_header(void* header)
    {
       header_ = header;
    }

    virtual int32_t SizeOfRecords()=0;
    virtual int EncodeData() = 0;     
    virtual void* DecodeData(uint8_t* bytes,int& length) = 0;

  protected:
    BinaryCodec* binaryCodec;
    void* header_;
  };
}

#endif
