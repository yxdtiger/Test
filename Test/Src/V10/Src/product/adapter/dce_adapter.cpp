#include "dce_adapter.h"
#include "net_module.h"
#include "../../include/wmdf_structs.h"
#include "../../include/date_time.h"
#include "../../protocol/mdf_message.h"
#include "../../environment/splitter.h"
#include "../../environment/utils.h"
#ifdef __linux__
#include <cstring>
#endif
using namespace wmdf;








DceAdapter::DceAdapter(ProductSource* product_source)
:ProductAdapter(product_source)
{
  splitter_ = new Splitter("10:10:10",":");
}


DceAdapter::~DceAdapter()
{
  if (splitter_ != NULL)
  {
    delete splitter_;
    splitter_=NULL;
  }
}

bool DceAdapter::isDceCode( uint32_t pos )
{
  if(((message_buffer_[pos]>='a'&&message_buffer_[pos]<='z')||(message_buffer_[pos]>='A'&&message_buffer_[pos]<='Z'))
    && (message_buffer_[pos+1]>='0'&&message_buffer_[pos+1]<='9')
    &&(message_buffer_[pos+2]>='0'&&message_buffer_[pos+2]<='9'))
    return true;
  else
    return false;
}

void wmdf::DceAdapter::ClearRecords()
{
  for (uint32_t index=0;index< old_records_.size();index++)
  {
    delete old_records_[index];
    old_records_[index] = NULL;
  }
  old_records_.clear();
}

