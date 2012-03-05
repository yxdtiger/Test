#include "logger.h"
#include "../environment/utils.h"
#include "../include/date_time.h"
#include "../environment/wmdf_file.h"
#include "../environment/application_directory.h"
#include "imonitor_event_collector.h"
#include "../environment/directory.h"
#include <cstdarg>


wmdf::Logger* wmdf::Logger::instance_ = new wmdf::Logger();

wmdf::Logger::Logger(void)
{
  file_appender_ = NULL;
  monitor_event_collector_ = NULL;
}

wmdf::Logger::~Logger(void)
{
  if (NULL != file_appender_)
  {
    file_appender_->Stop();
    delete file_appender_;
    file_appender_ = NULL;
  }
}

void wmdf::Logger::WriteLog( LogLevel log_level, const std::string& log )
{
  switch(log_level)
  {
  case LL_FATAL:
  case LL_ERROR:
  case LL_WARN:
  case LL_INFO:
  case LL_DEBUG:
    {
      char buff[500] = {0};
      #pragma warning(disable : 4996)
      sprintf(buff,"%s %s %s \n",DateTime::now2str().c_str(),Log_Level_Str[log_level].c_str(),log.c_str());
      #pragma warning(default : 4996)
      printf(buff);
      if(NULL != file_appender_)
      {
        file_appender_->AppendLog(buff);
      }
    }
    break;
  default:
    break;
  }
}

void wmdf::Logger::WriteLog( const std::string& log )
{
  printf("%s \n",log.c_str());
  if(NULL != file_appender_)
  {
    file_appender_->AppendLog(log+"\n");
  }
}

void wmdf::Logger::WriteEventLog( LogLevel level,int32_t event_id,const std::string& at_where,const std::string& what_happened )
{
  char log[500] = {0};
#pragma warning(disable : 4996)
  sprintf(log,"%s %s[%d] %s: %s",DateTime::now2str().c_str(),Log_Level_Str[level].c_str(),event_id,at_where.c_str(),what_happened.c_str());
#pragma warning(default : 4996)
  WriteLog(string(log));
  if (NULL!=monitor_event_collector_)
  {
    monitor_event_collector_->AddEvent(level,DateTime::now(),at_where,event_id,string(what_happened));
  }
}

void wmdf::Logger::WriteFormatEventLog( LogLevel level,int32_t event_id,const std::string& at_where,std::string what_happened,... )
{
  char buff[300] = {0};
  va_list pArgList;
  va_start(pArgList,what_happened);
#pragma warning(disable : 4996)
  vsprintf(buff,what_happened.c_str(),pArgList);
  va_end(pArgList);
  char log[500] = {0};
  sprintf(log,"%s %s[%d] %s: %s",DateTime::now2str().c_str(),Log_Level_Str[level].c_str(),event_id,at_where.c_str(),buff);

  WriteLog(string(log));
  if (NULL!=monitor_event_collector_)
  {
    monitor_event_collector_->AddEvent(level,DateTime::now(),at_where,event_id,string(buff));
  }
}

void wmdf::Logger::WriteFormatLog( LogLevel log_level,std::string format,... )
{
  char buff[500] = {0};
  va_list pArgList;
  va_start (pArgList,format);
  #pragma warning(disable : 4996)
  vsprintf(buff,format.c_str(),pArgList);
  #pragma warning(default : 4996)
  va_end(pArgList);
  WriteLog(log_level,std::string(buff));
}


wmdf::Logger* wmdf::Logger::instance()
{
  return instance_;
}

void wmdf::Logger::StartFileAppender()
{
  if (NULL == file_appender_)
  {
    file_appender_ = new FileAppender();
    file_appender_->Start();
  }
}

void wmdf::Logger::SetMonitorEventCollector( IMonitorEventCollector* collector )
{
  monitor_event_collector_  = collector;
}

wmdf::FileAppender::FileAppender( void )
{
  working_ = false;
  file_ = NULL;
}

wmdf::FileAppender::~FileAppender( void )
{
  Stop();
}

void wmdf::FileAppender::Start()
{
  if (!working_)
  {
    working_ = true;
    date_ = DateTime::today();
    work_thread_ = new boost::thread(boost::bind(&FileAppender::WriteLog,this));
  }
}

void wmdf::FileAppender::Stop()
{
  if (working_)
  {
    working_ = false;
    work_thread_->join();
    delete work_thread_;
    work_thread_ = NULL;
    buffer_.clear();
    if (NULL != file_)
    {
      file_->Close();
      delete file_;
      file_ = NULL;
    }
  }
}

void wmdf::FileAppender::AppendLog( const std::string& log )
{
  boost::mutex::scoped_lock lock(mutex_);
  buffer_.push_back(log);
  condition_.notify_one();
}

void wmdf::FileAppender::WriteLog()
{
  while (working_)
  {
    boost::mutex::scoped_lock lock(mutex_);
    while(buffer_.size()==0)
    {
      condition_.wait(lock);
    }
    PrepareLogFile();
    for (uint32_t index=0;index<buffer_.size();index++ )
    {
      file_->Write((unsigned char*)buffer_[index].c_str(),buffer_[index].length());
    }
    file_->Flush();
    buffer_.clear();
  }
}

void wmdf::FileAppender::PrepareLogFile()
{
  date_t today = DateTime::today();
  if (date_ != today)
  {
    if (NULL != file_)
    {
      file_->Close();
      delete file_;
      file_ = NULL;
    }
    date_ = today;
  }
  if (NULL == file_)
  {
    CreateLogFile();
  }
}

void wmdf::FileAppender::CreateLogFile()
{
  file_ = new WMDFFile();
  std::string filename = DateTime::date2ymd(date_,"-")+".log";
  std::string save_path = ApplicationDirectory::LogPath()+filename;
  if (!Directory::Exists(ApplicationDirectory::LogPath()))
  {
    Directory::CreateDirectories(ApplicationDirectory::LogPath());
  }
  if (!file_->IsExist(save_path.c_str()))
  {
    file_->Open( save_path.c_str(),"w+");
  }
  else
  {
    file_->Open( save_path.c_str(),"a+");
  }
}
