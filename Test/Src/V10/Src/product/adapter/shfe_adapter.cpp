#include "shfe_adapter.h"
#include "net_module.h"
#include "../../include/wmdf_structs.h"
#include "../../include/date_time.h"
#include "../../protocol/mdf_message.h"
#include "../../environment/splitter.h"
#include "../../environment/utils.h"
#include <string>
#ifdef __linux__
#include <cstring>
#endif

using namespace wmdf;



ShfeAdapter::ShfeAdapter(ProductSource* product_source)
:ProductAdapter(product_source)
{
  splitter_ = new Splitter("10:10:10",":");
}


ShfeAdapter::~ShfeAdapter()
{
  if (splitter_ != NULL)
  {
    delete splitter_;
    splitter_=NULL;
  }
}

bool ShfeAdapter::IsShfeCode( int32_t i )
{
  if(((message_buffer_[i]>='a'&& message_buffer_[i]<='z')||(message_buffer_[i]>='A'&&message_buffer_[i]<='Z'))
    && ((message_buffer_[i+1]>='a'&&message_buffer_[i+1]<='z')||(message_buffer_[i+1]>='A'&&message_buffer_[i+1]<='Z'))
    &&(message_buffer_[i+2]>='0'&& message_buffer_[i+2]<='9'))
    return true;
  else
    return false;
}

uint32_t wmdf::ShfeAdapter::ParseMessage( const uint32_t length )
{
  uint32_t size_of_old_struct=sizeof(STRUCT_SHFE);
  uint32_t pos = 0;
  uint32_t end_pos=0;
  bool have_packet=false;
  while(pos+size_of_old_struct <= length)
  {
    if(IsShfeCode(pos))
    {
      STRUCT_SHFE* old_shfe=new STRUCT_SHFE();
      memcpy(old_shfe,(void*)&message_buffer_[pos],size_of_old_struct);
      old_records_.push_back(old_shfe);
      pos=pos+size_of_old_struct;
      end_pos = pos;
    }
    else
    {
      pos++;
    }
  }
  if (old_records_.size()>0)
  {
    PublishMessages();
    ClearRecords();
  }
  return end_pos;
}

