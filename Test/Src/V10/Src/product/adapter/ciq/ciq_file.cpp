#include "ciq_file.h"
#include "../../../environment/file.h"
#include <sys/stat.h>

#ifdef __linux__
#include <unistd.h>
#endif



wmdf::CIQFile::CIQFile(const std::string &filename)
{
	file_ = NULL;
	file_size_ = -1;
	filepath_ = filename;
	last_modified_time_ = 0;
}

wmdf::CIQFile::CIQFile()
{

}

wmdf::CIQFile::~CIQFile()
{
	Close();
}

bool wmdf::CIQFile::Open()
{
	file_ = fopen(filepath_.c_str(), "r");
	if(file_ != NULL)
	{
		file_size_ = File::FileSize(filepath_);
		return true;
	}
	else
		return false;
}

bool wmdf::CIQFile::IsExist(  )
{
	return File::Exists(filepath_.c_str());
}

void wmdf::CIQFile::Close()
{
	if(file_ != NULL)
	{
		fclose(file_);
		file_ = NULL;
	}
}

bool wmdf::CIQFile::IsEOF()
{
	return feof(file_);
}


int64_t wmdf::CIQFile::GetPos()
{
	fpos_t p;
	int rtn = fgetpos(file_,&p);
#ifdef __linux__
	return (int64_t)p.__pos;
#else
    return (int64_t)p;
#endif
}

bool wmdf::CIQFile::SetPos(int64_t newPos)
{
	fpos_t pos;
#ifdef WIN32
	pos = newPos;
#elif __linux__
	pos.__pos = newPos;
#endif
	int rtn = fsetpos(file_, (&pos));
	if(rtn == 0)
		return true;
	else
		return false;
}

bool wmdf::CIQFile::IsModified()
{
	struct stat stat_buf;
	stat(filepath_.c_str(),&stat_buf);
	time_t modify_time = stat_buf.st_mtime;
	if(modify_time!=last_modified_time_)
	{
		last_modified_time_ = modify_time;
		SetPos(0);
		return true;
	}
	else
		return false;
}

wmdf::CIQIndex* wmdf::CIQFile::ReadCIQIndex( int32_t &ciqindex_count )
{
	ciqindex_count = file_size_/SIZE_OF_RECORD_;
	CIQIndex *ciqindex = new CIQIndex[ciqindex_count];
	fread(ciqindex,ciqindex_count,sizeof(CIQIndex),file_);
	return ciqindex;
}
