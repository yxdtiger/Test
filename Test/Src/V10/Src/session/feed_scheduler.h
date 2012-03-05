#ifndef FEED_SCHEDULER_H
#define FEED_SCHEDULER_H

#include <map>
#include <vector>
#include <boost/thread.hpp>
#include "../include/date_time.h"

namespace wmdf
{
  //if feed server needs to be restarted, 
  //it should be shut down after 5:00pm and 
  //restarted before 7:00am the other day.
  enum DailyOperations
  {
    DO_RESET,         //23:00 FeedServer dicconnnect all feed client; and feed client will reconnect to feed serer;
    DO_PREMARKETOPEN, //6:00  FeedServer will clean up data for the past day;
    DO_MARKETCLOSE   //to do Market Close
  };
  
  class DOTime
  {
  public:
   DOTime(date_t date,int32_t hour){date_= date;hour_=hour;}
   ~DOTime(){}
   bool equal(date_t date,int32_t hour)
   {
     return (date_ == date) && (hour_ == hour);
   }
   void set_date(date_t date){date_ = date;}
   void set_hour(int32_t hour){hour_=hour;}
  private:
    date_t date_;
    int32_t hour_;
  };
  
  class Subscriber;

  class FeedScheduler
  {
  public:
    FeedScheduler(void);
    virtual ~FeedScheduler(void);
    void Startup();
    void Shutdown();
  private:
    void Schedule();
  private:
    void CheckDailyOperations();
  private:
    void CheckSubscriber();
    void HandleSubscriber(Subscriber* subscriber);
		void KickAllSubscriber(Subscriber* subscriber);
    void SendMarketDateSignal(date_t date,MarketOperation market_operation);
  private:
    std::map<DailyOperations,DOTime*> operations_;
    std::vector<int32_t> to_be_kicked_;
    boost::thread* work_thread_;
    volatile bool isworking_;
  };
}

#endif
