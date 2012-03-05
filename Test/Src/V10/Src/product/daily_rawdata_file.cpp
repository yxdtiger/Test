#include "daily_rawdata_file.h"
#include "../environment/strutils.h"
#include "../environment/unrar_utils.h"
#include "../environment/application_directory.h"
#include "../environment/file.h"
#include "../environment/directory.h"

wmdf::DailyRawdataFile::DailyRawdataFile(int32_t date)
{
  date_ = date;
  current_index_ = 0;
}

wmdf::DailyRawdataFile::~DailyRawdataFile()
{

}


void wmdf::DailyRawdataFile::InsertAMFile(std::string file)
{
	filename_[0] = file;
}

void wmdf::DailyRawdataFile::InsertPMFile( std::string file)
{	
	filename_[1] = file;
}

void wmdf::DailyRawdataFile::InsertWholeDayFile( std::string file)
{
	filename_[2] = file;
}

bool wmdf::DailyRawdataFile::IsWholeDay()
{
	if((filename_[0]!=""&&filename_[1]!="")||(filename_[2]!=""))
		return true;
	else
		return false;
}

bool wmdf::DailyRawdataFile::IsCompressed( const std::string& filename )
{
  size_t pos = filename.find_last_of(".");
  std::string filesuffix = filename.substr(pos+1,filename.size());
  filesuffix = StrUtils::ToLower(filesuffix);
  return (filesuffix == "rar");
}

std::string wmdf::DailyRawdataFile::GetNextFile()
{
  for (int32_t index = current_index_;index < 3;index++)
  {
    current_index_++;
    if ("" != filename_[index])
    {
      return filename_[index];
    }
  }
  return "";
}

std::string wmdf::DailyRawdataFile::GetUncompressedFile( const std::string& filename )
{
  size_t pos = filename.find_last_of("\\");
  std::string file_path = filename.substr(pos+1,filename.length()-pos);
  
  pos = file_path.find_last_of(".");
  file_path.replace(pos+1,3,"txt");
  
  std::string retfile = ApplicationDirectory::ApplicationPath()+file_path;

  if (!File::Exists(retfile))
  {
    file_path.replace(pos+1,3,"dat");
    retfile = ApplicationDirectory::ApplicationPath()+file_path;
    //特殊处理一下，因为深交所的压缩文件有一个szse的目录
    if (!File::Exists(retfile))
    {
      retfile = ApplicationDirectory::ApplicationPath() + "szse\\" + file_path;
    }
  }
  return retfile;
}

void wmdf::DailyRawdataFile::Uncompress( const std::string& filename )
{
  UnrarUtils::ExtractArchive((char*)filename.c_str(),(char*)ApplicationDirectory::ApplicationPath().c_str());
}

bool wmdf::DailyRawdataFile::IsLastFile()
{
  for (int32_t index = current_index_;index < 3;index++)
  {
    return ("" == filename_[index]);
  }
  return true;
}