void wmdf::DceAdapter::PublishMessages()
{
  FuturesMarketSnapshot* dcel1=new FuturesMarketSnapshot[old_records_.size()];
  for(uint32_t i=0;i<old_records_.size();i++)
  {
    memset(dcel1[i].wind_code,0,sizeof(dcel1[i].wind_code));
    dcel1[i].trade_date=WMDF_CONSTS_32BIT_NIL_VALUE;
    dcel1[i].data_time_stamp=WMDF_CONSTS_32BIT_NIL_VALUE;
    dcel1[i].new_price=WMDF_CONSTS_DOUBLE_NIL_VALUE;
    dcel1[i].pre_close=WMDF_CONSTS_DOUBLE_NIL_VALUE;
    dcel1[i].open_price=WMDF_CONSTS_DOUBLE_NIL_VALUE;
    dcel1[i].high_price=WMDF_CONSTS_DOUBLE_NIL_VALUE;
    dcel1[i].low_price=WMDF_CONSTS_DOUBLE_NIL_VALUE;
    dcel1[i].total_volume=WMDF_CONSTS_64BIT_NIL_VALUE;
    dcel1[i].bid_price1=WMDF_CONSTS_DOUBLE_NIL_VALUE;
    dcel1[i].ask_price1=WMDF_CONSTS_DOUBLE_NIL_VALUE;
    dcel1[i].ask_size1=WMDF_CONSTS_32BIT_NIL_VALUE;
    dcel1[i].bid_size1=WMDF_CONSTS_32BIT_NIL_VALUE;
    dcel1[i].total_amount=WMDF_CONSTS_DOUBLE_NIL_VALUE;
    dcel1[i].settle=WMDF_CONSTS_DOUBLE_NIL_VALUE;
    dcel1[i].pre_settle=WMDF_CONSTS_DOUBLE_NIL_VALUE;
    dcel1[i].position=WMDF_CONSTS_64BIT_NIL_VALUE;
    dcel1[i].pre_position=WMDF_CONSTS_64BIT_NIL_VALUE;
    dcel1[i].average_price=WMDF_CONSTS_DOUBLE_NIL_VALUE;
    dcel1[i].high_limit=WMDF_CONSTS_DOUBLE_NIL_VALUE;
    dcel1[i].low_limit=WMDF_CONSTS_DOUBLE_NIL_VALUE;
  }

  static const char *dce_suffix = ".DCE";

  for(uint32_t i=0;i<old_records_.size();i++)
  {
    memcpy(dcel1[i].wind_code,old_records_[i]->InstrumentID,sizeof(dcel1[i].wind_code));
    memcpy(&dcel1[i].wind_code[5],dce_suffix,5);
    dcel1[i].wind_code[0] = toupper(dcel1[i].wind_code[0]);

    if(old_records_[i]->LastPrice > WMDF_CONSTS_MAX_FILTER_VALUE)
      dcel1[i].new_price = WMDF_CONSTS_DOUBLE_NIL_VALUE;
    else
      dcel1[i].new_price = old_records_[i]->LastPrice;

    if(old_records_[i]->PreClosePrice > WMDF_CONSTS_MAX_FILTER_VALUE)
      dcel1[i].pre_close = WMDF_CONSTS_DOUBLE_NIL_VALUE;
    else
      dcel1[i].pre_close = old_records_[i]->PreClosePrice;

    if(old_records_[i]->OpenPrice > WMDF_CONSTS_MAX_FILTER_VALUE)
      dcel1[i].open_price = WMDF_CONSTS_DOUBLE_NIL_VALUE;
    else
      dcel1[i].open_price = old_records_[i]->OpenPrice;

    if(old_records_[i]->HighestPrice > WMDF_CONSTS_MAX_FILTER_VALUE)
      dcel1[i].high_price = WMDF_CONSTS_DOUBLE_NIL_VALUE;
    else
      dcel1[i].high_price = old_records_[i]->HighestPrice;

    if(old_records_[i]->LowestPrice > WMDF_CONSTS_MAX_FILTER_VALUE)
      dcel1[i].low_price = WMDF_CONSTS_DOUBLE_NIL_VALUE;
    else
      dcel1[i].low_price = old_records_[i]->LowestPrice;

    dcel1[i].total_volume=(int64_t)old_records_[i]->Volume;
    dcel1[i].bid_price1=old_records_[i]->BidPrice1;
    dcel1[i].ask_price1=old_records_[i]->AskPrice1;
    dcel1[i].bid_size1=(uint32_t)old_records_[i]->BidVolume1;
    dcel1[i].ask_size1=(uint32_t)old_records_[i]->AskVolume1;
    dcel1[i].position = old_records_[i]->OpenInterest;
    dcel1[i].pre_position = old_records_[i]->PreOpenInterest;

    if(old_records_[i]->PreSettlementPrice > WMDF_CONSTS_MAX_FILTER_VALUE)
      dcel1[i].pre_settle = WMDF_CONSTS_DOUBLE_NIL_VALUE;
    else
      dcel1[i].pre_settle = old_records_[i]->PreSettlementPrice;

    if(old_records_[i]->SettlementPrice > WMDF_CONSTS_MAX_FILTER_VALUE)
      dcel1[i].settle = WMDF_CONSTS_DOUBLE_NIL_VALUE;
    else
      dcel1[i].settle = old_records_[i]->SettlementPrice;

    dcel1[i].total_amount=old_records_[i]->Turnover;
    dcel1[i].high_limit=old_records_[i]->UpperLimitPrice;
    dcel1[i].low_limit=old_records_[i]->LowerLimitPrice;

    int ymd=atoi(old_records_[i]->TradingDay);
    int year=ymd/10000;
    int month=(ymd-year*10000)/100;
    int day=ymd-year*10000-month*100;
    dcel1[i].trade_date=DateTime::ymd2date(year,month,day);

    std::string time_str(old_records_[i]->UpdateTime);
    splitter_->reset(time_str,":");
    assert(splitter_->size()>=3);
    int hour=atoi((*splitter_)[0].c_str());
    int minute=atoi((*splitter_)[1].c_str());
    int second=atoi((*splitter_)[2].c_str());
    dcel1[i].data_time_stamp=DateTime::hms2second(hour,minute,second)*1000;
  }

  MdfMessage* mdf_msg=new MdfMessage();
  mdf_msg->set_local_time(DateTime::get_current_time());
  mdf_msg->set_socket_id(0);
  mdf_msg->set_product_id(PRODUCT_DCE_LEVEL1_ID);
  mdf_msg->set_message_id(DCEL1_MARKETDATA_ID);
  mdf_msg->set_major_version(MAJOR_VERSION_DCE_LEVEL1_ID);
  mdf_msg->set_minor_version(MINOR_VERSION_DCE_LEVEL1_ID);

  MarketMessageHeader* header = new MarketMessageHeader();
  header->place_holder = 0;
  mdf_msg->set_packet_num(++packet_id_);
  mdf_msg->set_header(header);
  mdf_msg->set_record_count(old_records_.size());
  mdf_msg->set_body(dcel1);

  PublishMdfMessage(mdf_msg);
}

uint32_t wmdf::DceAdapter::ParseMessage( const uint32_t length )
{
  uint32_t size_of_old_struct=sizeof(STRUCT_DCE);
  uint32_t pos = 0;
  uint32_t end_pos = 0;
  while(pos+size_of_old_struct <= length)
  {
    if(isDceCode(pos))
    {
      STRUCT_DCE* old_dce=new STRUCT_DCE();
      memcpy(old_dce,(void*)&message_buffer_[pos],size_of_old_struct);
      old_records_.push_back(old_dce);
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
