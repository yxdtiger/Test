#include "file_seek.h"
#include <assert.h>

using namespace wmdf;
using namespace std;

FileSeek::~FileSeek(void)
{
}

BinFileSeek::BinFileSeek(string& Path)
{
	m_NeedFreeFile = true;
	m_File = new WMDFFile();
}

BinFileSeek::BinFileSeek(WMDFFile* pFile)
{
	m_NeedFreeFile = false;
	m_File = pFile;
}

BinFileSeek::~BinFileSeek(void)
{
	assert(m_File != NULL);
	m_File->Close();
	if (m_NeedFreeFile)
		delete m_File;
	m_File = NULL;
}

bool BinFileSeek::SetPos(int64_t newPos)
{
	return m_File->SetPos(newPos);
}

bool BinFileSeek::GetPos(int64_t &pos)
{
	return m_File->GetPos(pos);
}

bool BinFileSeek::SeekToPos(int64_t offset, SeekMode origin)
{
	return m_File->SeekToPos(offset,origin);
}

bool BinFileSeek::SeekToEnd()
{
	return m_File->SeekToEnd();
}

bool BinFileSeek::SeekToBegin()
{
	return m_File->SeekToBegin();
}

bool BinFileSeek::SeekFromCurrent(int64_t offset)
{
	return m_File->SeekFromCurrent(offset);
}

bool BinFileSeek::SeekFromBegin(int64_t offset)
{
	return m_File->SeekFromBegin(offset);
}

bool BinFileSeek::SeekFromEnd(int64_t offset)
{
	return m_File->SeekFromEnd(offset);
}

bool BinFileSeek::IsEndofFile()
{
	int64_t oldpos = 0,newpos = 0;
	GetPos(oldpos);
	this->SeekToEnd();
	GetPos(newpos);
	SetPos(oldpos);
	return newpos <= oldpos;
	//return m_File->IsEndofFile();
}