void wmdf::ShfeAdapter::PublishMessages()
{
  FuturesMarketSnapshot* shfel1=new FuturesMarketSnapshot[old_records_.size()];
  for(uint32_t i=0;i<old_records_.size();i++)
  {
    memset(shfel1[i].wind_code,0,sizeof(shfel1[i].wind_code));
    shfel1[i].trade_date=WMDF_CONSTS_32BIT_NIL_VALUE;
    shfel1[i].data_time_stamp=WMDF_CONSTS_32BIT_NIL_VALUE;
    shfel1[i].new_price=WMDF_CONSTS_DOUBLE_NIL_VALUE;
    shfel1[i].pre_close=WMDF_CONSTS_DOUBLE_NIL_VALUE;
    shfel1[i].open_price=WMDF_CONSTS_DOUBLE_NIL_VALUE;
    shfel1[i].high_price=WMDF_CONSTS_DOUBLE_NIL_VALUE;
    shfel1[i].low_price=WMDF_CONSTS_DOUBLE_NIL_VALUE;
    shfel1[i].total_volume=WMDF_CONSTS_64BIT_NIL_VALUE;
    shfel1[i].bid_price1=WMDF_CONSTS_DOUBLE_NIL_VALUE;
    shfel1[i].ask_price1=WMDF_CONSTS_DOUBLE_NIL_VALUE;
    shfel1[i].ask_size1=WMDF_CONSTS_32BIT_NIL_VALUE;
    shfel1[i].bid_size1=WMDF_CONSTS_32BIT_NIL_VALUE;
    shfel1[i].total_amount=WMDF_CONSTS_DOUBLE_NIL_VALUE;
    shfel1[i].settle=WMDF_CONSTS_DOUBLE_NIL_VALUE;
    shfel1[i].pre_settle=WMDF_CONSTS_DOUBLE_NIL_VALUE;
    shfel1[i].position=WMDF_CONSTS_64BIT_NIL_VALUE;
    shfel1[i].pre_position=WMDF_CONSTS_64BIT_NIL_VALUE;
    shfel1[i].average_price=WMDF_CONSTS_DOUBLE_NIL_VALUE;
    shfel1[i].high_limit=WMDF_CONSTS_DOUBLE_NIL_VALUE;
    shfel1[i].low_limit=WMDF_CONSTS_DOUBLE_NIL_VALUE;
  }

  static const char *shf_suffix = ".SHF";

  for(uint32_t i=0;i<old_records_.size();i++)
  {
    memcpy(shfel1[i].wind_code,old_records_[i]->InstrumentID,sizeof(shfel1[i].wind_code));
    memcpy(&shfel1[i].wind_code[6],shf_suffix,5);
    shfel1[i].wind_code[0] = toupper(shfel1[i].wind_code[0]);
    shfel1[i].wind_code[1] = toupper(shfel1[i].wind_code[1]);

    shfel1[i].new_price=old_records_[i]->LastPrice;
    shfel1[i].pre_close=old_records_[i]->PreClosePrice;
    shfel1[i].open_price=old_records_[i]->OpenPrice;
    shfel1[i].high_price=old_records_[i]->HighestPrice;
    shfel1[i].low_price=old_records_[i]->LowestPrice;
    shfel1[i].total_volume=(int64_t)old_records_[i]->Volume;
    shfel1[i].bid_price1=old_records_[i]->BidPrice1;
    shfel1[i].ask_price1=old_records_[i]->AskPrice1;
    shfel1[i].bid_size1=(uint32_t)old_records_[i]->BidVolume1;
    shfel1[i].ask_size1=(uint32_t)old_records_[i]->AskVolume1;
    shfel1[i].total_amount=old_records_[i]->Turnover;
    shfel1[i].high_limit=old_records_[i]->UpperLimitPrice;
    shfel1[i].low_limit=old_records_[i]->LowerLimitPrice;

    shfel1[i].pre_settle=old_records_[i]->PreSettlementPrice;
		if(old_records_[i]->SettlementPrice == 0)
			shfel1[i].settle = WMDF_CONSTS_DOUBLE_NIL_VALUE;
		else
			shfel1[i].settle=old_records_[i]->SettlementPrice;
    shfel1[i].pre_position=old_records_[i]->PreOpenInterest;
    shfel1[i].position=old_records_[i]->OpenInterest;


    int ymd=atoi(old_records_[i]->TradingDay);
    int year=ymd/10000;
    int month=(ymd-year*10000)/100;
    int day=ymd-year*10000-month*100;
    shfel1[i].trade_date=DateTime::ymd2date(year,month,day);

    std::string time_str(old_records_[i]->UpdateTime);
    splitter_->reset(time_str,":");
    assert(splitter_->size()>=3);
    int hour=atoi((*splitter_)[0].c_str());
    int minute=atoi((*splitter_)[1].c_str());
    int second=atoi((*splitter_)[2].c_str());

    shfel1[i].data_time_stamp=DateTime::hms2second(hour,minute,second)*1000;
  }
  MdfMessage* mdf_msg=new MdfMessage();
  mdf_msg->set_local_time(DateTime::get_current_time());
  mdf_msg->set_socket_id(0);
  mdf_msg->set_product_id(PRODUCT_SHFE_LEVEL1_ID);
  mdf_msg->set_message_id(SHFEL1_MARKETDATA_ID);
  mdf_msg->set_major_version(MAJOR_VERSION_SHFE_LEVEL1_ID);
  mdf_msg->set_minor_version(MINOR_VERSION_SHFE_LEVEL1_ID);

  MarketMessageHeader* header = new MarketMessageHeader();
  header->place_holder = 0;
  mdf_msg->set_packet_num(++packet_id_);
  mdf_msg->set_header(header);
  mdf_msg->set_record_count(old_records_.size());
  mdf_msg->set_body(shfel1);

  PublishMdfMessage(mdf_msg);
}

void wmdf::ShfeAdapter::ClearRecords()
{
  for (uint32_t index=0;index< old_records_.size();index++)
  {
    delete old_records_[index];
    old_records_[index] = NULL;
  }
  old_records_.clear();
}
