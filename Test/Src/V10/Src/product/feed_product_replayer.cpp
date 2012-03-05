#include "feed_product_replayer.h"
#include "../environment/wmdf_file.h"
#include "../protocol/protocol_description.h"
#include "product_source.h"
#include "product_adapter_factory.h"
#include "../protocol/mdf_message.h"
#include "../session/message_publisher.h"
#include "adapter/iproduct_adapter.h"
#include "replay_adapter.h"
#include "../environment/utils.h"
#include "../monitor/logger.h"

wmdf::FeedProductReplayer* wmdf::FeedProductReplayer::instance_ = NULL;

wmdf::FeedProductReplayer::FeedProductReplayer(void)
{
  is_paused_ = false;
  is_replaying_ = false;
  replay_speed_ = 1;
}

wmdf::FeedProductReplayer::~FeedProductReplayer(void)
{
  TerminateReplay();
}

wmdf::FeedProductReplayer* wmdf::FeedProductReplayer::instance()
{
  if (NULL == FeedProductReplayer::instance_)
  {
    FeedProductReplayer::instance_ = new FeedProductReplayer();
  }
  return instance_;
}

void wmdf::FeedProductReplayer::PauseReplay()
{
  is_paused_ = true;
}

void wmdf::FeedProductReplayer::ResumeReplay()
{
  is_paused_ = false;
}

void wmdf::FeedProductReplayer::TerminateReplay()
{
  if (is_replaying_)
  {
    is_replaying_ = false;
    work_thread_->join();
    delete work_thread_;
    work_thread_ = NULL;
    if (NULL != replay_file_)
    {
      delete replay_file_;
      replay_file_ = NULL;
    }
  }
}

void wmdf::FeedProductReplayer::StartReplay(const std::string& filename,double speed )
{
  if (!is_replaying_)
  {
    is_replaying_ = true;
    replay_speed_ = speed;
    WMDFFile* file = new WMDFFile();
    file->Open(filename.c_str(),"rb");
    file->SeekToBegin();
    replay_file_ = new ReplayFile(file);
    replay_file_->PrepareReplayContext();
    Logger::instance()->WriteLog(LL_INFO,"start to replay "+ filename);
    work_thread_ = new boost::thread(boost::bind(&FeedProductReplayer::Replay,this));
  }
}

void wmdf::FeedProductReplayer::Replay()
{
  while (is_replaying_)
  {
    ProcessReplay();
    if (replay_file_->IsEof())
    {
      Logger::instance()->WriteLog(LL_INFO,"replay finished");
      return;
    }
    //Utils::SleepTime(1);
  }
}

void wmdf::FeedProductReplayer::ProcessReplay()
{
  if (replay_file_->IsEof())
  {
    return;
  }
  if (replay_file_->IsFirst())
  {
    Message* msg = replay_file_->FetchNextMessage();
    if (msg == NULL) return;
    last_time_ = msg->local_time;
    last_tick_ = DateTime::get_current_time();
    replay_file_->ForwardOne();
    return;
  }
  time_stamp_t time_span = DateTime::get_current_time() - last_tick_;
  while ((!replay_file_->IsEof()) && (!is_paused_) && (is_replaying_))
  {
    Message* msg = replay_file_->FetchNextMessage();
    if (msg == NULL) return;
    time_stamp_t msg_time_span = msg->local_time-last_time_;
    if (msg_time_span>60000)//大于一分钟
    {
      last_time_ = msg->local_time;
      last_tick_ = DateTime::get_current_time();
      replay_file_->ForwardOne();
      return;
    }
    if (msg_time_span < time_span * replay_speed_)
    {
      replay_file_->ForwardOne();
    }
    else
    {
      break;
    }
  }
}

/**********************************************************/
/*********************ReplayFile***************************/
/**********************************************************/
wmdf::ReplayFile::ReplayFile( WMDFFile* file )
{
  file_ = file;
  product_source_ = NULL;
  protocol_description_ = NULL;
  is_first_ = false;
  progress_ = 0;
  file_size_= file->GetSize();
  curr_message_ = NULL;
}

wmdf::ReplayFile::~ReplayFile( void )
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
  if (NULL != protocol_description_)
  {
    delete protocol_description_;
    protocol_description_ = NULL;
  }
}

bool wmdf::ReplayFile::IsEof()
{
  int64_t pos = 0;
  file_->GetPos(pos);
  return pos == file_size_;
}

bool wmdf::ReplayFile::IsFirst()
{
  return is_first_;
}

Message* wmdf::ReplayFile::FetchNextMessage()
{
  if ( NULL == curr_message_ )
  {
    uint8_t STX=0;
    file_->Read(STX);
    uint32_t msg_len=0;
    file_->Read(msg_len);

    int64_t pos = 0;
    file_->GetPos(pos);
    int64_t left_size = (file_size_ - pos);
    if ((left_size < MSG_HEADER_LEN) || (left_size < msg_len))
    {
      return NULL;
    }
    curr_message_ = CreateMessage(msg_len-MSG_HEADER_LEN);
    curr_message_->STX = STX;
    curr_message_->msg_len = msg_len;
    file_->Read(curr_message_->local_time);
    file_->Read(curr_message_->packet_num );
    file_->Read(curr_message_->product_id);
    file_->Read(curr_message_->message_id);
    file_->Read(curr_message_->placeholder,2);
    file_->Read(curr_message_->check_sum);
    file_->Read(curr_message_->data,msg_len-MSG_HEADER_LEN);

    file_->GetPos(pos);
    int32_t progress = (int32_t)((pos*1.0)/(file_size_*1.0) *100);
    if (progress > progress_ )
    {
      progress_ = progress;
      Logger::instance()->WriteFormatLog(LL_INFO,"Progress: %d ", progress_);
    }
    return curr_message_;
  }
  else
  {
    return curr_message_;
  }
}

void wmdf::ReplayFile::PrepareReplayContext()
{
  date_t date = 0;
  file_->Read(date);
  int16_t product_id = 0;
  file_->Read(product_id);
  uint8_t major_version = 0;
  file_->Read(major_version);
  uint8_t minor_version = 0;
  file_->Read(minor_version);
  int32_t code_type = 0;
  file_->Read(code_type);
  is_first_ = true;

  protocol_description_ = new ProtocolDescription(product_id,major_version,minor_version,(CodecType)code_type);

  if ((protocol_description_->codec_type()==CT_BINARY)
    || (protocol_description_->codec_type()==CT_FAST))
  {
    client_callback_ = new ReplayAdapter(protocol_description_);
  }

#ifdef WMDFADAPTER_
  else   //CT_FEEDHANDLER
  {
    product_source_ =  new ProductSource();
    product_source_->set_codec_type(protocol_description_->codec_type());
    product_source_->set_product_id(product_id);
    client_callback_ = ProductAdapterFactory::CreateDispatcher(product_source_,date);
  }
#endif
  assert(client_callback_ != NULL);
}

void wmdf::ReplayFile::ForwardOne()
{
  if ((NULL != curr_message_) && (client_callback_ != NULL))
  {
    client_callback_->OnRecvMsg(curr_message_);
    ReleaseMessage(curr_message_);
    curr_message_ = NULL;
  }
  is_first_ = false;
}

