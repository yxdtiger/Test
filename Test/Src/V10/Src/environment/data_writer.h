#if !defined DATAWRITER_H
#define DATAWRITER_H


#include "../include/wmdf_api.h"
#include "file_seek.h"

namespace wmdf
{

class  DataWriter: virtual public FileSeek
{
public:
	virtual int32_t WriteInteger(int32_t Value) = 0;
	virtual int32_t WriteUInt(uint32_t Value) = 0;
	virtual int32_t WriteShort(int16_t Value) = 0;
	virtual int32_t WriteUShort(uint16_t Value) = 0;
	virtual int32_t WriteChar(int8_t Value) = 0;
	virtual int32_t WriteUChar(uint8_t Value) = 0;
	virtual int32_t WriteFloat(float Value) = 0;
	virtual int32_t WriteDouble(double Value) = 0;
	virtual int32_t WriteString(char* Value) = 0;//'\0'Ω·Œ≤
  virtual int32_t WriteString(const std::string& Value) = 0;//'\0'Ω·Œ≤
	virtual int32_t WriteBuffer(unsigned char* buf,const int& szie) = 0;
};

#pragma warning(disable:4250)

class  CBinDataWriter: virtual public DataWriter,virtual public BinFileSeek
{
public:
  CBinDataWriter(std::string& Path);
	CBinDataWriter(WMDFFile* pFile);
public:
	virtual int32_t WriteInteger(int32_t Value);
	virtual int32_t WriteUInt(uint32_t Value);
	virtual int32_t WriteShort(int16_t Value);
	virtual int32_t WriteUShort(uint16_t Value);
	virtual int32_t WriteChar(int8_t Value);
	virtual int32_t WriteUChar(uint8_t Value);
	virtual int32_t WriteFloat(float Value);
	virtual int32_t WriteDouble(double Value);
	virtual int32_t WriteString(char* Value);//'\0'Ω·Œ≤
	virtual int32_t WriteString(const std::string& Value);
	virtual int32_t WriteBuffer(unsigned char* buf,const int& szie);
};
#pragma warning(default:4250)

}

#endif
