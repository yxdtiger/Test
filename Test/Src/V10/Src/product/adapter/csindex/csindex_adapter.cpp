#include "csindex_adapter.h"
#include "../../../environment/file.h"
#include "../../../monitor/logger.h"
#include "../../../environment/utils.h"
#include "../../../protocol/mdf_message.h"
#include "../../../include/date_time.h"
#include "../../../environment/splitter.h"
#include "csindex_file.h"

using namespace std;


const char * wmdf::CSIndexAdapter::suffix = ".CSI";

wmdf::CSIndexAdapter::CSIndexAdapter( ProductSource *product_source )
:IProductAdapter(product_source)
{
	work_thread_ = NULL;
	csindex_file_ = new CSIndexFile(product_source->ip_address());
	splitter_ = new Splitter("","|");
}

wmdf::CSIndexAdapter::~CSIndexAdapter()
{
	Stop();
	if(csindex_file_!=NULL)
	{
		delete csindex_file_;
		csindex_file_=NULL;
	}
}

void wmdf::CSIndexAdapter::Start()
{
	if(!is_working_)
	{
		if(!csindex_file_->IsExist())
		{
			Logger::instance()->WriteLog(LL_ERROR,"CSIndex file doesn't exist.");
			return;
		}

		if(csindex_file_->Open())
		{
			is_working_ = true;
			work_thread_ = new boost::thread(boost::bind(&CSIndexAdapter::Dispatch,this));
			Logger::instance()->WriteLog(LL_INFO,"CSIndex Adapter has started.");
		}
		else
		{
			Logger::instance()->WriteLog(LL_ERROR,"Failed to open CSIndex file.");
		}
	}
}


void wmdf::CSIndexAdapter::Stop()
{
	if(is_working_)
	{
		is_working_ = false;
		work_thread_->join();
		csindex_file_->Close();
		Logger::instance()->WriteLog(LL_INFO,"CSIndex Adapter stoped.");
	}
}


void wmdf::CSIndexAdapter::Dispatch()
{
	while(is_working_)
	{
		if(csindex_file_->IsModified())
		{
			FetchRecords();
			PackAndSend();
		}
		Utils::SleepTime(product_source_->port());
	}
}


void wmdf::CSIndexAdapter::FetchRecords()
{
	receive_size_  += csindex_file_->FileSize();
	receive_packet_count_+=1;

	int32_t current_line_number = 0;
	while(!csindex_file_->IsEOF())
	{
		csindex_file_->ReadLine();
		++current_line_number;
		if(current_line_number > 1)
		{
			IndexSnapshot *data = ProcessLine();
			if(data != NULL)
			{
				index_data_.push_back(data);
			}
		}
	}
}

void wmdf::CSIndexAdapter::PackAndSend()
{
	int size = index_data_.size();
	int index = 0;
	for(int i= 0;i!= (size+DEFAULT_RECORDS_PER_MESSAGE-1)/DEFAULT_RECORDS_PER_MESSAGE;++i)
	{
		int count = (size - index)<DEFAULT_RECORDS_PER_MESSAGE?(size - index):DEFAULT_RECORDS_PER_MESSAGE;
		MdfMessage* mdf_message = CreateMDFMessage(index,count);
		PublishMdfMessage(mdf_message);
		index += count;
	}
	DestroyRecords();
}



wmdf::MdfMessage* wmdf::CSIndexAdapter::CreateMDFMessage( int32_t index,int count )
{
	MdfMessage* mdf_message = new MdfMessage();
	mdf_message->set_local_time(DateTime::get_current_time());

	MarketMessageHeader* header = new MarketMessageHeader();
	header->place_holder = 0;
	mdf_message->set_header(header);


	mdf_message->set_product_id(PRODUCT_CSINDEX_LEVEL1_ID);
	mdf_message->set_message_id(CS_INDEXDATA_ID);
	mdf_message->set_major_version(MAJOR_VERSION_CSINDEX_ID);
	mdf_message->set_minor_version(MINOR_VERSION_CSINDEX_ID);
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

void wmdf::CSIndexAdapter::DestroyRecords()
{
	for(uint32_t i=0;i<index_data_.size();i++)
	{
		if(index_data_[i] != NULL)
		{
			delete index_data_[i];
			index_data_[i] = NULL;
		}
	}
	index_data_.clear();
}

bool wmdf::CSIndexAdapter::is_connected()
{
	return is_working_;
}

IndexSnapshot* wmdf::CSIndexAdapter::ProcessLine()
{
	std::string line;
	csindex_file_->GetLine(line);
	splitter_->reset(line,"|");
	if((*splitter_)[0]=="01")
	{
		IndexSnapshot *data = new IndexSnapshot();
		memcpy(data->wind_code,(*splitter_)[2].c_str(),sizeof((*splitter_)[2])+1);
		memcpy(&data->wind_code[6],suffix,5);
		data->data_time_stamp = DateTime::get_current_time();
		data->new_price = atof((*splitter_)[6].c_str());
		data->open_price = atof((*splitter_)[7].c_str());
		data->high_price = atof((*splitter_)[8].c_str());
		data->low_price = atof((*splitter_)[9].c_str());
		data->pre_close = atof((*splitter_)[10].c_str());
		data->total_volume = atoi((*splitter_)[14].c_str());
		data->total_amount = atof((*splitter_)[15].c_str());
		return data;
	}
	else
		return NULL;
}

