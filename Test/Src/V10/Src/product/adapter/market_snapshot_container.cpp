#include "market_snapshot_container.h"
#include "../../protocol/mdf_message.h"
#include "../../include/date_time.h"

using namespace std;


wmdf::MarketSnapshotContainer::MarketSnapshotContainer()
{
	num_of_marketsnapshot_ = 0;
}

wmdf::MarketSnapshotContainer::~MarketSnapshotContainer()
{
	for(map<std::string,L2MarketSnapshot* >::iterator iter = market_snapshots_.begin();iter!=market_snapshots_.end();++iter)
	{
		L2MarketSnapshot *ms = iter->second;
		if(ms!=NULL)
		{
			delete ms;
			ms = NULL;
		}
	}
}

void wmdf::MarketSnapshotContainer::MergeMarketSnapshot( L2MarketSnapshot *marketsnapshot )
{
  boost::mutex::scoped_lock lock(mutex_);
	string windcode = marketsnapshot->wind_code;
	map<string,L2MarketSnapshot* >::iterator iter = market_snapshots_.find(windcode);
	if(iter == market_snapshots_.end())
	{
		L2MarketSnapshot *ms = new L2MarketSnapshot();
		memcpy(ms,marketsnapshot,sizeof(L2MarketSnapshot));
		market_snapshots_.insert(pair<string,L2MarketSnapshot*>(windcode,ms));
		++num_of_marketsnapshot_;
	}
	else
	{
		L2MarketSnapshot *ms = iter->second;
		UpdateValue(ms,marketsnapshot);
	}
}

void wmdf::MarketSnapshotContainer::UpdateValue( L2MarketSnapshot *des, L2MarketSnapshot *src )
{
	if(src->trade_date != WMDF_CONSTS_32BIT_NIL_VALUE)
		des->trade_date = src->trade_date;

	if(src->trade_time != WMDF_CONSTS_32BIT_NIL_VALUE)
		des->trade_time = src->trade_time;

	if(src->new_price == src->new_price)
		des->new_price = src->new_price;

	if(src->prev_close == src->prev_close)
		des->prev_close = src->prev_close;

	if(src->today_open == src->today_open)
		des->today_open = src->today_open;

	if(src->today_high == src->today_high)
		des->today_high = src->today_high;

	if(src->today_low == src->today_low)
		des->today_low = src->today_low;

	if(src->volume != WMDF_CONSTS_64BIT_NIL_VALUE)
		des->volume = src->volume;

	if(src->total_amount == src->total_amount)
		des->volume = src->volume;

	if(src->bid_price1 == src->bid_price1)
		des->bid_price1 = src->bid_price1;

	if(src->bid_price2 == src->bid_price2)
		des->bid_price2 = src->bid_price2;

	if(src->bid_price3 == src->bid_price3)
		des->bid_price3 = src->bid_price3;

	if(src->bid_price4 == src->bid_price4)
		des->bid_price4 = src->bid_price4;

	if(src->bid_price5 == src->bid_price5)
		des->bid_price5 = src->bid_price5;

	if(src->bid_price6 == src->bid_price6)
		des->bid_price6 = src->bid_price6;

	if(src->bid_price7 == src->bid_price7)
		des->bid_price7 = src->bid_price7;

	if(src->bid_price8 == src->bid_price8)
		des->bid_price8 = src->bid_price8;

	if(src->bid_price9 == src->bid_price9)
		des->bid_price9 = src->bid_price9;

	if(src->bid_price10 == src->bid_price10)
		des->bid_price10 = src->bid_price10;

	if(src->ask_price1 == src->ask_price1)
		des->ask_price1 = src->ask_price1;

	if(src->ask_price2 == src->ask_price2)
		des->ask_price2 = src->ask_price2;

	if(src->ask_price3 == src->ask_price3)
		des->ask_price3 = src->ask_price3;

	if(src->ask_price4 == src->ask_price4)
		des->ask_price4 = src->ask_price4;

	if(src->ask_price5 == src->ask_price5)
		des->ask_price5 = src->ask_price5;

	if(src->ask_price6 == src->ask_price6)
		des->ask_price6 = src->ask_price6;

	if(src->ask_price7 == src->ask_price7)
		des->ask_price7 = src->ask_price7;

	if(src->ask_price8 == src->ask_price8)
		des->ask_price8 = src->ask_price8;

	if(src->ask_price9 == src->ask_price9 )
		des->ask_price9 = src->ask_price9;

	if(src->ask_price10 == src->ask_price10)
		des->ask_price10 = src->ask_price10;

	if(src->bid_volume1 != WMDF_CONSTS_32BIT_NIL_VALUE)
		des->bid_volume1 = src->bid_volume1;

	if(src->bid_volume2 != WMDF_CONSTS_32BIT_NIL_VALUE)
		des->bid_volume2 = src->bid_volume2;

	if(src->bid_volume3 != WMDF_CONSTS_32BIT_NIL_VALUE)
		des->bid_volume3 = src->bid_volume3;

	if(src->bid_volume4 != WMDF_CONSTS_32BIT_NIL_VALUE)
		des->bid_volume4 = src->bid_volume4;

	if(src->bid_volume5 != WMDF_CONSTS_32BIT_NIL_VALUE)
		des->bid_volume5 = src->bid_volume5;

	if(src->bid_volume6 != WMDF_CONSTS_32BIT_NIL_VALUE)
		des->bid_volume6 = src->bid_volume6;

	if(src->bid_volume7 != WMDF_CONSTS_32BIT_NIL_VALUE)
		des->bid_volume7 = src->bid_volume7;

	if(src->bid_volume8 != WMDF_CONSTS_32BIT_NIL_VALUE)
		des->bid_volume8 = src->bid_volume8;

	if(src->bid_volume9 != WMDF_CONSTS_32BIT_NIL_VALUE)
		des->bid_volume9 = src->bid_volume9;

	if(src->bid_volume10 != WMDF_CONSTS_32BIT_NIL_VALUE)
		des->bid_volume10 = src->bid_volume10;

	if(src->ask_volume1 != WMDF_CONSTS_32BIT_NIL_VALUE)
		des->ask_volume1 = src->ask_volume1;

	if(src->ask_volume2 != WMDF_CONSTS_32BIT_NIL_VALUE)
		des->ask_volume2 = src->ask_volume2;

	if(src->ask_volume3 != WMDF_CONSTS_32BIT_NIL_VALUE)
		des->ask_volume3 = src->ask_volume3;

	if(src->ask_volume4 != WMDF_CONSTS_32BIT_NIL_VALUE)
		des->ask_volume4 = src->ask_volume4;

	if(src->ask_volume5 != WMDF_CONSTS_32BIT_NIL_VALUE)
		des->ask_volume5 = src->ask_volume5;

	if(src->ask_volume6 != WMDF_CONSTS_32BIT_NIL_VALUE)
		des->ask_volume6 = src->ask_volume6;

	if(src->ask_volume7 != WMDF_CONSTS_32BIT_NIL_VALUE)
		des->ask_volume7 = src->ask_volume7;

	if(src->ask_volume8 != WMDF_CONSTS_32BIT_NIL_VALUE)
		des->ask_volume8 = src->ask_volume8;

	if(src->ask_volume9 != WMDF_CONSTS_32BIT_NIL_VALUE)
		des->ask_volume9 = src->ask_volume9;

	if(src->ask_volume10 != WMDF_CONSTS_32BIT_NIL_VALUE)
		des->ask_volume10 = src->ask_volume10;

	if(src->transactions_count != WMDF_CONSTS_64BIT_NIL_VALUE)
		des->transactions_count = src->transactions_count;

	if(src->weighted_average_best_bid == src->weighted_average_best_bid)
		des->weighted_average_best_bid = src->weighted_average_best_bid;

	if(src->weighted_average_best_ask == src->weighted_average_best_ask)
		des->weighted_average_best_ask = src->weighted_average_best_ask;

// 	if(src->iopv == src->iopv)
// 		des->iopv = src->iopv;
// 
// 	if(src->ytm == src->ytm)
// 		des->ytm = src->ytm;
	
	if(src->l2_total_bid_qty != WMDF_CONSTS_64BIT_NIL_VALUE)
		des->l2_total_bid_qty = src->l2_total_bid_qty;

	if(src->l2_total_offer_qty != WMDF_CONSTS_64BIT_NIL_VALUE)
		des->l2_total_offer_qty = src->l2_total_offer_qty;

}

