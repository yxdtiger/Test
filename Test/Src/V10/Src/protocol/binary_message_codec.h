#ifndef BINARY_MESSAGE_CODEC_H
#define BINARY_MESSAGE_CODEC_H

#include "../include/wmdf_api.h"
#include "binary_codec.h"
#include "message_codec.h"
#include <stdio.h>

namespace wmdf{

	class BinaryMessageCodec:public MessageCodec
	{
	public:
		BinaryMessageCodec()
		{
			binaryCodec = new BinaryCodec();
      single_length = 0;
		}
		virtual ~BinaryMessageCodec(void)
		{
			delete binaryCodec;
      binaryCodec=NULL;
		}

		virtual int32_t SizeOfRecords(void* items,int count)=0;
		virtual int EncodeData(void* tr,int count) = 0;
		virtual void* DecodeData(uint8_t* bytes,int length,int& count) = 0;
		virtual void DequeueBuffer(uint8_t* buffer)
		{
			return binaryCodec->buffer.GetBuffer(buffer);
		}
	protected:

		BinaryCodec* binaryCodec;
    int single_length;
	};

}
#endif
