#include "swzs_adapter.h"
#include "../../include/wmdf_structs.h"
#include "../../include/date_time.h"


wmdf::SWIndexAdapter::SWIndexAdapter( ProductSource* product_source )
:DBFAdapter(product_source)
{
	max_fields_num_ = SWZS_FIELD_NUMBER;
	adapter_name_  = "SWZS_Adapter";
}

wmdf::SWIndexAdapter::~SWIndexAdapter( void )
{

}

const char * wmdf::SWIndexAdapter::swzs_suffix_ = ".SI";

void* wmdf::SWIndexAdapter::FetchARecord()
{
	IndexSnapshot* record = new IndexSnapshot();
	memset(record,0,sizeof(IndexSnapshot));
	dbf_get_string(dbh_,SWZS_WINDCODE_INDEX,record->wind_code,16);
	memcpy(&record->wind_code[6],swzs_suffix_,4);
	record->data_time_stamp = DateTime::get_current_time();
	record->pre_close = dbf_get_num(dbh_,SWZS_PRECLOSE_INDEX);
	record->open_price = dbf_get_num(dbh_,SWZS_OPEN_INDEX);
	record->high_price = dbf_get_num(dbh_,SWZS_HIGH_INDEX);
	record->low_price = dbf_get_num(dbh_,SWZS_LOW_INDEX);
	record->new_price = dbf_get_num(dbh_,SWZS_NEW_INDEX);
	record->total_amount = dbf_get_num(dbh_,SWZS_TOTALAMOUNT_INDEX);
	record->total_volume = dbf_get_num(dbh_,SWZS_TOTALVOLUME_INDEX);
	return record;
}

wmdf::MdfMessage* wmdf::SWIndexAdapter::CreateMDFMessage( int index,int count )
{
	MdfMessage* mdf_message = new MdfMessage();
	mdf_message->set_local_time(DateTime::get_current_time());

	MarketMessageHeader* header = new MarketMessageHeader();
	header->place_holder = 0;
	mdf_message->set_header(header);

	mdf_message->set_product_id(PRODUCT_SWINDEX_LEVEL1_ID);
	mdf_message->set_message_id(SW_INDEXDATA_ID);
	mdf_message->set_major_version(MAJOR_VERSION_SWINDEX_ID);
	mdf_message->set_minor_version(MINOR_VERSION_SWINDEX_ID);
	mdf_message->set_record_count(count);
	mdf_message->set_packet_num(++packet_id_);

	IndexSnapshot* recs = new IndexSnapshot[count];
	mdf_message->set_body(recs);
	for(int i=0;i<count;i++,index++)
	{
		memcpy(&recs[i],records_[index],sizeof(IndexSnapshot));
	}
	return mdf_message;
}




