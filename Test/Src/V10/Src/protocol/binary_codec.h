#ifndef BINARY_CODEC_H
#define BINARY_CODEC_H
#include "../include/wmdf_api.h"
#include "codec_buffer.h"


namespace wmdf{

class BinaryCodec
{
public:
	BinaryCodec(void);
	virtual ~BinaryCodec(void);

	CodecBuffer buffer;
	void InitEncoder();
	void EncodeInt8(int8_t value);
	void EncodeInt32(int32_t value);
	void EncodeInt64(int64_t value);
	void EncodeBytes(uint8_t*  value,int length);
	void EncodeDouble(double value);

	void    InitDecoder(uint8_t* bytes,int length);
  int8_t  DecodeInt8();
	int32_t DecodeInt32();
	int64_t DecodeInt64();

	void    DecodeBytes(uint8_t* dst,int length);
	double  DecodeDouble();
  int     Check_Msg_Avail ();
};
}

#endif
