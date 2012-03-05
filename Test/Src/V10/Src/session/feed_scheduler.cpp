#include "feed_scheduler.h"
#include "../include/date_time.h"
#include "../environment/server_environment.h"
#include "../session/feed_server.h"
#include "../environment/system_config.h"
#include "../environment/utils.h"
#include "../session/subscriber.h"
#include "../session/message_publisher.h"
#include "../session/subscriber_manager.h"
#include "../monitor/logger.h"
#include <boost/bind.hpp>

using namespace std;

wmdf::FeedScheduler::FeedScheduler(void)
{
	isworking_ = false;
}

wmdf::FeedScheduler::~FeedScheduler(void)
{
  Shutdown();
}

void wmdf::FeedScheduler::Schedule()
{
  while(isworking_)
  {
    //检查每日数据行为
    CheckDailyOperations();

    //检查是否有需要踢掉的订阅者（连接）
    CheckSubscriber();

    Utils::SleepTime(10); // wait 10ms
  }
}

void wmdf::FeedScheduler::Startup()
{
  if (!isworking_)
  {
    isworking_ = true;
		//0点重置
		DOTime* dotime = new DOTime(DateTime::today(),ServerEnvironment::system_config()->server_reset_operation_hour());
		operations_.insert(make_pair(DO_RESET,dotime));
		//6点预开盘
		dotime = new DOTime(DateTime::today(),ServerEnvironment::system_config()->pre_market_operation_hour());
		operations_.insert(make_pair(DO_PREMARKETOPEN,dotime));
		//收盘
		dotime = new DOTime(DateTime::today(),ServerEnvironment::system_config()->after_market_operation_hour());
		operations_.insert(make_pair(DO_MARKETCLOSE,dotime));
    work_thread_ = new boost::thread(boost::bind(&FeedScheduler::Schedule,this));
  }
}

void wmdf::FeedScheduler::Shutdown()
{
  if (isworking_)
  {
    isworking_ = false;
    work_thread_->join();
  }
  if (work_thread_ != NULL)
  {
    delete work_thread_;
    work_thread_ = NULL;
  }
	map<DailyOperations, DOTime*>::iterator iter = operations_.begin();
	while (iter != operations_.end())
	{
		if(NULL != iter->second)
		{
			delete iter->second;
			iter->second = NULL;
		}
		iter++;
	}
}

void wmdf::FeedScheduler::CheckDailyOperations()
{
  date_t today = DateTime::today();
  int32_t hour = DateTime::hour();
  map<DailyOperations, DOTime*>::iterator iter = operations_.begin();
  while (iter != operations_.end())
  {
    if (iter->second->equal(today,hour))
    {
      if (iter->first == DO_RESET)
      {
        //ServerEnvironment::feed_server()->NotifyLogout();
        //Utils::SleepTime(1000);
				ServerEnvironment::feed_server()->subscriber_manager()->for_each_subscriber(boost::bind(&wmdf::FeedScheduler::KickAllSubscriber,this,_1));        
        Logger::instance()->WriteEventLog(LL_INFO,MEI_FEED_SERVER_RESET,"scheduler","reset feed server.");
        operations_[DO_RESET]->set_date(today+1);

      }
      else if (iter->first == DO_PREMARKETOPEN)
      {
        Logger::instance()->WriteEventLog(LL_INFO,MEI_FEED_SERVER_PREMARKETOPEN,"scheduler","prepare for market open.");
        operations_[DO_PREMARKETOPEN]->set_date(today+1);
      }
      else if (iter->first == DO_MARKETCLOSE)
      {
        MarketDateSignal* date_signal = new MarketDateSignal[1];
        date_signal[0].market_date = today;
        date_signal[0].market_operation = MO_CLOSE;
        date_signal[0].product_id = PRODEUCT_SYSTEM_ID;
        MdfMessage* message = new MdfMessage();
        message->set_packet_num(0);
        message->set_local_time(DateTime::get_current_time());
        message->set_socket_id(0);
        message->set_product_id(PRODEUCT_SYSTEM_ID);
        message->set_message_id(SYSTEM_MARKET_DATE_SIGNAL_ID);
        message->set_major_version(MAJOR_VERSION_SYSTEM_ID);
        message->set_minor_version(MINOR_VERSION_SYSTEM_ID);
        message->set_record_count(1);
        message->set_body(date_signal);
        message->set_header(NULL);

        SharedMdfMessage shared_mdf_message(message);
        MessagePublisher::PublishOneMessage(shared_mdf_message);

        Logger::instance()->WriteEventLog(LL_INFO,MEI_FEED_SERVER_MARKETCLOSE,"scheduler","prepare for market close.");
        operations_[DO_MARKETCLOSE]->set_date(today+1);
      }
    }
    iter++;
  }
}

void wmdf::FeedScheduler::CheckSubscriber()
{
  ServerEnvironment::feed_server()->subscriber_manager()->for_each_subscriber(boost::bind(&wmdf::FeedScheduler::HandleSubscriber,this,_1));
  for(uint32_t index=0;index<to_be_kicked_.size();index++)
  {
    ServerEnvironment::feed_server()->KickSubscriber(to_be_kicked_[index]);
  }
  to_be_kicked_.clear();
}

void wmdf::FeedScheduler::HandleSubscriber( Subscriber* subscriber )
{
   if (subscriber->to_be_kicked())
   {
     to_be_kicked_.push_back(subscriber->socket_id());
   }
}

void wmdf::FeedScheduler::KickAllSubscriber( Subscriber* subscriber )
{
	subscriber->set_to_be_kicked(true);
}