L2MarketSnapshot* wmdf::MarketSnapshotContainer::get_marketsnapshot( const std::string& windcode )
{
  boost::mutex::scoped_lock lock(mutex_);
	map<std::string,L2MarketSnapshot* >::iterator iter = market_snapshots_.find(windcode);
	if(iter != market_snapshots_.end())
  {
			return iter->second;
  }
  return NULL;
}

std::vector<wmdf::MdfMessage*>* wmdf::MarketSnapshotContainer::GetMdfMessage()
{
  boost::mutex::scoped_lock lock(mutex_);
  uint32_t record_count = 100;
  uint32_t msg_count = market_snapshots_.size() / record_count;
  if (market_snapshots_.size() % record_count > 0) 
  {
    msg_count = msg_count + 1;
  }
  std::vector<MdfMessage*>* mdf_messages = new std::vector<MdfMessage*>;

  size_t size_of_snapshot = sizeof(L2MarketSnapshot);

  uint32_t snap_index = 0;
  uint32_t msg_body_index = 0;
  MdfMessage* msg = NULL;
  map<string,L2MarketSnapshot*>::iterator iter = market_snapshots_.begin();
  while (iter != market_snapshots_.end())
  {
    //是RecordCount的整数倍的时候创建mdfmessage
    if (snap_index%record_count == 0)
    {
      if ((market_snapshots_.size()-snap_index) >= record_count)
      {
        msg = CreateMdfMessage(record_count);
        msg_body_index = 0;
      }
      else
      {
        msg = CreateMdfMessage(market_snapshots_.size()-snap_index);
        msg_body_index = 0;
      }
    }
    L2MarketSnapshot *msg_body = (L2MarketSnapshot*)msg->body();
    memcpy(msg_body+snap_index,iter->second,size_of_snapshot);
    snap_index++;
    msg_body_index ++;
  }
  return mdf_messages;
}

wmdf::MdfMessage* wmdf::MarketSnapshotContainer::CreateMdfMessage( int32_t record_count )
{
  MdfMessage* mdf_msg = new MdfMessage();
  mdf_msg->set_local_time(DateTime::get_current_time());
  mdf_msg->set_socket_id(0);
  mdf_msg->set_product_id(PRODUCT_SSE_LEVEL2_ID);
  mdf_msg->set_message_id(SSEL2_MARKETDATA_ID);
  mdf_msg->set_major_version(MAJOR_VERSION_SSE_LEVEL2_ID);
  mdf_msg->set_minor_version(MINOR_VERSION_SSE_LEVEL2_ID);
  mdf_msg->set_record_count(record_count);
  MarketMessageHeader* head_of_marketsnapshot = new MarketMessageHeader();
  head_of_marketsnapshot->place_holder = 0;
  mdf_msg->set_packet_num(0);
  mdf_msg->set_header(head_of_marketsnapshot);
  L2MarketSnapshot* msg_body = new L2MarketSnapshot[record_count];
  mdf_msg->set_body(msg_body);
  return mdf_msg;
}