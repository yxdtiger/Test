#include "rawdata_product_replayer.h"
#include "../environment/wmdf_file.h"
#include "product_source.h"
#include "adapter/iproduct_adapter.h"
#include "product_adapter_factory.h"
#include "../environment/utils.h"
#include "../session/message_publisher.h"
#include "../protocol/mdf_message.h"
#include "../environment/directory.h"
#include "../monitor/logger.h"
#include "../include/date_time.h"
#include "../environment/file.h"
#include "../environment/strutils.h"

wmdf::RawDataProductReplayer* wmdf::RawDataProductReplayer::instance_ = NULL;

wmdf::RawDataProductReplayer::RawDataProductReplayer(void)
{
  is_paused_ = false;
  is_replaying_ = false;
  replay_delay_ = 15;
  product_id_ = PRODUCT_INVALID_ID;
  current_day_index_ = -1;
	last_file_of_curr_day_ = false;
  curr_date_ = 0;
  extracted_file_ = "";
}

wmdf::RawDataProductReplayer::~RawDataProductReplayer(void)
{
  TerminateReplay();
}

wmdf::RawDataProductReplayer* wmdf::RawDataProductReplayer::instance()
{
  if (NULL == RawDataProductReplayer::instance_)
  {
    RawDataProductReplayer::instance_ = new RawDataProductReplayer();
  }
  return instance_;
}

void wmdf::RawDataProductReplayer::PauseReplay()
{
  is_paused_ = true;
}

void wmdf::RawDataProductReplayer::ResumeReplay()
{
  is_paused_ = false;
}

void wmdf::RawDataProductReplayer::TerminateReplay()
{
  if (is_replaying_)
  {
    is_replaying_ = false;
    work_thread_->join();
    delete work_thread_;
    work_thread_ = NULL;
    if (NULL != curr_replay_file_)
    {
      delete curr_replay_file_;
      curr_replay_file_ = NULL;
    }
    ClearDailyFiles();
    current_day_index_ = -1;
		last_file_of_curr_day_ = false;
  }
}
void wmdf::RawDataProductReplayer::AnalyzeDailyFile(std::vector<std::string>& replay_files)
{
  for(size_t i=0; i!=replay_files.size(); ++i)
  {
    string cur_file = StrUtils::ToLower(replay_files[i]);
    string file_date = "";
    int index1 = cur_file.find_last_of("/");
    int index2 = cur_file.find_last_of("\\");
    int index3 = index1<index2?index2:index1;

    for (size_t index=index3+1;index!=cur_file.size();index++)
    {
      if ((cur_file[index]>='0') && (cur_file[index]<='9'))
      {	
        file_date += cur_file[index]; 
      }
    }
    if (file_date == "")    { continue; }
    int32_t date = atoi(file_date.c_str());
    DailyRawdataFile* rawdatafile = FetchDailyRawDataFile(date);
    if(NULL == rawdatafile)
    {
      rawdatafile = new DailyRawdataFile(date);
      daily_rawdata_files_.push_back(rawdatafile);
    }

    if(cur_file.find("pm")!=std::string::npos)
    {
      rawdatafile->InsertPMFile(cur_file);
    }
    else if(cur_file.find("am")!=std::string::npos)
    {
      rawdatafile->InsertAMFile(cur_file);
    }
    else
    {
      rawdatafile->InsertWholeDayFile(cur_file);			
    }
  }
}
void wmdf::RawDataProductReplayer::StartReplay(int16_t product_id, const std::string& filename,int32_t replay_delay )
{
  if (!is_replaying_)
  {
    is_replaying_ = true;
    product_id_ = product_id;
    replay_delay_ = replay_delay;
    curr_replay_file_ = NULL;
    current_day_index_ = -1;
		last_file_of_curr_day_ = false;
    std::vector<std::string> replay_files_;
    Directory::GetAllFileOfPath(filename,replay_files_);
		AnalyzeDailyFile(replay_files_);
    work_thread_ = new boost::thread(boost::bind(&RawDataProductReplayer::Replay,this));
  }
}

