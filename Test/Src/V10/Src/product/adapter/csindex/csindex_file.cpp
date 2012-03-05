#include "csindex_file.h"
#include "../../../environment/file.h"
#include <sys/stat.h>
#include "../../../environment/directory.h"
#include "../../../include/date_time.h"


#ifdef __linux__
#include <unistd.h>
#endif



wmdf::CSIndexFile::CSIndexFile( const std::string &filename )
{
	file_ = NULL;
	file_size_ = -1;
	filepath_ = filename;
	last_modified_time_ = 0;
}

wmdf::CSIndexFile::CSIndexFile()
{

}

wmdf::CSIndexFile::~CSIndexFile()
{
	Close();
}

bool wmdf::CSIndexFile::Open()
{
	file_ = fopen(filepath_.c_str(), "r");
	if(file_ != NULL)
	{
		file_size_ = File::FileSize(filepath_);
		return true;
	}
	else
		return NULL;
}


void wmdf::CSIndexFile::Close()
{
	if(file_ != NULL)
	{
		fclose(file_);
		file_ = NULL;
	}
}


bool wmdf::CSIndexFile::IsExist()
{
	if(Directory::IsDirectory(filepath_))
	{
		int32_t today = DateTime::date2ymd(DateTime::today());
		char date[10];
		sprintf(date,"%d",today);
		std::string filename = "\\indexpress";
		filename += date;
		filename += ".txt";
		filepath_ += filename;
		return File::Exists(filepath_);
	}
	else
		return File::Exists(filepath_);
}

bool wmdf::CSIndexFile::IsEOF()
{
	return feof(file_);
}

bool wmdf::CSIndexFile::IsModified()
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

int64_t wmdf::CSIndexFile::GetPos()
{
	fpos_t p;
	int rtn = fgetpos(file_,&p);
#ifdef __linux__
	return (int64_t)p.__pos;
#else
	return (int64_t)p;
#endif
}

bool wmdf::CSIndexFile::SetPos( int64_t newPos)
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


void wmdf::CSIndexFile::ReadLine()
{
	char line[line_length_]={0};
	fgets(line,line_length_,file_);
	curr_line_ = line;
}

void wmdf::CSIndexFile::GetLine( std::string &line )
{
	line = curr_line_;
}

int64_t wmdf::CSIndexFile::FileSize()
{
	return file_size_;
}














