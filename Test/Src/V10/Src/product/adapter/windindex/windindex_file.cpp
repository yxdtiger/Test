#include "windindex_file.h"
#include "../../../environment/file.h"
#include "../../../include/date_time.h"
#include <cassert>
#include <sys/stat.h>


#ifdef __linux__
#include <unistd.h>
#endif



wmdf::WindIndexFile::WindIndexFile(const std::string &filename)
{
	file_ = NULL;
	file_size_ = -1;
	filepath_ = filename;
	last_modified_time_ = 0;
}

wmdf::WindIndexFile::WindIndexFile()
{

}

wmdf::WindIndexFile::~WindIndexFile()
{
	Close();
}

bool wmdf::WindIndexFile::Open()
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

void wmdf::WindIndexFile::Close()
{
	if(file_ != NULL)
	{
		fclose(file_);
		file_ = NULL;
	}
}


bool wmdf::WindIndexFile::IsExist()
{
	return File::Exists(filepath_.c_str());
}

bool wmdf::WindIndexFile::IsEOF()
{
	return feof(file_);
}

int64_t wmdf::WindIndexFile::RemainBytes()
{
	return file_size_-GetPos();
}

int64_t wmdf::WindIndexFile::GetPos()
{
	fpos_t p;
	int rtn = fgetpos(file_,&p);
#ifdef __linux__
	return (int64_t)p.__pos;
#else
	return (int64_t)p;
#endif
}

bool wmdf::WindIndexFile::SetPos( int64_t newPos)
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

bool wmdf::WindIndexFile::IsModified()
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

int64_t wmdf::WindIndexFile::FileSize()
{
	return file_size_;
}



wmdf::WindIndex* wmdf::WindIndexFile::ReadWindIndex(int32_t &windindex_count)
{
	windindex_count = (file_size_-4)/RECORD_SIZE_;
	WindIndex *windindex = new WindIndex[windindex_count];
	int32_t trade_date;
	fread(&trade_date,1,4,file_);
	fread(windindex,windindex_count,sizeof(WindIndex),file_);
	return windindex;
}








