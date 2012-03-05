#include "productdata_logger.h"
#include <boost/bind.hpp>
#include "../environment/wmdf_file.h"
#include "../environment/application_directory.h"
#include "../environment/utils.h"
#include "../environment/strutils.h"
#include "../include/date_time.h"
#include "../protocol/protocol_description.h"

using namespace std;

wmdf::ProductRawData::ProductRawData(int16_t product_id, uint8_t* rawdata, int32_t length )
{
  product_id_ = product_id;
  rawdata_ = rawdata;
  length_ = length;
}

wmdf::ProductRawData::~ProductRawData()
{
  if (NULL != rawdata_)
  {
    delete[] rawdata_;
		rawdata_ = NULL;
  }
}

wmdf::ProductDataLogger::ProductDataLogger(void)
{
	working_=false;
}

wmdf::ProductDataLogger::~ProductDataLogger(void)
{
  std::map<int16_t,ProtocolDescription*>::iterator iter = protocol_descriptions_.begin();
  while(iter != protocol_descriptions_.end())
  {
    delete iter->second;
    iter++;
  }
  protocol_descriptions_.clear();
  Stop();
}

void wmdf::ProductDataLogger::AddToBuffer(ProductRawData* raw_data)
{
  boost::mutex::scoped_lock lock(mutex_);
  buffer_.push_back(raw_data);
}

void wmdf::ProductDataLogger::Start()
{
  if (!working_)
  {
    working_ = true;
    date_ = DateTime::today();
    tempbuff_.clear();
    work_thread_ = new boost::thread(boost::bind(&ProductDataLogger::WriteBuffer,this));
  }
}

void wmdf::ProductDataLogger::Stop()
{
  if (working_)
  {
    working_ = false;
    work_thread_->join();
    delete work_thread_;
    boost::mutex::scoped_lock lock(mutex_);
    std::vector<ProductRawData*>::iterator iter = buffer_.begin();
    while (iter != buffer_.end())
    {
      delete *iter;
			iter++;
    }
    buffer_.clear();
    CloseAllFile();
  }
}

void wmdf::ProductDataLogger::WriteBuffer()
{
  while (working_)
  {
    Utils::SleepTime(5000);
    //如果日期换了的话重新生成文件
    date_t today = DateTime::today();
    if (date_ != today)
    {
      CloseAllFile();
      date_ = today;
    }
    if  (buffer_.size() > 0)
    {
      boost::mutex::scoped_lock lock(mutex_);  
      buffer_.swap(tempbuff_);
    }

    for (uint32_t index =0;index<tempbuff_.size();index++)
    {
      ProductRawData* data = tempbuff_[index];
      WMDFFile* w_file = FetchProductFile(data->product_id());
      if (NULL != w_file)
      {
        w_file->Write(data->rawdata(),data->length());
        w_file->Flush();
      }
      delete data;
    }
    tempbuff_.clear();
  }
}

void wmdf::ProductDataLogger::RegisterProduct( ProtocolDescription* protocol_description_ )
{
  protocol_descriptions_.insert(make_pair(protocol_description_->product_id(),protocol_description_));
}

wmdf::WMDFFile* wmdf::ProductDataLogger::FetchProductFile( int16_t product_id )
{
  std::map<int16_t, WMDFFile*>::iterator iter = product_files_.find(product_id);
  if (iter != product_files_.end())
  {
    return iter->second;
  }
  else
  {
    std::map<int16_t, ProtocolDescription*>::iterator iter = protocol_descriptions_.find(product_id);
    if (iter != protocol_descriptions_.end())
    {
      ProtocolDescription* pd =  iter->second;

      WMDFFile* file = new WMDFFile();
      std::string save_path = StrUtils::formatstring(256,"%s%d_%s.dat",ApplicationDirectory::LogPath().c_str(),product_id,DateTime::date2ymd(date_,"").c_str());

			if (!file->IsExist(save_path.c_str()))
			{
				file->Open( save_path.c_str(),"wb+");
				//写文件头
				file->Write(DateTime::today());
				file->Write(pd->product_id());
				file->Write(pd->major_version());
				file->Write(pd->minor_version());
				file->Write((int32_t)pd->codec_type());
			}
			else
			{
				file->Open( save_path.c_str(),"ab+");
        file->SeekToEnd();
			}
			product_files_.insert(make_pair(pd->product_id(),file));

      return file;
    }
    return NULL;
  }
}

void wmdf::ProductDataLogger::CloseAllFile()
{
  std::map<int16_t,WMDFFile*>::iterator iter = product_files_.begin();
  while(iter != product_files_.end())
  {
    iter->second->Flush();
    iter->second->Close();
    delete iter->second;
    iter->second = NULL;
    iter++;
  }
  product_files_.clear();
}
