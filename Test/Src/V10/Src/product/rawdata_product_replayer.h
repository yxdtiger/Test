#ifndef RAWDATA_PRODUCT_REPLAYER_H
#define RAWDATA_PRODUCT_REPLAYER_H

#include <string>
#include <vector>
#include <map>
#include <boost/thread.hpp>
#include "net_module.h"
#include "../include/wmdf_api.h"
#include "../include/date_time.h"
#include "../session/feed_client.h"
#include "daily_rawdata_file.h"

namespace wmdf
{
  class WMDFFile;
  class RawDataReplayFile;
  class ProductSource;
  class ProtocolDescription;

  class RawDataProductReplayer
  {
  public:
    static RawDataProductReplayer* instance();
    void StartReplay(int16_t product_id,const std::string& filename,int32_t replay_delay);
    void PauseReplay();
    void ResumeReplay();
    void TerminateReplay();
    bool is_paused() {return is_paused_;}
    bool is_replaying() {return is_replaying_;}
  private:
    RawDataProductReplayer(void);
    virtual ~RawDataProductReplayer(void);    
    void Replay();
    void PrepareReplayFile();
    void SendMarketDateSignal();
		void AnalyzeDailyFile(std::vector<std::string>& replay_files);
    void ClearDailyFiles();
    DailyRawdataFile* FetchDailyRawDataFile(int32_t date);
  private:
    static RawDataProductReplayer* instance_;
    int16_t product_id_;
    RawDataReplayFile* curr_replay_file_;
    int32_t curr_date_;
    int32_t replay_delay_;  //每个包之间的延迟时间
    bool is_paused_;
    bool is_replaying_;
    boost::thread* work_thread_;
    std::string extracted_file_;
    int32_t current_day_index_;
    bool last_file_of_curr_day_;
		std::vector<DailyRawdataFile*> daily_rawdata_files_;
  };

  class RawDataReplayFile
  {
  public:
    RawDataReplayFile(int16_t product_id,int32_t curr_date, WMDFFile* file);
    ~RawDataReplayFile(void);
    bool IsEof();
    bool IsFirst();
    void PrepareReplayContext();
    void ForwardOne(Message* msg);
    Message* FetchNextMessage();
    void ResetContext();
  private:
    Message* curr_message_;
    WMDFFile* file_;
    int64_t file_size_;
    ProductSource* product_source_;
    int16_t product_id_;
    IClientCallback* client_callback_;
    int32_t packet_num_;
    int32_t progress_;
		int32_t curr_date_;
		bool is_unrar_file_;
		const static int32_t read_size_ = 256*1024;
  };






}

#endif
