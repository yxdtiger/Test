#include "binary_codec.h"
#include <cstring>

using namespace wmdf;


BinaryCodec::BinaryCodec(void)
{
  
}


BinaryCodec::~BinaryCodec(void)
{
}

void BinaryCodec::InitDecoder(uint8_t* bytes,int length)
{
	buffer.ResetBuffer(bytes,length);
}

void BinaryCodec::InitEncoder()
{  
	buffer.MallocBuffer();
}

void BinaryCodec::EncodeInt8(int8_t value)
{
	*buffer.tail ++ = value;
}

void BinaryCodec::EncodeInt32(int32_t value)
{
	memcpy(buffer.tail,&value,sizeof(value));
	buffer.tail += sizeof(value);
}

void BinaryCodec::EncodeInt64(int64_t value)
{
	memcpy(buffer.tail,&value,sizeof(value));
	buffer.tail += sizeof(value);
}

void BinaryCodec::EncodeDouble(double value)
{
	memcpy(buffer.tail,&value,sizeof(value));
	buffer.tail += sizeof(value);
}

void BinaryCodec::EncodeBytes(uint8_t* src,int length)
{
	memcpy(buffer.tail,src,length);
	buffer.tail += length;
}

int8_t BinaryCodec::DecodeInt8()
{
  return*buffer.tail++;
}

int32_t BinaryCodec::DecodeInt32()
{
	int32_t value;
	memcpy(&value,buffer.tail,sizeof(value));
	buffer.tail+=sizeof(value);
	return value;
}

int64_t BinaryCodec::DecodeInt64()
{
	int64_t value;
	memcpy(&value,buffer.tail,sizeof(value));
	buffer.tail+=sizeof(value);
	return value;
}

double BinaryCodec::DecodeDouble()
{
	double value;
	memcpy(&value,buffer.tail,sizeof(value));
	buffer.tail+=sizeof(value);
	return value;
}

void BinaryCodec::DecodeBytes(uint8_t* dst,int length)
{
	memcpy(dst,buffer.tail,length);
	buffer.tail+=length;
}

int BinaryCodec::Check_Msg_Avail ()
{
  if (buffer.Get_Buffer_Left () < 102400)
    buffer.RiseBuffer();
  return 0;
}
