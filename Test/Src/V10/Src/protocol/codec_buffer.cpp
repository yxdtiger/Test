#include <iostream>
#include "codec_buffer.h"
#ifdef __linux__
#include <cstdlib>
#include <cstring>
#endif

using namespace wmdf;
CodecBuffer::CodecBuffer()
{
	data=NULL;
	head=NULL;
	tail=NULL;
	end=NULL;
}
CodecBuffer::~CodecBuffer()
{
	if(data!=NULL)
		free(data);
	data=NULL;
	head=NULL;
	tail=NULL;
	end=NULL;
}
void CodecBuffer::MallocBuffer(int length)
{
	if(data==NULL)
	{
		data=(uint8_t*)malloc(length);
	}
  else if(Get_Buffer_Size()<length)
  {
    free(data);
    data=(uint8_t*)malloc(length);
  }
	ResetBuffer(data,length);
}

void CodecBuffer::RiseBuffer()
{
  int size = Get_Buffer_Size();
  int used_size = Get_Buffer_Used();

  uint8_t* mBuffer = (uint8_t*)malloc(2*size);
  memcpy(mBuffer,data,used_size);
  free(data);

  data = mBuffer;
  head=data;
  tail=data+used_size;
  end=data+size*2;
}

void CodecBuffer::ResetBuffer(uint8_t* bytes,int length)
{
	head=bytes;
	tail=bytes;
	end=bytes+length;
}

void CodecBuffer::GetBuffer(uint8_t* buffer)
{
  int bufferSize = Get_Buffer_Used();
	memcpy(buffer,data,bufferSize);
}

void CodecBuffer::PrintBuffer()
{
	std::cout<<"----Buffer中Byte数目为："<<std::dec<<tail-head<<"个。----"<<std::endl;
	/*for(uint8_t* i=head;i<tail;i++)
		std::cout<<std::hex<<(int)*i<<" ";
	std::cout<<std::endl;*/
}
void CodecBuffer::PrintBuffer2()
{
  std::cout<<"----Buffer中Byte数目为："<<std::dec<<end-head<<"个。----"<<std::endl;
  /*for(uint8_t* i=head;i<end;i++)
    std::cout<<std::hex<<(int)*i<<" ";
  std::cout<<std::endl;*/
}

int CodecBuffer::IsBufferEmpty ()
{
	return head >= tail;
}

int CodecBuffer::Get_Buffer_Left ()
{
	return end - tail;
}

int CodecBuffer::Get_Buffer_Size ()
{
	return end - head;
}

int CodecBuffer::Get_Buffer_Used ()
{
	return tail - head;
}
