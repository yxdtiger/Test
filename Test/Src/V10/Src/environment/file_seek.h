#ifndef  FILESEEK_H
#define FILESEEK_H

#include "../environment/wmdf_file.h"
#include <string>

namespace wmdf
{

class FileSeek
{
public:
	virtual ~FileSeek(void);
public:
	virtual bool SetPos(int64_t newPos) = 0;
	virtual bool GetPos(int64_t &pos) = 0;
	virtual bool SeekToPos(int64_t offset, SeekMode origin) = 0;
	virtual bool SeekToEnd() = 0;
	virtual bool SeekToBegin() = 0;
	virtual bool SeekFromCurrent(int64_t offset) = 0;
	virtual bool SeekFromBegin(int64_t offset) = 0;
	virtual bool SeekFromEnd(int64_t offset) = 0;
	virtual bool IsEndofFile() = 0;
};

class BinFileSeek: virtual public FileSeek
{
public:
  BinFileSeek(std::string& Path);
	BinFileSeek(WMDFFile* pFile);
	virtual ~BinFileSeek(void);
protected:
	WMDFFile* m_File;
private:
	bool m_NeedFreeFile;
public:
	virtual bool SetPos(int64_t newPos);
	virtual bool GetPos(int64_t &pos);
	virtual bool SeekToPos(int64_t offset, SeekMode origin);
	virtual bool SeekToEnd();
	virtual bool SeekToBegin();
	virtual bool SeekFromCurrent(int64_t offset);
	virtual bool SeekFromBegin(int64_t offset);
	virtual bool SeekFromEnd(int64_t offset);
	virtual bool IsEndofFile();
};

}

#endif
