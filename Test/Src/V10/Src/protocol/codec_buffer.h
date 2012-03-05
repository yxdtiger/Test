#ifndef CODEC_BUFFER_H
#define CODEC_BUFFER_H

#include "../include/wmdf_api.h"
namespace wmdf
{
	#define INITIAL_MSG_SIZE  2097152 //2M

	class CodecBuffer
	{
	public:
		CodecBuffer(void);
		virtual ~CodecBuffer(void);

		void MallocBuffer(int length=INITIAL_MSG_SIZE);
		void ResetBuffer(uint8_t* bytes,int length);
    void RiseBuffer();

		int		Get_Buffer_Used ();
		int		Get_Buffer_Left ();
		int		Get_Buffer_Size ();
		int		IsBufferEmpty ();
		void	GetBuffer(uint8_t* buffer);
    void      PrintBuffer();
    void      PrintBuffer2();

		uint8_t*  tail;
	private:
		uint8_t*  head;
		uint8_t*  end;
		uint8_t*  data;
	};
}
#endif
