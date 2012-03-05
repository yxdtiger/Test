#include "windindex_adapter.h"
#include "../../../environment/file.h"
#include "../../../monitor/logger.h"
#include "../../../protocol/mdf_message.h"
#include "../../../include/date_time.h"
#include "../../../environment/utils.h"
#include "windindex_file.h"





wmdf::WindIndexAdapter::WindIndexAdapter( ProductSource *product_source )
:IProductAdapter(product_source)
{
	work_thread_ = NULL;
	windindex_file_ = new WindIndexFile(product_source->ip_address());
}

wmdf::WindIndexAdapter::~WindIndexAdapter()
{
	Stop();
	if(windindex_file_!=NULL)
	{
		delete windindex_file_;
		windindex_file_ = NULL;
	}
}

void wmdf::WindIndexAdapter::Start()
{
	if(!is_working_)
	{
		if(!windindex_file_->IsExist())
		{
			Logger::instance()->WriteLog(LL_ERROR,"WindIndex file doesn't exist.");
			return;
		}

		if(windindex_file_->Open())
		{
			is_working_ = true;
			work_thread_ = new boost::thread(boost::bind(&WindIndexAdapter::Dispatch,this));
			Logger::instance()->WriteLog(LL_INFO,"WindIndex Adapter has started.");
		}
		else
		{
			Logger::instance()->WriteLog(LL_INFO,"Failed to open WindIndex file.");
		}
	}
}

void wmdf::WindIndexAdapter::Stop()
{
	if (is_working_)
	{
		is_working_ = false;
		work_thread_->join();
		windindex_file_->Close();
		Logger::instance()->WriteLog(LL_INFO,"WindIndex Adapter stoped.");
	}
}

void wmdf::WindIndexAdapter::Dispatch()
{
	while(is_working_)
	{
		if(windindex_file_->IsModified())
		{
			FetchRecords();
			PackAndSend();
		}
	}
	Utils::SleepTime(product_source_->port());
}

void wmdf::WindIndexAdapter::FetchRecords()
{
	int32_t num_of_windindex = 0;
	WindIndex *windindex_data = windindex_file_->ReadWindIndex(num_of_windindex);

	for(int32_t i=0;i!=num_of_windindex;++i)
	{
		IndexSnapshot *index = FetchARecord(&windindex_data[i]);
		if(index != NULL)
			index_data_.push_back(index);
	}
	delete[] windindex_data;
}

void wmdf::WindIndexAdapter::PackAndSend()
{
	int size = index_data_.size();
	int index = 0;
	for(int i= 0;i!=(size+DEFAULT_RECORDS_PER_MESSAGE_-1)/DEFAULT_RECORDS_PER_MESSAGE_;++i)
	{
		int count = (size - index)<DEFAULT_RECORDS_PER_MESSAGE_?(size - index):DEFAULT_RECORDS_PER_MESSAGE_;
		MdfMessage* mdf_message = CreateMDFMessage(index,count);
		PublishMdfMessage(mdf_message);
		index += count;
	}
	DestroyRecords();
}


void wmdf::WindIndexAdapter::DestroyRecords()
{
	for(uint32_t i=0;i<index_data_.size();i++)
	{
		if(index_data_[i]!=NULL)
		{
			delete index_data_[i];
			index_data_[i] = NULL;
		}
	}
	index_data_.clear();
}

bool wmdf::WindIndexAdapter::is_connected()
{
	return is_working_;
}

wmdf::MdfMessage* wmdf::WindIndexAdapter::CreateMDFMessage( int32_t index,int32_t count )
{
	MdfMessage* mdf_message = new MdfMessage();
	mdf_message->set_local_time(DateTime::get_current_time());

	MarketMessageHeader* header = new MarketMessageHeader();
	header->place_holder = 0;
	mdf_message->set_header(header);


	mdf_message->set_product_id(PRODUCT_WINDINDEX_LEVEL1_ID);
	mdf_message->set_message_id(WIND_INDEXDATA_ID);
	mdf_message->set_major_version(MAJOR_VERSION_WINDINDEX_ID);
	mdf_message->set_minor_version(MINOR_VERSION_WINDINDEX_ID);
	mdf_message->set_record_count(count);
	mdf_message->set_packet_num(++packet_id_);

	IndexSnapshot *recs = new IndexSnapshot[count];
	mdf_message->set_body(recs);
	for(int i=0;i!=count;++i,++index)
	{
		memcpy(&recs[i],index_data_[index],sizeof(IndexSnapshot));
	}
	return mdf_message;
}

IndexSnapshot* wmdf::WindIndexAdapter::FetchARecord(WindIndex *windindex_data)
{
	if(windindex_data!=NULL)
	{
		IndexSnapshot *record = new IndexSnapshot();
		memset(record,0,sizeof(IndexSnapshot));
		char type = windindex_data->windcode[0];
		if(type == 9)
		{
			memcpy(record->wind_code,&windindex_data->windcode[1],9);
		}
		else if(type == 11)
		{
			memcpy(record->wind_code,&windindex_data->windcode[1],11);
		}
		record->data_time_stamp = DateTime::get_current_time();
		record->high_price = windindex_data->highprice;
		record->low_price =  windindex_data->lowprice;
		record->new_price = windindex_data->newprice;
		record->open_price = windindex_data->openprice;
		record->pre_close = windindex_data->preclose;
		record->total_amount = windindex_data->amount;
		record->total_volume = (int64_t)windindex_data->volume;
		return record;
	}
	else
		return NULL;
}








