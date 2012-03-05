#if !defined DATAREADER_H
#define DATAREADER_H

#include "file_seek.h"



namespace wmdf
{

typedef unsigned char byte;

class  DataReader: virtual public FileSeek
{
public:
	virtual int32_t ReadInteger() = 0;
	virtual uint32_t ReadUInt() = 0;
	virtual int16_t ReadShort() = 0;
	virtual uint16_t ReadUShort() = 0;
	virtual int8_t ReadChar() = 0;
	virtual uint8_t ReadUChar() = 0;
	virtual float ReadFloat() = 0;
	virtual double ReadDouble() = 0;
	virtual char* ReadString() = 0;//先读长度，再读buf
	virtual byte* ReadBuffer(byte* buf,const int& szie) = 0;
};

#pragma warning(disable:4250)

class  CBinDataReader: virtual public DataReader,virtual public BinFileSeek
{
public:
  CBinDataReader(std::string& Path);
	CBinDataReader(WMDFFile* pFile);
public:
	virtual int32_t ReadInteger();
	virtual uint32_t ReadUInt();
	virtual int16_t ReadShort();
	virtual uint16_t ReadUShort();
	virtual int8_t ReadChar();
	virtual uint8_t ReadUChar();
	virtual float ReadFloat();
	virtual double ReadDouble();
	virtual char* ReadString();//先读长度，再读buf
	virtual byte* ReadBuffer(byte* buf,const int& szie);
};

#pragma warning(default:4250)

}


#endif
