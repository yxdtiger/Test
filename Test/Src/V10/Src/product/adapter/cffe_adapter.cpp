#include "cffe_adapter.h"
#include "net_module.h"
#include "../../include/wmdf_structs.h"
#include "../../include/date_time.h"
#include "../../protocol/mdf_message.h"
#include "../../environment/splitter.h"
#include "../../environment/utils.h"
#include <cstring>

using namespace wmdf;

CffeL2Adapter::CffeL2Adapter(ProductSource* product_source)
:ProductAdapter(product_source)
{
  splitter_ = new Splitter("10:10:10",":");
}


CffeL2Adapter::~CffeL2Adapter()
{
  if (splitter_ != NULL)
  {
    delete splitter_;
    splitter_=NULL;
  }
}


void CffeL2Adapter::PublishMessages()
{
	CFEL2MarketSnapshot* cfel2 = new CFEL2MarketSnapshot[old_records_.size()];
	FuturesMarketSnapshot* cfel1 = new FuturesMarketSnapshot[old_records_.size()];
	for(uint32_t i=0;i<old_records_.size();i++)
	{
		memset(cfel1[i].wind_code,0,sizeof(cfel1[i].wind_code));
		cfel1[i].trade_date=WMDF_CONSTS_32BIT_NIL_VALUE;
		cfel1[i].data_time_stamp=WMDF_CONSTS_32BIT_NIL_VALUE;
		cfel1[i].new_price=WMDF_CONSTS_DOUBLE_NIL_VALUE;
		cfel1[i].pre_close=WMDF_CONSTS_DOUBLE_NIL_VALUE;
		cfel1[i].open_price=WMDF_CONSTS_DOUBLE_NIL_VALUE;
		cfel1[i].high_price=WMDF_CONSTS_DOUBLE_NIL_VALUE;
		cfel1[i].low_price=WMDF_CONSTS_DOUBLE_NIL_VALUE;
		cfel1[i].total_volume=WMDF_CONSTS_64BIT_NIL_VALUE;
		cfel1[i].bid_price1=WMDF_CONSTS_DOUBLE_NIL_VALUE;
		cfel1[i].ask_price1=WMDF_CONSTS_DOUBLE_NIL_VALUE;
		cfel1[i].ask_size1=WMDF_CONSTS_32BIT_NIL_VALUE;
		cfel1[i].bid_size1=WMDF_CONSTS_32BIT_NIL_VALUE;
		cfel1[i].total_amount=WMDF_CONSTS_DOUBLE_NIL_VALUE;
		cfel1[i].settle=WMDF_CONSTS_DOUBLE_NIL_VALUE;
		cfel1[i].pre_settle=WMDF_CONSTS_DOUBLE_NIL_VALUE;
		cfel1[i].position=WMDF_CONSTS_64BIT_NIL_VALUE;
		cfel1[i].pre_position=WMDF_CONSTS_64BIT_NIL_VALUE;
		cfel1[i].average_price=WMDF_CONSTS_DOUBLE_NIL_VALUE;
		cfel1[i].high_limit=WMDF_CONSTS_DOUBLE_NIL_VALUE;
		cfel1[i].low_limit=WMDF_CONSTS_DOUBLE_NIL_VALUE;
	}


  for(uint32_t i=0;i<old_records_.size();i++)
  {
    memset(cfel2[i].wind_code,0,sizeof(cfel2[i].wind_code));
    cfel2[i].trade_date=WMDF_CONSTS_32BIT_NIL_VALUE;
    cfel2[i].data_time_stamp=WMDF_CONSTS_32BIT_NIL_VALUE;
    cfel2[i].new_price=WMDF_CONSTS_DOUBLE_NIL_VALUE;
    cfel2[i].pre_close=WMDF_CONSTS_DOUBLE_NIL_VALUE;
    cfel2[i].open_price=WMDF_CONSTS_DOUBLE_NIL_VALUE;
    cfel2[i].high_price=WMDF_CONSTS_DOUBLE_NIL_VALUE;
    cfel2[i].low_price=WMDF_CONSTS_DOUBLE_NIL_VALUE;
    cfel2[i].total_volume=WMDF_CONSTS_64BIT_NIL_VALUE;
    cfel2[i].bid_price1=WMDF_CONSTS_DOUBLE_NIL_VALUE;                  // 申买价一				11
    cfel2[i].bid_price2=WMDF_CONSTS_DOUBLE_NIL_VALUE;                  // 申买价二				12
    cfel2[i].bid_price3=WMDF_CONSTS_DOUBLE_NIL_VALUE;                  // 申买价三				13
    cfel2[i].bid_price4=WMDF_CONSTS_DOUBLE_NIL_VALUE;                  // 申买价四				14
    cfel2[i].bid_price5=WMDF_CONSTS_DOUBLE_NIL_VALUE;                  // 申买价五				15
    cfel2[i].ask_price1=WMDF_CONSTS_DOUBLE_NIL_VALUE;                  // 申卖价一				21
    cfel2[i].ask_price2=WMDF_CONSTS_DOUBLE_NIL_VALUE;                  // 申卖价二				22
    cfel2[i].ask_price3=WMDF_CONSTS_DOUBLE_NIL_VALUE;                  // 申卖价三				23
    cfel2[i].ask_price4=WMDF_CONSTS_DOUBLE_NIL_VALUE;                  // 申卖价四				24
    cfel2[i].ask_price5=WMDF_CONSTS_DOUBLE_NIL_VALUE;                  // 申卖价五				25
    cfel2[i].ask_size1=WMDF_CONSTS_32BIT_NIL_VALUE;                // 申卖量一				41
    cfel2[i].ask_size2=WMDF_CONSTS_32BIT_NIL_VALUE;                // 申卖量二				42
    cfel2[i].ask_size3=WMDF_CONSTS_32BIT_NIL_VALUE;                // 申卖量三				43
    cfel2[i].ask_size4=WMDF_CONSTS_32BIT_NIL_VALUE;                // 申买量四				34
    cfel2[i].ask_size5=WMDF_CONSTS_32BIT_NIL_VALUE;                // 申卖量五				45
    cfel2[i].bid_size1=WMDF_CONSTS_32BIT_NIL_VALUE;                // 申买量一				31
    cfel2[i].bid_size2=WMDF_CONSTS_32BIT_NIL_VALUE;                // 申买量二				32
    cfel2[i].bid_size3=WMDF_CONSTS_32BIT_NIL_VALUE;                // 申买量三				33
    cfel2[i].bid_size4=WMDF_CONSTS_32BIT_NIL_VALUE;                // 申卖量四				44
    cfel2[i].bid_size5=WMDF_CONSTS_32BIT_NIL_VALUE;                // 申买量五				35
    
    cfel2[i].total_amount=WMDF_CONSTS_DOUBLE_NIL_VALUE;
    cfel2[i].pre_settle=WMDF_CONSTS_DOUBLE_NIL_VALUE;
    cfel2[i].position=WMDF_CONSTS_64BIT_NIL_VALUE;
    cfel2[i].pre_position=WMDF_CONSTS_64BIT_NIL_VALUE;
    cfel2[i].high_limit=WMDF_CONSTS_DOUBLE_NIL_VALUE;
    cfel2[i].low_limit=WMDF_CONSTS_DOUBLE_NIL_VALUE;
		cfel2[i].settle = WMDF_CONSTS_DOUBLE_NIL_VALUE;
  }




	static const char *cfe_suffix = ".CFE";

	for(uint32_t i=0;i<old_records_.size();i++)
	{
		memcpy(cfel2[i].wind_code,old_records_[i]->InstrumentID,sizeof(cfel2[i].wind_code));
		memcpy(&cfel2[i].wind_code[6],cfe_suffix,5);

		memcpy(cfel1[i].wind_code,old_records_[i]->InstrumentID,sizeof(cfel1[i].wind_code));
		memcpy(&cfel1[i].wind_code[6],cfe_suffix,5);

		cfel2[i].new_price = old_records_[i]->LastPrice;
		cfel1[i].new_price = old_records_[i]->LastPrice;

		cfel2[i].pre_close = old_records_[i]->PreClosePrice;
		cfel1[i].pre_close = old_records_[i]->PreClosePrice;

		cfel2[i].open_price = old_records_[i]->OpenPrice;
		cfel1[i].open_price = old_records_[i]->OpenPrice;

		cfel2[i].high_price = old_records_[i]->HighestPrice;
		cfel1[i].high_price = old_records_[i]->HighestPrice;

		cfel2[i].low_price = old_records_[i]->LowestPrice;
		cfel1[i].low_price = old_records_[i]->LowestPrice;

		cfel2[i].total_volume=(int64_t)old_records_[i]->Volume;
		cfel1[i].total_volume=(int64_t)old_records_[i]->Volume;

		cfel2[i].bid_price1=old_records_[i]->BidPrice1;
		cfel2[i].bid_price2=old_records_[i]->BidPrice2;
		cfel2[i].bid_price3=old_records_[i]->BidPrice3;
		cfel2[i].bid_price4=old_records_[i]->BidPrice4;
		cfel2[i].bid_price5=old_records_[i]->BidPrice5;
		cfel2[i].ask_price1=old_records_[i]->AskPrice1;
		cfel2[i].ask_price2=old_records_[i]->AskPrice2;
		cfel2[i].ask_price3=old_records_[i]->AskPrice3;
		cfel2[i].ask_price4=old_records_[i]->AskPrice4;
		cfel2[i].ask_price5=old_records_[i]->AskPrice5;
		cfel2[i].bid_size1=(int32_t)old_records_[i]->BidVolume1;
		cfel2[i].bid_size2=(int32_t)old_records_[i]->BidVolume2;
		cfel2[i].bid_size3=(int32_t)old_records_[i]->BidVolume3;
		cfel2[i].bid_size4=(int32_t)old_records_[i]->BidVolume4;
		cfel2[i].bid_size5=(int32_t)old_records_[i]->BidVolume5;
		cfel2[i].ask_size1=(int32_t)old_records_[i]->AskVolume1;
		cfel2[i].ask_size2=(int32_t)old_records_[i]->AskVolume2;
		cfel2[i].ask_size3=(int32_t)old_records_[i]->AskVolume3;
		cfel2[i].ask_size4=(int32_t)old_records_[i]->AskVolume4;
		cfel2[i].ask_size5=(int32_t)old_records_[i]->AskVolume5;

		cfel1[i].bid_price1=old_records_[i]->BidPrice1;
		cfel1[i].ask_price1=old_records_[i]->AskPrice1;
		cfel1[i].ask_size1=(int32_t)old_records_[i]->AskVolume1;
		cfel1[i].bid_size1=(int32_t)old_records_[i]->BidVolume1;


		cfel2[i].total_amount=old_records_[i]->Turnover;
		cfel2[i].pre_settle=old_records_[i]->PreSettlementPrice;
		cfel2[i].position=(int64_t)old_records_[i]->OpenInterest;
		cfel2[i].pre_position=(int64_t)old_records_[i]->PreOpenInterest;
		cfel2[i].high_limit=old_records_[i]->UpperLimitPrice;
		cfel2[i].low_limit=old_records_[i]->LowerLimitPrice;
		if(old_records_[i]->SettlementPrice!=0)
			cfel2[i].settle = old_records_[i]->SettlementPrice;

    cfel1[i].total_amount=old_records_[i]->Turnover;
		cfel1[i].pre_settle = old_records_[i]->PreSettlementPrice;
		cfel1[i].position = (int64_t)old_records_[i]->OpenInterest;
		cfel1[i].pre_position = (int64_t)old_records_[i]->PreOpenInterest;
		cfel1[i].high_limit = old_records_[i]->UpperLimitPrice;
		cfel1[i].low_limit=old_records_[i]->LowerLimitPrice;
		if(old_records_[i]->SettlementPrice!=0)
			cfel1[i].settle = old_records_[i]->SettlementPrice;

		int ymd=atoi(old_records_[i]->TradingDay);
		cfel2[i].trade_date=DateTime::ymd2date(ymd);
		cfel1[i].trade_date=DateTime::ymd2date(ymd);

		std::string time_str(old_records_[i]->UpdateTime);
		splitter_->reset(time_str,":");
		assert(splitter_->size()>=3);
		int hour=atoi((*splitter_)[0].c_str());
		int minute=atoi((*splitter_)[1].c_str());
		int second=atoi((*splitter_)[2].c_str());
		cfel2[i].data_time_stamp=DateTime::hms2second(hour,minute,second)*1000;
		cfel1[i].data_time_stamp=cfel2[i].data_time_stamp;
	}

	MdfMessage* mdf_msg=new MdfMessage();
	mdf_msg->set_local_time(DateTime::get_current_time());
	mdf_msg->set_socket_id(0);
	mdf_msg->set_product_id(PRODUCT_CFFE_LEVEL2_ID);
	mdf_msg->set_message_id(CFFEL2_MARKETDATA_ID);
	mdf_msg->set_major_version(MAJOR_VERSION_CFFE_LEVEL2_ID);
	mdf_msg->set_minor_version(MINOR_VERSION_CFFE_LEVEL2_ID);
	MarketMessageHeader* header = new MarketMessageHeader();
	header->place_holder = 0;
	mdf_msg->set_packet_num(packet_id_+1);
	mdf_msg->set_header(header);
	mdf_msg->set_record_count(old_records_.size());
	mdf_msg->set_body(cfel2);

	//Utils::PrintMDFMessageData(mdf_msg,fout2);
	PublishMdfMessage(mdf_msg);


	MdfMessage* mdf_msg2=new MdfMessage();
	mdf_msg2->set_local_time(DateTime::get_current_time());
	mdf_msg2->set_socket_id(0);
	mdf_msg2->set_product_id(PRODUCT_CFFE_LEVEL1_ID);
	mdf_msg2->set_message_id(CFFEL1_MARKETDATA_ID);
	mdf_msg2->set_major_version(MAJOR_VERSION_CFFE_LEVEL1_ID);
	mdf_msg2->set_minor_version(MINOR_VERSION_CFFE_LEVEL1_ID);
	MarketMessageHeader* header2 = new MarketMessageHeader();
	header2->place_holder = 0;
	mdf_msg2->set_packet_num(packet_id_+1);
	mdf_msg2->set_header(header2);
	mdf_msg2->set_record_count(old_records_.size());
	mdf_msg2->set_body(cfel1);

	packet_id_++;

	//Utils::PrintMDFMessageData(mdf_msg,fout2);
	PublishMdfMessage(mdf_msg2);
}

uint32_t wmdf::CffeL2Adapter::ParseMessage( const uint32_t length )
{
  uint32_t size_of_old_struct=sizeof(Cffe_L2_Market_Struct);
  uint32_t pos = 0;
  uint32_t end_pos = 0;
  while(pos+size_of_old_struct <= length)
  {
    if(message_buffer_[pos]=='I'&&message_buffer_[pos+1]=='F')
    {
      Cffe_L2_Market_Struct* record = new Cffe_L2_Market_Struct();
      memset(record,0,size_of_old_struct);
      memcpy(record,(void*)&message_buffer_[pos],size_of_old_struct);
      old_records_.push_back(record);
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

void wmdf::CffeL2Adapter::ClearRecords()
{
  for (uint32_t index=0;index< old_records_.size();index++)
  {
    delete old_records_[index];
    old_records_[index] = NULL;
  }
  old_records_.clear();
}

