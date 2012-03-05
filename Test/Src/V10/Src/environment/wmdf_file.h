#if !defined WMDFFILE_H
#define WMDFFILE_H

#include <stdio.h>
#include "../include/wmdf_api.h"

namespace wmdf{

#define	MODE_OPENREAD_BIN "rb"
#define	MODE_CREATEWRITE_BIN "wb"
#define MODE_OPENWRITE_BIN  "ab"
#define	MODE_OPENREAD_TEXT "r"
#define MODE_OPENWRITE_TEXT "a"
#define	MODE_OPENREADWRITE_TEXT "a+"
#define MODE_CREATEWRITE_TEXT "w"
#define MODE_CREATEREADWRITE_TEXT "w+"

enum SeekMode
{
	SEEK_FROMBEGIN = SEEK_SET,
	SEEK_FROMEND = SEEK_END,
	SEEK_FROMCURRENT = SEEK_CUR,
};

#include <stdio.h>

class WMDFFile
{
public:
	WMDFFile();
	virtual ~WMDFFile();

public:
	bool Open(const char* path, const char* mode);
	void Close();
  void Flush();
	bool IsExist(const char * path);
  int64_t  GetSize();
	bool SetPos(int64_t newPos);
	bool GetPos(int64_t &pos);
	bool SeekToPos(int64_t offset, SeekMode origin);
	bool SeekToEnd();
	bool SeekToBegin();
	bool SeekFromCurrent(int64_t offset);
	bool SeekFromBegin(int64_t offset);
	bool SeekFromEnd(int64_t offset);

	int Write(unsigned char *string, int32_t size);
	int Write(char *string, int32_t size);
	int Write(int8_t value);
	int Write(uint8_t value);
	int Write(int32_t value);
	int Write(uint32_t value);
	int Write(int16_t value);
	int Write(uint16_t value);
	int Write(float value);
	int Write(double value);
  int Write(int64_t value);

	//返回实际读的字符串长度
	int Read(int32_t &value);
	int Read(uint32_t &value);
	int Read(int16_t &value);
	int Read(uint16_t &value);
	int Read(int8_t &value);
	int Read(uint8_t &value);
	int Read(float &value);
	int Read(double &value);
  int Read(int64_t &value);

	int Read(unsigned char *string,int32_t size);
	int Read(char *string,int32_t size);
private:
	int WriteBuffer(const void *buffer, int32_t size);
	int ReadBuffer(void *buffer, int32_t size);
	FILE *m_pFile;
	int64_t file_size_;
};

}

#endif