void wmdf::RawDataProductReplayer::Replay()
{
  while (is_replaying_)
  {
    PrepareReplayFile();
    if (current_day_index_ >= (int32_t)daily_rawdata_files_.size())
    {
      Logger::instance()->WriteLog(LL_INFO,"all replay finished");  
      return;
    }
    if ((curr_replay_file_ != NULL) && (!curr_replay_file_->IsEof()) && (!is_paused_))
    {
      Message* msg = curr_replay_file_->FetchNextMessage();
      if (msg == NULL) return;
      curr_replay_file_->ForwardOne(msg);
    }  
    if (replay_delay_ > 0)
    {
      Utils::SleepTime(replay_delay_);
    }
    if ((curr_replay_file_ != NULL) && (curr_replay_file_->IsEof()))
    {
			if(last_file_of_curr_day_)
				SendMarketDateSignal();
      Logger::instance()->WriteFormatLog(LL_INFO,"replay finished. %d",last_file_of_curr_day_);
      
      curr_replay_file_->ResetContext();
      if (curr_replay_file_ != NULL)
      {
        delete curr_replay_file_;
        curr_replay_file_ = NULL;
      }
      
      if (File::Exists(extracted_file_))
      {
        File::Remove(extracted_file_);
      }
    }
  }
}

void wmdf::RawDataProductReplayer::PrepareReplayFile()
{
  if ((NULL != curr_replay_file_)&& (!curr_replay_file_->IsEof()))
  {
    return;
  }
  if ((current_day_index_ < 0))
  {
    current_day_index_ = 0;
    curr_replay_file_ = NULL;
  }
  else
  {
		if(last_file_of_curr_day_)
		{
			current_day_index_++;
      curr_replay_file_ = NULL;
		}
  }
  if (current_day_index_ < (int32_t)daily_rawdata_files_.size())
  {
		DailyRawdataFile* daily_raw_files = daily_rawdata_files_[current_day_index_];
    curr_date_ = daily_raw_files->date();
    printf("\n");
    Logger::instance()->WriteFormatLog(LL_INFO,"******  date: %d, %d of %d  *******",curr_date_,current_day_index_+1, daily_rawdata_files_.size());
    std::string filename = daily_raw_files->GetNextFile();
    if (filename == "") return;
    last_file_of_curr_day_ = daily_raw_files->IsLastFile();

    if (daily_raw_files->IsCompressed(filename))
    {
      daily_raw_files->Uncompress(filename);
      filename = daily_raw_files->GetUncompressedFile(filename);
      extracted_file_ = filename;
    }
    else
    {
      extracted_file_ = "";
    }
    if (File::Exists(filename))
    {
      WMDFFile* file = new WMDFFile();
      file->Open(filename.c_str(),"rb");
      curr_replay_file_ = new RawDataReplayFile(product_id_, curr_date_ ,file);
      curr_replay_file_->PrepareReplayContext();
      Logger::instance()->WriteLog(LL_INFO,"start to replay "+ filename);		
    }
    else
    {
      Logger::instance()->WriteFormatLog(LL_INFO,"file not existed( %s).",filename.c_str());
    }
  }
}

void wmdf::RawDataProductReplayer::SendMarketDateSignal()
{
  MarketDateSignal* date_signal = new MarketDateSignal[1];
  date_signal[0].market_operation = MO_CLOSE;
  date_signal[0].market_date = DateTime::ymd2date(curr_date_);
  date_signal[0].product_id = product_id_;
  MdfMessage* message = new MdfMessage();
  message->set_packet_num(0);
  message->set_local_time(DateTime::get_current_time());
  message->set_socket_id(0);
  message->set_product_id(PRODEUCT_SYSTEM_ID);
  message->set_message_id(SYSTEM_MARKET_DATE_SIGNAL_ID);
  message->set_major_version(MAJOR_VERSION_SYSTEM_ID);
  message->set_minor_version(MINOR_VERSION_SYSTEM_ID);
  message->set_record_count(1);
  message->set_body(date_signal);
  message->set_header(NULL);

  SharedMdfMessage shared_mdf_message(message);
  MessagePublisher::PublishOneMessage(shared_mdf_message);
  Utils::SleepTime(20000);

}

