#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <boost/thread/mutex.hpp>
#include <boost/thread/condition.hpp>
#include <boost/thread.hpp>
#include "../include/wmdf_api.h"

namespace wmdf
{
  enum LogLevel
  {
    LL_FATAL = 0,
    LL_ERROR = 1,
    LL_WARN = 2,
    LL_INFO = 3,
    LL_DEBUG = 4,
  };

  static const std::string Log_Level_Str[5]={"Fatal","Error","Warn","Info","Debug"};

  class FileAppender;
  class WMDFFile;
  class IMonitorEventCollector;

  class Logger
  {
  public:
    static Logger* instance();
    void WriteEventLog(LogLevel level,int32_t event_id,const std::string& at_where,const std::string& what_happened);
    void WriteFormatEventLog(LogLevel level,int32_t event_id,const std::string& at_where,std::string what_happened,...);
    void WriteFormatLog(LogLevel log_level,std::string format,...);
    void WriteLog(LogLevel log_level, const std::string& log);   
    void StartFileAppender();
    void SetMonitorEventCollector(IMonitorEventCollector* collector);
  private:
    void WriteLog(const std::string& log);
    void Fatal(const std::string& log);
    void Error(const std::string& log);
    void Warn(const std::string& log);
    void Info(const std::string& log);
    void Debug(const std::string& log);
  private:
    void AddToMonitor();
  private:
    Logger(void);
    virtual ~Logger(void);
    static Logger* instance_;
    FileAppender* file_appender_;
    IMonitorEventCollector* monitor_event_collector_;
  };

  class FileAppender
  {
  public:
    FileAppender(void);
    virtual ~FileAppender(void);
    void Start();
    void Stop();
    void AppendLog(const std::string& log);
  private:
    void WriteLog();
    void PrepareLogFile();
    void CreateLogFile();
  private:
    boost::mutex mutex_;
    boost::condition condition_;
    boost::thread* work_thread_;
    bool working_;
    WMDFFile* file_;
    std::vector<std::string> buffer_;
    date_t date_;
  };
}

#endif
