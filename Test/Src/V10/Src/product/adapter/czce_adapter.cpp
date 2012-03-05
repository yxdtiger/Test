#include "czce_adapter.h"
#include "net_module.h"
#include "../../include/wmdf_structs.h"
#include "../../include/date_time.h"
#include "../../protocol/mdf_message.h"
#include "../../environment/utils.h"
#include <cstring>
// #include <fstream>
using namespace wmdf;

//std::ofstream fout3("test_czce");


CzceAdapter::CzceAdapter(ProductSource* product_source)
:ProductAdapter(product_source)
{

}


CzceAdapter::~CzceAdapter()
{

}

uint32_t wmdf::CzceAdapter::ParseMessage( const uint32_t length )
{
  uint32_t size_of_old_struct=sizeof(STRUCT_CZC);
  uint32_t pos = 0;
  uint32_t end_pos = 0;
  while(pos+size_of_old_struct <= length)
  {
    if(message_buffer_[pos]==0 &&message_buffer_[pos+1]==4 && message_buffer_[pos+2]==1)
    {
      STRUCT_CZC* old_czce=new STRUCT_CZC();
      memcpy(old_czce,(void*)&message_buffer_[pos],size_of_old_struct);
      old_records_.push_back(old_czce);
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

void wmdf::CzceAdapter::PublishMessages()
{
  FuturesMarketSnapshot* czcel1=new FuturesMarketSnapshot[old_records_.size()];
  for(uint32_t i=0;i<old_records_.size();i++)
  {
    memset(czcel1[i].wind_code,0,sizeof(czcel1[i].wind_code));
    czcel1[i].trade_date=WMDF_CONSTS_32BIT_NIL_VALUE;
    czcel1[i].data_time_stamp=WMDF_CONSTS_32BIT_NIL_VALUE;
    czcel1[i].new_price=WMDF_CONSTS_DOUBLE_NIL_VALUE;
    czcel1[i].pre_close=WMDF_CONSTS_DOUBLE_NIL_VALUE;
    czcel1[i].open_price=WMDF_CONSTS_DOUBLE_NIL_VALUE;
    czcel1[i].high_price=WMDF_CONSTS_DOUBLE_NIL_VALUE;
    czcel1[i].low_price=WMDF_CONSTS_DOUBLE_NIL_VALUE;
    czcel1[i].total_volume=WMDF_CONSTS_64BIT_NIL_VALUE;
    czcel1[i].bid_price1=WMDF_CONSTS_DOUBLE_NIL_VALUE;
    czcel1[i].ask_price1=WMDF_CONSTS_DOUBLE_NIL_VALUE;
    czcel1[i].ask_size1=WMDF_CONSTS_32BIT_NIL_VALUE;
    czcel1[i].bid_size1=WMDF_CONSTS_32BIT_NIL_VALUE;
    czcel1[i].total_amount=WMDF_CONSTS_DOUBLE_NIL_VALUE;
    czcel1[i].settle=WMDF_CONSTS_DOUBLE_NIL_VALUE;
    czcel1[i].pre_settle=WMDF_CONSTS_DOUBLE_NIL_VALUE;
    czcel1[i].position=WMDF_CONSTS_64BIT_NIL_VALUE;
    czcel1[i].pre_position=WMDF_CONSTS_64BIT_NIL_VALUE;
    czcel1[i].average_price=WMDF_CONSTS_DOUBLE_NIL_VALUE;
    czcel1[i].high_limit=WMDF_CONSTS_DOUBLE_NIL_VALUE;
    czcel1[i].low_limit=WMDF_CONSTS_DOUBLE_NIL_VALUE;
  }

  static const char *czc_suffix = ".CZC";
  for(uint32_t i=0;i<old_records_.size();i++)
  {
    memcpy(czcel1[i].wind_code,old_records_[i]->InstrumentId,sizeof(czcel1[i].wind_code));
    memcpy(&czcel1[i].wind_code[5],czc_suffix,5);
    czcel1[i].new_price=(double)atoi(old_records_[i]->LastPrice);
    czcel1[i].pre_close=(double)atoi(old_records_[i]->PreClose);
    czcel1[i].open_price=(double)atoi(old_records_[i]->OpenPrice);
    czcel1[i].high_price=(double)atoi(old_records_[i]->HighPrice);
    czcel1[i].low_price=(double)atoi(old_records_[i]->LowPrice);
    czcel1[i].total_volume=(int64_t)atoi(old_records_[i]->TotalVolume);
    czcel1[i].bid_price1=(double)atoi(old_records_[i]->BidPrice);
    czcel1[i].ask_price1=(double)atoi(old_records_[i]->AskPrice);
    czcel1[i].bid_size1=(uint32_t)atoi(old_records_[i]->BidLot);
    czcel1[i].ask_size1=(uint32_t)atoi(old_records_[i]->AskLot);
    czcel1[i].settle=(double)atoi(old_records_[i]->SettlePrice);
    czcel1[i].pre_settle=(double)atoi(old_records_[i]->PreSettle);
    czcel1[i].position=(uint64_t)atoi(old_records_[i]->OpenInterest);
    czcel1[i].average_price=(double)atoi(old_records_[i]->AveragePrice);
    czcel1[i].high_limit=(double)atoi(old_records_[i]->HighLimit);
    czcel1[i].low_limit=(double)atoi(old_records_[i]->LowLimit);
    czcel1[i].trade_date=DateTime::today();
    czcel1[i].data_time_stamp=DateTime::get_current_time();
  }


  MdfMessage* mdf_msg=new MdfMessage();
  mdf_msg->set_local_time(DateTime::get_current_time());
  mdf_msg->set_socket_id(0);
  mdf_msg->set_product_id(PRODUCT_CZCE_LEVEL1_ID);
  mdf_msg->set_message_id(CZCEL1_MARKETDATA_ID);
  mdf_msg->set_major_version(MAJOR_VERSION_CZCE_LEVEL1_ID);
  mdf_msg->set_minor_version(MINOR_VERSION_CZCE_LEVEL1_ID);

  MarketMessageHeader* header = new MarketMessageHeader();
  header->place_holder = 0;
  mdf_msg->set_packet_num(++packet_id_);
  mdf_msg->set_header(header);
  mdf_msg->set_record_count(old_records_.size());
  mdf_msg->set_body(czcel1);

  //Utils::PrintMDFMessageData(mdf_msg,fout3);
  PublishMdfMessage(mdf_msg);

}

void wmdf::CzceAdapter::ClearRecords()
{
  for (uint32_t index=0;index< old_records_.size();index++)
  {
    delete old_records_[index];
    old_records_[index] = NULL;
  }
  old_records_.clear();
}
