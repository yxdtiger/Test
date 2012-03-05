#include "dbf_adapter.h"
#include <fcntl.h>
#include "../../monitor/logger.h"
#include "../../include/date_time.h"
#include "../../environment/utils.h"
#include "../../environment/file.h"
using namespace std;

wmdf::DBFAdapter::DBFAdapter(ProductSource* product_source)
:IProductAdapter(product_source),max_fields_num_(0),adapter_name_("DBF Adapter")
{
  dbh_ = NULL;
  last_heart_beat_time_ = 0;
  time_span_ = product_source->port();
  records_per_message_ = DEFAULT_RECORDS_PER_MESSAGE;
  product_source_->set_which_is_in_use(0);
}

wmdf::DBFAdapter::~DBFAdapter(void)
{
  if (is_working_)
  {
    Stop();
  }
  if(dbh_!=NULL)
    dbf_destroy(dbh_);
}

void wmdf::DBFAdapter::Start()
{
  if (!is_working_)
  {
    if(CheckDbfFile())
    {
      is_working_ = true;
      work_thread_ = new boost::thread(boost::bind(&DBFAdapter::Dispatch,this));
      Logger::instance()->WriteFormatLog(LL_INFO,"Product Adapter: %s started.",adapter_name_.c_str());
    }
  }
}

void wmdf::DBFAdapter::Stop()
{
  if (is_working_)
  {
    is_working_ = false;
    work_thread_->join();
    Logger::instance()->WriteFormatLog(LL_INFO,"Product Adapter: %s ended.",adapter_name_.c_str());
  }
}

bool wmdf::DBFAdapter::is_connected()
{
  return is_working_;
}

void wmdf::DBFAdapter::Dispatch()
{
  while(is_working_)
  {
    for(int i = 0;i<3;i++)
    {
      if(Open())
      {
        FetchRecords();
        PackAndSend();
        Close();
        break;
      }
      else 
        Utils::SleepTime(10*i);
    }
    Utils::SleepTime(time_span_);
  }
}

void  wmdf::DBFAdapter::FetchRecords()
{
  //记录接收到的数据大小
  receive_size_  += wmdf::File::FileSize(product_source_->ip_address());
  receive_packet_count_+=1;

  while(dbf_next_record(dbh_))
  {
    void* item = FetchARecord();
    if(NULL != item)
      records_.push_back(item);
  }

}

void wmdf::DBFAdapter::PackAndSend()
{
  int size = records_.size();
  int index = 0;
  for(int i= 0;i< (size+records_per_message_-1)/records_per_message_;i++)
  {
    int count = (size - index)<records_per_message_?(size - index):records_per_message_;
    MdfMessage* mdf_message = CreateMDFMessage(index,count);
    PublishMdfMessage(mdf_message);
    index += count;
  }
  DestroyRecords();
}

void wmdf::DBFAdapter::DestroyRecords()
{
  for(uint32_t i=0;i<records_.size();i++)
  {
    delete records_[i];
    records_[i] = NULL;
  }
  records_.clear();
}

bool wmdf::DBFAdapter::Open()
{
  if(dbh_==NULL)
  {
    for(int i=0;i<3;i++)
    {
      if((dbh_ = dbf_open(product_source_->ip_address().c_str(),O_RDONLY))!=NULL)
      {
        return true;
      }
      Utils::SleepTime(10);
    }
  }
  Logger::instance()->WriteFormatLog(LL_ERROR,"failed to open source dbf:%s .",product_source_->ip_address().c_str());
  return false;
}

bool wmdf::DBFAdapter::CheckDbfFile()
{
  bool flag;
  if(Open())
  {
    if(dbh_->db_nfields<max_fields_num_)
    {
      Logger::instance()->WriteFormatEventLog(LL_ERROR,MEI_SOURCE_DISCONNECTED,"dbf adapter","bad format in source dbf:%s .",product_source_->ip_address().c_str());
      flag = false;
    }
    else
    {
      flag = true;
    }
    Close();
  }
  else
  {
    Logger::instance()->WriteFormatEventLog(LL_ERROR,MEI_SOURCE_DISCONNECTED,"dbf adapter","failed to open source dbf:%s .",product_source_->ip_address().c_str());
    flag = false;
  }
  return flag;
}

bool wmdf::DBFAdapter::Close()
{
  if(dbh_!=NULL)
  {
    dbf_destroy(dbh_);
    dbh_= NULL;
  }
  return true;
}