void wmdf::RawDataProductReplayer::ClearDailyFiles()
{
  for(uint32_t index = 0; index != daily_rawdata_files_.size(); ++index)
  {
     delete daily_rawdata_files_[index];
  }
  daily_rawdata_files_.clear();
}

wmdf::DailyRawdataFile* wmdf::RawDataProductReplayer::FetchDailyRawDataFile( int32_t date )
{
  for(uint32_t index = 0; index != daily_rawdata_files_.size(); ++index)
  {
    if (daily_rawdata_files_[index]->date() == date)
    {
      return daily_rawdata_files_[index]; 
    }
  }
  return NULL;
}
/**********************************************************/
/*********************ReplayFile***************************/
/**********************************************************/
wmdf::RawDataReplayFile::RawDataReplayFile(int16_t product_id,int32_t curr_date,WMDFFile* file )
{
  file_ = file;
  product_source_ = NULL;
  product_id_ = product_id;
	curr_date_ = curr_date;
  packet_num_=0;
  file_size_= file->GetSize();
  progress_ = 0;
	is_unrar_file_ = false;
}

wmdf::RawDataReplayFile::~RawDataReplayFile( void )
{
  if ( NULL != file_ )
  {
    file_->Close();
    delete file_;
    file_ = NULL;
  }
  if (NULL != product_source_)
  {
    delete product_source_;
    product_source_ = NULL;
  }
  if (NULL!=client_callback_)
  {
    delete client_callback_;
    client_callback_ = NULL;
  }
}

bool wmdf::RawDataReplayFile::IsEof()
{
  int64_t pos = 0;
  file_->GetPos(pos);
  return pos == file_size_;
}

bool wmdf::RawDataReplayFile::IsFirst()
{
  int64_t pos = 0;
  if (file_->GetPos(pos))
  {
    return pos == 0;
  }
  return false;
}

Message* wmdf::RawDataReplayFile::FetchNextMessage()
{
  int32_t msg_body_len = read_size_;
  int64_t pos = 0;
  file_->GetPos(pos);
  int64_t left_size = (file_size_ - pos);
  if (left_size < msg_body_len)
  {
    msg_body_len = (int32_t)left_size;
  }
  Message* msg = CreateMessage(msg_body_len);
  msg->local_time = DateTime::get_current_time();
  msg->packet_num = packet_num_++;
  msg->product_id = product_id_;
  msg->message_id = 0;
  file_->Read(msg->data,msg_body_len);
  
  file_->GetPos(pos);
  int32_t progress = (int32_t)((pos*1.0)/(file_size_*1.0) *100);
  if (progress > progress_ )
  {
    progress_ = progress;
    Logger::instance()->WriteFormatLog(LL_INFO,"Progress: %d ", progress_);
  }
  return msg;
}

void wmdf::RawDataReplayFile::ForwardOne( Message* msg )
{
  if (NULL!=client_callback_)
  {
    client_callback_->OnRecvMsg(msg);
    ReleaseMessage(msg);
  }
}

void wmdf::RawDataReplayFile::PrepareReplayContext()
{
  product_source_ =  new ProductSource();
  product_source_->set_codec_type(CT_RAWDATA);
  product_source_->set_product_id(product_id_);
	client_callback_ = ProductAdapterFactory::CreateDispatcher(product_source_,DateTime::ymd2date(curr_date_));
  assert(client_callback_ != NULL);
}

void wmdf::RawDataReplayFile::ResetContext()
{
  client_callback_->ResetContext();
}
