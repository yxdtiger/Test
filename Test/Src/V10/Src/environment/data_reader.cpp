#include "data_reader.h"

using namespace wmdf;

CBinDataReader::CBinDataReader(std::string& Path):BinFileSeek(Path)
{
	m_File->Open((char*)Path.c_str(),MODE_OPENREAD_BIN);
}
CBinDataReader::CBinDataReader(WMDFFile* pFile):BinFileSeek(pFile)
{

}

int CBinDataReader::ReadInteger()
{
	int32_t Result;
	m_File->Read(Result);
	return Result;
}
unsigned int CBinDataReader::ReadUInt()
{
	uint32_t Result;
	m_File->Read(Result);
	return Result;
}
short CBinDataReader::ReadShort()
{
	int16_t Result;
	m_File->Read(Result);
	return Result;
}
unsigned short CBinDataReader::ReadUShort()
{
	uint16_t Result;
	m_File->Read(Result);
	return Result;
}
int8_t CBinDataReader::ReadChar()
{
	int8_t Result;
	m_File->Read(Result);
	return Result;
}
unsigned char CBinDataReader::ReadUChar()
{
	uint8_t Result;
	m_File->Read(Result);
	return Result;
}
float CBinDataReader::ReadFloat()
{
	float Result;
	m_File->Read(Result);
	return Result;
}
double CBinDataReader::ReadDouble()
{
	double Result;
	m_File->Read(Result);
	return Result;
}
char* CBinDataReader::ReadString()//先读长度，再读buf
{
	int32_t len = this->ReadInteger();
	char* Result = new char[len];
	ReadBuffer((byte*)Result,len);
	return Result;
}
byte* CBinDataReader::ReadBuffer(byte* buf,const int& szie)
{
	m_File->Read((char*)buf,szie);
	return buf;
}
