#include "data_writer.h"
#include <cstring>

using namespace wmdf;
using namespace std;

typedef unsigned char byte;

CBinDataWriter::CBinDataWriter(std::string& Path):BinFileSeek(Path)
{
	m_File->Open((char*)Path.c_str(),"wa+");
}

CBinDataWriter::CBinDataWriter(WMDFFile* pFile):BinFileSeek(pFile)
{

}

int32_t CBinDataWriter::WriteInteger(int32_t Value)
{
	m_File->Write(Value);
	return sizeof(Value);
}

int32_t CBinDataWriter::WriteUInt(uint32_t Value)
{
	m_File->Write(Value);
	return sizeof(Value);
}

int32_t CBinDataWriter::WriteShort(int16_t Value)
{
	m_File->Write(Value);
	return sizeof(Value);
}

int32_t CBinDataWriter::WriteUShort(uint16_t Value)
{
	m_File->Write(Value);
	return sizeof(Value);
}

int32_t CBinDataWriter::WriteChar(int8_t Value)
{
	m_File->Write(Value);
	return sizeof(Value);
}

int32_t CBinDataWriter::WriteUChar(uint8_t Value)
{
	m_File->Write(Value);
	return sizeof(Value);
}

int32_t CBinDataWriter::WriteFloat(float Value)
{
	m_File->Write(Value);
	return sizeof(Value);
}

int32_t CBinDataWriter::WriteDouble(double Value)
{
	m_File->Write(Value);
	return sizeof(Value);
}

int32_t CBinDataWriter::WriteString(char* Value)//先读长度，再读buf
{
	int len = strlen(Value) + 1;
	long Result = WriteInteger(len);
	Result += WriteBuffer((unsigned char*)Value,len);
	return Result;
}

int32_t CBinDataWriter::WriteString(const std::string& Value)
{
	long Result = WriteString(Value);
	return Result;
}

int32_t CBinDataWriter::WriteBuffer(unsigned char* buf,const int& szie)
{
	m_File->Write((char*)buf,szie);
	return szie;
}
