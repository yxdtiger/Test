#include "wmdf_file.h"
#ifdef __linux__
#include <unistd.h>
#endif

#include "file.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifdef _WIN32
#include <io.h>
#elif __linux__
#include <sys/io.h>
#endif

#ifndef F_OK
#define F_OK 0
#endif
#define EXIST(p)  (!access((p), F_OK))

using namespace wmdf;

WMDFFile::WMDFFile()
{
	m_pFile = NULL;
}

WMDFFile::~WMDFFile()
{
	Close();
}
bool WMDFFile::Open(const char * path,const char *mode)
{
	m_pFile = fopen(path, mode);
	file_size_ = File::FileSize(path);
	if(m_pFile == NULL)
		return false;
	else
		return true;
}
void WMDFFile::Close()
{
	if(m_pFile != NULL)
	{
		fclose(m_pFile);
		m_pFile = NULL;
	}
}

bool WMDFFile::IsExist(const char * path)
{
#ifdef WIN32
	return _access(path,F_OK)==0;
#elif __linux__
	return access(path,F_OK)==0;
#endif
}

bool WMDFFile::SetPos(int64_t newPos)
{
	fpos_t pos;
#ifdef WIN32
	pos = newPos;
#elif __linux__
	pos.__pos = newPos;
#endif
	int rtn = fsetpos(m_pFile, (&pos));
	if(rtn == 0)
		return true;
	else
		return false;
}
bool WMDFFile::GetPos(int64_t &pos)
{
	fpos_t p;
	int rtn = fgetpos(m_pFile,&p);
#ifdef WIN32
	pos = (int64_t)p;
#elif __linux__
	pos = (int64_t)p.__pos;
#endif

	if(rtn == 0)
		return true;
	else
		return false;
}

bool WMDFFile::SeekToPos(int64_t offset, SeekMode origin)
{	
	int rtn = fseek(m_pFile, offset, (int32_t)origin);
	if(rtn == 0)
		return true;
	else
		return false;
}

bool WMDFFile::SeekToEnd()
{
	int rtn = fseek(m_pFile, 0, SEEK_END);
	if(rtn == 0)
		return true;
	else
		return false;
}

bool WMDFFile::SeekToBegin()
{
	int rtn = fseek(m_pFile, 0, SEEK_SET);
	if(rtn == 0)
		return true;
	else
		return false;
}

bool WMDFFile::SeekFromCurrent(int64_t offset)
{
	int rtn = fseek(m_pFile, offset, SEEK_CUR);
	if(rtn == 0)
		return true;
	else
		return false;
}

bool WMDFFile::SeekFromBegin(int64_t offset)
{
	int rtn = fseek(m_pFile, offset, SEEK_SET);
	if(rtn == 0)
		return true;
	else
		return false;
}

bool WMDFFile::SeekFromEnd(int64_t offset)
{
	int rtn = fseek(m_pFile, offset, SEEK_END);
	if(rtn == 0)
		return true;
	else
		return false;
}

int WMDFFile::WriteBuffer(const void *buffer, int32_t size)
{
/*	int len = fwrite(buffer,1,size,m_pFile);
	fflush(m_pFile);
	return len;*/
	return fwrite(buffer,1,size,m_pFile);
}

int WMDFFile::Write(unsigned char *string, int32_t size)
{
	return WriteBuffer(string,size);
}

int WMDFFile::Write(char *string, int32_t size)
{
	return WriteBuffer(string,size);
}

int WMDFFile::Write(int8_t value)
{
	return WriteBuffer(&value,sizeof(int8_t));
}

int WMDFFile::Write(uint8_t value)
{
	return WriteBuffer(&value,sizeof(uint8_t));
}

int WMDFFile::Write(int32_t value)
{
	return WriteBuffer(&value,sizeof(int32_t));
}

int WMDFFile::Write(uint32_t value)
{
	return WriteBuffer(&value,sizeof(uint32_t));
}

int WMDFFile::Write(int16_t value)
{
	return WriteBuffer(&value,sizeof(int16_t));
}

int WMDFFile::Write(uint16_t value)
{
	return WriteBuffer(&value,sizeof(uint16_t));
}

int WMDFFile::Write(float value)
{
	return WriteBuffer(&value,sizeof(float));
}

int WMDFFile::Write(double value)
{
	return WriteBuffer(&value,sizeof(double));
}

int WMDFFile::ReadBuffer(void *buffer, int32_t size)
{
	return fread(buffer,1,size,m_pFile);
}

int WMDFFile::Read(int32_t &value)
{
	return ReadBuffer(&value,sizeof(int32_t));
}

int WMDFFile::Read(uint32_t &value)
{
	return ReadBuffer(&value,sizeof(uint32_t));
}

int WMDFFile::Read(int16_t &value)
{
	return ReadBuffer(&value,sizeof(int16_t));
}

int WMDFFile::Read(uint16_t &value)
{
	return ReadBuffer(&value,sizeof(uint16_t));
}

int WMDFFile::Read(int8_t &value)
{
	return ReadBuffer(&value,sizeof(int8_t));
}

int WMDFFile::Read(uint8_t &value)
{
	return ReadBuffer(&value,sizeof(uint8_t));
}

int WMDFFile::Read(float &value)
{
	return ReadBuffer(&value,sizeof(float));
}

int WMDFFile::Read(double &value)
{
	return ReadBuffer(&value,sizeof(double));
}

int WMDFFile::Read(unsigned char *string,int32_t size)
{
	return ReadBuffer(string,size);
}

int WMDFFile::Read(char *string,int32_t size)
{
	return ReadBuffer(string,size);
}

int wmdf::WMDFFile::Read( int64_t &value )
{
  
  return ReadBuffer(&value,sizeof(int64_t));
}

int64_t wmdf::WMDFFile::GetSize()
{
	return file_size_;
//   int64_t currentPos = ftell(m_pFile);    //取得当前文件指针位置,可能已经移动了文件指针
//   fseek(m_pFile, 0, SEEK_END);//移动到文件的结尾
//   int64_t lastPos = ftell(m_pFile);//获得文件大小
// 	
//   fseek(m_pFile,currentPos,SEEK_SET);//恢复到原来的文件指针位置
//   return lastPos;
}

void wmdf::WMDFFile::Flush()
{
  if(m_pFile != NULL)
  {
    fflush(m_pFile);
  }
}
