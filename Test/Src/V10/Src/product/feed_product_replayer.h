#ifndef FEED_PRODUCT_REPLAYER_H
#define FEED_PRODUCT_REPLAYER_H

#include <string>
#include "../include/wmdf_api.h"
#include "../session/feed_client.h"
#include "../include/date_time.h"
#include "net_module.h"
#include <boost/thread.hpp>

namespace wmdf
{
  class WMDFFile;
  class ReplayFile;
  class ProductSource;
  class ProtocolDescription;
  
  class FeedProductReplayer
  {
  public:
    static FeedProductReplayer* instance();
    void StartReplay(const std::string& filename,double speed);
    void PauseReplay();
    void ResumeReplay();
    void TerminateReplay();
    bool is_paused() {return is_paused_;}
    bool is_replaying() {return is_replaying_;}
  private:
    FeedProductReplayer(void);
    virtual ~FeedProductReplayer(void);    
    void Replay();
    void ProcessReplay();
  private:
    static FeedProductReplayer* instance_;
    std::string replay_file_name_;
    ReplayFile* replay_file_;
    double replay_speed_;
    bool is_paused_;
    bool is_replaying_;
    time_stamp_t last_time_; //
    time_stamp_t last_tick_;
    boost::thread* work_thread_;
  };

  class ReplayFile
  {
  public:
    ReplayFile(WMDFFile* file);
    ~ReplayFile(void);
    bool IsEof();
    bool IsFirst();
    void PrepareReplayContext();
    void ForwardOne();
    Message* FetchNextMessage();
  private:
    Message* curr_message_;
    WMDFFile* file_;
    int64_t file_size_;
    int64_t curr_position_;
    ProductSource* product_source_;
    ProtocolDescription* protocol_description_;
    IClientCallback* client_callback_;
    bool is_first_;
    int32_t progress_;
  };






}

#endif
