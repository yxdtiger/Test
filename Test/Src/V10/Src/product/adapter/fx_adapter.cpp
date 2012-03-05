#include "fx_adapter.h"
#include "net_module.h"
#include "../../include/wmdf_structs.h"
#include "../../include/date_time.h"
#include "../../protocol/mdf_message.h"
#ifdef __linux__
#include <cstring>
#endif
using namespace wmdf;


FxAdapter::FxAdapter(ProductSource* product_source)
:ProductAdapter(product_source)
{

}


FxAdapter::~FxAdapter()
{

}

uint32_t wmdf::FxAdapter::ParseMessage( const uint32_t length )
{
  uint32_t size_of_old_struct=sizeof(STRUCT_FX);
  uint32_t pos = 0;
  uint32_t end_pos = 0;
  while(pos+size_of_old_struct <= length)
  {
    if(message_buffer_[pos]==PREFIX && message_buffer_[pos+size_of_old_struct-1]==SUFFIX)
    {
      STRUCT_FX* old_fx=new STRUCT_FX();
      memcpy(old_fx,(void*)&message_buffer_[pos],size_of_old_struct);
      old_records_.push_back(old_fx);
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

void wmdf::FxAdapter::PublishMessages()
{
  FXMarketSnapshot* fx=new FXMarketSnapshot[old_records_.size()];
  for(size_t i = 0; i!=old_records_.size(); ++i)
  {
    memset(fx[i].wind_code,0,sizeof(fx[i].wind_code));
    fx[i].trade_date = WMDF_CONSTS_32BIT_NIL_VALUE;
    fx[i].data_time_stamp = WMDF_CONSTS_32BIT_NIL_VALUE;
    fx[i].new_price = WMDF_CONSTS_DOUBLE_NIL_VALUE;
    fx[i].pre_close = WMDF_CONSTS_DOUBLE_NIL_VALUE;
    fx[i].open_price = WMDF_CONSTS_DOUBLE_NIL_VALUE;
    fx[i].high_price = WMDF_CONSTS_DOUBLE_NIL_VALUE;
    fx[i].low_price = WMDF_CONSTS_DOUBLE_NIL_VALUE;
    fx[i].bid_price1 = WMDF_CONSTS_DOUBLE_NIL_VALUE;
    fx[i].ask_price1 = WMDF_CONSTS_DOUBLE_NIL_VALUE;
  }


  for(uint32_t i=0;i<old_records_.size();i++)
  {
    memcpy(fx[i].wind_code,old_records_[i]->szWindCode,sizeof(old_records_[i]->szWindCode));
    fx[i].new_price=(double)old_records_[i]->fNewPrice;
    fx[i].pre_close=(double)old_records_[i]->fLastClose;
    fx[i].open_price=(double)old_records_[i]->fOpen;
    fx[i].high_price=(double)old_records_[i]->fHigh;
    fx[i].low_price=(double)old_records_[i]->fLow;
    fx[i].bid_price1=(double)old_records_[i]->f1;
    fx[i].ask_price1=(double)old_records_[i]->f2;

    int ymd=old_records_[i]->nDate;
    int year=2000+ymd/100000000;
    int month=(ymd-(year-2000)*100000000)/1000000;
    int day=(ymd-(year-2000)*100000000-month*1000000)/10000;
    fx[i].trade_date=DateTime::ymd2date(year,month,day);

    int hour=(ymd-(year-2000)*100000000-month*1000000-day*10000)/100;
    int minute=ymd-(year-2000)*100000000-month*1000000-day*10000-hour*100;
    int second=0;
    fx[i].data_time_stamp=DateTime::hms2second(hour,minute,second)*1000;
  }

  MdfMessage* mdf_msg=new MdfMessage();
  mdf_msg->set_local_time(DateTime::get_current_time());
  mdf_msg->set_socket_id(0);
  mdf_msg->set_product_id(PRODUCT_FX_ID);
  mdf_msg->set_message_id(FX_MARKETDATA_ID);
  mdf_msg->set_major_version(MAJOR_VERSION_FX_ID);
  mdf_msg->set_minor_version(MINOR_VERSION_FX_ID);

  MarketMessageHeader* header = new MarketMessageHeader();
  header->place_holder = 0;
  mdf_msg->set_packet_num(++packet_id_);
  mdf_msg->set_header(header);
  mdf_msg->set_record_count(old_records_.size());
  mdf_msg->set_body(fx);

  PublishMdfMessage(mdf_msg);

}

void wmdf::FxAdapter::ClearRecords()
{
  for (uint32_t index=0;index< old_records_.size();index++)
  {
    delete old_records_[index];
    old_records_[index] = NULL;
  }
  old_records_.clear();
}
