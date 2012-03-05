#include "ciq_adapter.h"
#include "../../../environment/file.h"
#include "../../../monitor/logger.h"
#include "../../../protocol/mdf_message.h"
#include "../../../environment/utils.h"
#include "../../../include/date_time.h"
#include <fcntl.h>
#include "ciq_file.h"


using namespace std;

const char * wmdf::CIQAdapter::suffix_ = ".CI";


wmdf::CIQAdapter::CIQAdapter( ProductSource *product_source )
:IProductAdapter(product_source)
{
	work_thread_ = NULL;
	ciq_file_ = new CIQFile(product_source->ip_address());
}

wmdf::CIQAdapter::~CIQAdapter()
{
	Stop();
	if(ciq_file_ != NULL)
	{
		delete ciq_file_;
		ciq_file_ = NULL;
	}
}

void wmdf::CIQAdapter::Start()
{
	if(!is_working_)
	{
		if(!ciq_file_->IsExist())
		{
			Logger::instance()->WriteLog(LL_ERROR,"CIQ file doesn't exist.");
			return;
		}

		if(ciq_file_->Open())
		{
			is_working_ = true;
			work_thread_ = new boost::thread(boost::bind(&CIQAdapter::Dispatch,this));
			Logger::instance()->WriteLog(LL_INFO,"CIQ Adapter has started.");
		}
		else
		{
			Logger::instance()->WriteLog(LL_INFO,"Failed to open CIQ file.");
		}
	}
}

void wmdf::CIQAdapter::Stop()
{
	if (is_working_)
	{
		is_working_ = false;
		work_thread_->join();
		ciq_file_->Close();
		Logger::instance()->WriteLog(LL_INFO,"CIQ Adapter stoped.");
	}
}

void wmdf::CIQAdapter::Dispatch()
{
	while(is_working_)
	{
		if(ciq_file_->IsModified())
		{
			FetchRecords();
			PackAndSend();
		}
		Utils::SleepTime(product_source_->port());
	}
}


void wmdf::CIQAdapter::FetchRecords()
{
	int32_t num_of_ciqindex = 0;
	CIQIndex *ciq_index = ciq_file_->ReadCIQIndex(num_of_ciqindex);

	for(int32_t i=0;i!=num_of_ciqindex;++i)
	{
		IndexSnapshot *index = FetchARecord(&ciq_index[i]);
		if(index!=NULL)
			index_data_.push_back(index);
	}
	delete[] ciq_index;

}

void wmdf::CIQAdapter::PackAndSend()
{
	int32_t size = index_data_.size();
	int32_t index = 0;
	for(int i= 0;i!=(size+DEFAULT_RECORDS_PER_MESSAGE_-1)/DEFAULT_RECORDS_PER_MESSAGE_;++i)
	{
		int count = (size - index)<DEFAULT_RECORDS_PER_MESSAGE_?(size - index):DEFAULT_RECORDS_PER_MESSAGE_;
		MdfMessage* mdf_message = CreateMDFMessage(index,count);
		PublishMdfMessage(mdf_message);
		index += count;
	}
	DestroyRecords();
}

IndexSnapshot * wmdf::CIQAdapter::FetchARecord( CIQIndex *data )
{
	if(data!=NULL)
	{
		IndexSnapshot *record = new IndexSnapshot();
		memset(record,0,sizeof(IndexSnapshot));
		record->data_time_stamp = DateTime::get_current_time();

		memcpy(record->wind_code,data->windcode,sizeof(data->windcode));
		memcpy(&record->wind_code[6],suffix_,4);
		char price[9] = {0};
		char amount[13] = {0};
		char volume[11] = {0};
		memcpy(price,data->preclose,sizeof(data->preclose));
		record->pre_close = atof(price);
		memcpy(price,data->openprice,sizeof(data->openprice));
		record->open_price = atof(price);
		memcpy(price,data->highprice,sizeof(data->highprice));
		record->high_price = atof(price);
		memcpy(price,data->lowprice,sizeof(data->lowprice));
		record->low_price = atof(price);
		memcpy(price,data->newprice,sizeof(data->newprice));
		record->new_price = atof(price);
		memcpy(amount,data->amount,sizeof(data->amount));
		record->total_amount = atof(amount);
		memcpy(volume,data->volume,sizeof(data->volume));
		record->total_volume = (int64_t)atoi(volume);
		return record;
	}
	else
		return NULL;
}


wmdf::MdfMessage* wmdf::CIQAdapter::CreateMDFMessage( int32_t index,int32_t count )
{
	MdfMessage* mdf_message = new MdfMessage();
	mdf_message->set_local_time(DateTime::get_current_time());

	MarketMessageHeader* header = new MarketMessageHeader();
	header->place_holder = 0;
	mdf_message->set_header(header);

	mdf_message->set_product_id(PRODUCT_CIQINDEX_LEVEL1_ID);
	mdf_message->set_message_id(CIQ_INDEXDATA_ID);
	mdf_message->set_major_version(MAJOR_VERSION_CIQINDEX_ID);
	mdf_message->set_minor_version(MINOR_VERSION_CIQINDEX_ID);
	mdf_message->set_record_count(count);
	mdf_message->set_packet_num(++packet_id_);

	IndexSnapshot* recs = new IndexSnapshot[count];
	mdf_message->set_body(recs);
	for(int i=0;i!=count;++i,++index)
	{
		memcpy(&recs[i],index_data_[index],sizeof(IndexSnapshot));
	}
	return mdf_message;
}

bool wmdf::CIQAdapter::is_connected()
{
	return is_working_;
}

void wmdf::CIQAdapter::DestroyRecords()
{
	for(uint32_t i=0;i<index_data_.size();i++)
	{
		delete index_data_[i];
		index_data_[i] = NULL;
	}
	index_data_.clear();
}


