#ifndef CAPTURE_TIMER_H
#define CAPTURE_TIMER_H
#include <boost/thread.hpp>
#include <string>
#include "../../include/wmdf_api.h"
namespace wmdf
{
  class CaptureTimer
  {
  public:
    CaptureTimer(int16_t product_id);
    virtual ~CaptureTimer(void);

    void Start();
    void Stop();
  private:
    void Capture();

    int16_t product_id_;
    int32_t time_span_;

    date_t next_date_;
    int32_t start_time_;
    int32_t end_time_;
    std::string file_path_;

    bool is_working_;    
    boost::thread* work_thread_;
  };
}

#endif
