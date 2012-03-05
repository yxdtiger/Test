#include "simulator_adapter.h"
#include "simulator_utils.h"
#include "../../environment/utils.h"
#include "../../include/wmdf_structs.h"
#include "../../protocol/mdf_message.h"
#include "../../session/message_publisher.h"
#include "../../include/date_time.h"
#include "../../monitor/logger.h"
#include <boost/bind.hpp>


using namespace wmdf;

wmdf::SimulatorAdapter::SimulatorAdapter(ProductSource* product_source)
:IProductAdapter(product_source)
{
  InitData();
  work_thread_=NULL;
}

wmdf::SimulatorAdapter::~SimulatorAdapter(void)
{
  if(is_working_)
  {
    Stop();
  }
  if (work_thread_ != NULL)
  {
    delete work_thread_;
    work_thread_ = NULL;
  }

  map<string,L2MarketSnapshot*>::iterator iter = markets.begin();
  while(iter != markets.end())
  {
    if(NULL != iter->second)
    {
      delete iter->second;
      iter->second = NULL;
    }
    iter++;
  }
  markets.clear();
  codes.clear();
}

void wmdf::SimulatorAdapter::Start()
{
  while(!is_working_)
  {
    is_working_ = true;
    work_thread_ = new boost::thread(boost::bind(&SimulatorAdapter::ProduceMDFMessage,this));
    Logger::instance()->WriteLog(LL_INFO,"Product Adapter: Simulator started.");
  }
  
}

void wmdf::SimulatorAdapter::Stop()
{
  if (is_working_)
  {
    is_working_ = false;
    work_thread_->join();
    Logger::instance()->WriteLog(LL_INFO,"Product Adapter: Simulator stop.");
  }
}

void wmdf::SimulatorAdapter::InitData()
{
  codes.push_back("600000.SH");
  codes.push_back("600006.SH");
  codes.push_back("601998.SH");
  codes.push_back("000002.SZ");
  codes.push_back("000728.SZ");
  codes.push_back("300024.SZ");

  date_t date = DateTime::today();
  time_stamp_t currTime = DateTime::get_current_time();
  
  L2MarketSnapshot* marketData=NULL;

  for(uint32_t i=0;i<codes.size();i++)
  {
    marketData = new L2MarketSnapshot();
		#pragma region 初始化市场快照
    memcpy(marketData->wind_code,codes[i].c_str(),sizeof(marketData->wind_code));
    //marketData->trade_date    = date;
    marketData->data_time_stamp    = currTime;						
    marketData->pre_close    = 21.6+i*13;
    marketData->open_price		= 21.65+i*13;
    marketData->high_price		= 22.9+i*13;
    marketData->low_price			= 20.5+i*13;
    marketData->new_price			= 21.8+i*13;
    marketData->total_volume				=	1000000+i*110000;
    marketData->total_amount	=	marketData->new_price*marketData->total_volume;							
    marketData->bid_price1		=marketData->new_price-0.01;
    marketData->bid_price2		=marketData->new_price-0.02;
    marketData->bid_price3		=marketData->new_price-0.03;
    marketData->bid_price4		=marketData->new_price-0.04;
    marketData->bid_price5		=marketData->new_price-0.05;
    marketData->bid_price6		=marketData->new_price-0.06;
    marketData->bid_price7		=marketData->new_price-0.07;
    marketData->bid_price8		=marketData->new_price-0.08;
    marketData->bid_price9		=marketData->new_price-0.09;
    marketData->bid_price10		=marketData->new_price-0.10;	

		marketData->ask_price1		=marketData->new_price+0.01;
		marketData->ask_price2		=marketData->new_price+0.02;
		marketData->ask_price3		=marketData->new_price+0.03;
		marketData->ask_price4		=marketData->new_price+0.04;
		marketData->ask_price5		=marketData->new_price+0.05;
		marketData->ask_price6		=marketData->new_price+0.06;
		marketData->ask_price7		=marketData->new_price+0.07;
		marketData->ask_price8		=marketData->new_price+0.08;
		marketData->ask_price9		=marketData->new_price+0.09;
		marketData->ask_price10		=marketData->new_price+0.10;	

    marketData->ask_size1		=2600+i*19;
    marketData->ask_size2		=2000+i*19;
    marketData->ask_size3		=1900+i*19;
    marketData->ask_size4		=2100+i*19;
    marketData->ask_size5		=2200+i*19;
    marketData->ask_size6		=2600+i*19;
    marketData->ask_size7		=2000+i*19;
    marketData->ask_size8		=1900+i*19;
    marketData->ask_size9		=2100+i*19;
    marketData->ask_size10	=2200+i*19;

    marketData->bid_size1		=2300+i*19;
    marketData->bid_size2		=2400+i*19;
    marketData->bid_size3		=2000+i*19;
    marketData->bid_size4		=1500+i*19;
    marketData->bid_size5		=1300+i*19;
    marketData->bid_size6		=2300+i*19;
    marketData->bid_size7		=2400+i*19;
    marketData->bid_size8		=2000+i*19;
    marketData->bid_size9		=1500+i*19;
    marketData->bid_size10	=1300+i*19;

    marketData->weighted_average_ask_price = 21.75+i*13;
    marketData->weighted_average_bid_price = 21.85+i*13;
    marketData->total_bid_size = int64_t(0.4*marketData->total_volume);
    marketData->total_bid_size = int64_t(0.6*marketData->total_volume);
    //marketData->ytm =0.11+i/10;
    //marketData->iopv=21.8+i*13;
    #pragma endregion
		markets.insert(make_pair(codes[i],marketData));
  }
}

void wmdf::SimulatorAdapter::ProduceMDFMessage()
{
  while(is_working_)
  {
    GenerateTransData();
    GenerateMarketData(); 
    GenerateOrderQueueData();
    Utils::SleepTime(2);
  }
}

void wmdf::SimulatorAdapter::ProcessTrans(L2Transaction* trans)
{
  L2MarketSnapshot* marketData =markets[trans->wind_code];
  if(SimulatorUtils::isDoubleEqual(trans->price,marketData->ask_price1))
  {
    marketData->ask_size1-=trans->volume;
    if(marketData->ask_size1<=0)
    {
      marketData->ask_size1 = marketData->ask_size2;
      marketData->ask_size2 = marketData->ask_size3;
      marketData->ask_size3 = marketData->ask_size4;
      marketData->ask_size4 = marketData->ask_size5;
      marketData->ask_size5 = marketData->ask_size6;
      marketData->ask_size6 = marketData->ask_size7;
      marketData->ask_size7 = marketData->ask_size8;
      marketData->ask_size8 = marketData->ask_size9;
      marketData->ask_size9 = marketData->ask_size10;

      marketData->ask_price1 = marketData->ask_price2;
      marketData->ask_price2 = marketData->ask_price3;
      marketData->ask_price3 = marketData->ask_price4;
      marketData->ask_price4 = marketData->ask_price5;
      marketData->ask_price5 = marketData->ask_price6;
      marketData->ask_price6 = marketData->ask_price7;
      marketData->ask_price7 = marketData->ask_price8;
      marketData->ask_price8 = marketData->ask_price9;
      marketData->ask_price9 = marketData->ask_price10;
			marketData->ask_price10 = marketData->ask_price10+0.01;
    }
  }
  else if(SimulatorUtils::isDoubleEqual(trans->price,marketData->bid_price1))
  {
    marketData->bid_size1-=trans->volume;
    if(marketData->bid_size1<=0)
    {
      marketData->bid_size1 = marketData->bid_size2;
      marketData->bid_size2 = marketData->bid_size3;
      marketData->bid_size3 = marketData->bid_size4;
      marketData->bid_size4 = marketData->bid_size5;
      marketData->bid_size5 = marketData->bid_size6;
      marketData->bid_size6 = marketData->bid_size7;
      marketData->bid_size7 = marketData->bid_size8;
      marketData->bid_size8 = marketData->bid_size9;
      marketData->bid_size9 = marketData->bid_size10;

      marketData->bid_price1 = marketData->bid_price2;
      marketData->bid_price2 = marketData->bid_price3;
      marketData->bid_price3 = marketData->bid_price4;
      marketData->bid_price4 = marketData->bid_price5;
      marketData->bid_price5 = marketData->bid_price6;
      marketData->bid_price6 = marketData->bid_price7;
      marketData->bid_price7 = marketData->bid_price8;
      marketData->bid_price8 = marketData->bid_price9;
      marketData->bid_price9 = marketData->bid_price10;
      marketData->bid_price10 = marketData->bid_price10-0.01;
    }
  }
}

void wmdf::SimulatorAdapter::GenerateMarketData()
{
  MdfMessage* mdf_msg=new MdfMessage();
  mdf_msg->set_local_time(DateTime::get_current_time());
  mdf_msg->set_socket_id(0);
  mdf_msg->set_product_id(PRODUCT_SSE_LEVEL2_ID);
  mdf_msg->set_message_id(SSEL2_MARKETDATA_ID);
  mdf_msg->set_major_version(MAJOR_VERSION_SSE_LEVEL2_ID);
  mdf_msg->set_minor_version(MINOR_VERSION_SSE_LEVEL2_ID);
  MarketMessageHeader* header = new MarketMessageHeader();
  header->place_holder = 0;
	mdf_msg->set_packet_num(++packet_id_);
  mdf_msg->set_header(header);
  mdf_msg->set_record_count((int32_t)codes.size());

  L2MarketSnapshot* datas = new L2MarketSnapshot[codes.size()];
  L2MarketSnapshot* marketData=NULL;
  for(int i=0;i<mdf_msg->record_count();i++)
  {
    marketData =markets[codes[i]];
    memcpy(&datas[i],marketData,sizeof(L2MarketSnapshot));
  }
  mdf_msg->set_body(datas);
  PublishMdfMessage(mdf_msg);
}




void wmdf::SimulatorAdapter::GenerateTransData()
{
  MdfMessage* mdf_msg=new MdfMessage();
  mdf_msg->set_local_time(DateTime::get_current_time());
  mdf_msg->set_socket_id(0);
  mdf_msg->set_product_id(PRODUCT_SSE_LEVEL2_ID);
  mdf_msg->set_major_version(MAJOR_VERSION_SSE_LEVEL2_ID);
  mdf_msg->set_minor_version(MINOR_VERSION_SSE_LEVEL2_ID);
  mdf_msg->set_message_id(SSEL2_TRANSACTION_ID);
  MarketMessageHeader* header = new MarketMessageHeader();
  header->place_holder = 0;
	mdf_msg->set_packet_num(++packet_id_);
	mdf_msg->set_header(header);
  

  
  
  int32_t count = 0;
  int32_t size =(int32_t)codes.size();
  int* counts = new int[size];
  for(int t=0;t<size;t++)
  {
    counts[t] = SimulatorUtils::GetRandomValue(0,10);
    count+=counts[t];
  }
  mdf_msg->set_record_count(count);
  L2Transaction* datas= new L2Transaction[count]; 
  memset(datas,0,sizeof(L2Transaction)*count);
  L2MarketSnapshot* marketData=NULL;

  for(int i=0,index=0;i<size;i++)
  {
    marketData =markets[codes[i]];
    int value = counts[i];
    for(int j=0;j<value;j++)
    {
      #pragma region 生成逐笔成交的新值
      #pragma warning(disable : 4996)
      strcpy(datas[index].wind_code,codes[i].c_str());
      #pragma warning(default : 4996)  
      
      datas[index].rec_no++;
      datas[index].trade_time = DateTime::get_current_time();
      if(SimulatorUtils::GetRandomValue(0,2)<1)
      {
        datas[index].price=marketData->bid_price1;
        if(marketData->bid_size1<10)
          datas[index].volume=marketData->bid_size1;
        else
          datas[index].volume=marketData->bid_size1*(value%5+1)/5;
      }
      else
      {
        datas[index].price=marketData->ask_price1;
        if(marketData->ask_size1<10)
          datas[index].volume=marketData->ask_size1;
        else
          datas[index].volume=marketData->ask_size1*(value%5+1)/5;
      }
      datas[index].amount = datas[index].price*datas[index].volume;
      ProcessTrans(&datas[index]);
      index++;      
      #pragma endregion
    }
  }
  
  mdf_msg->set_body(datas);
  delete[] counts;
  PublishMdfMessage(mdf_msg);
}

void wmdf::SimulatorAdapter::GenerateOrderQueueData()
{
  MdfMessage* mdf_msg=new MdfMessage();
  mdf_msg->set_local_time(DateTime::get_current_time());
  mdf_msg->set_socket_id(0);
  mdf_msg->set_product_id(PRODUCT_SSE_LEVEL2_ID);
  mdf_msg->set_major_version(MAJOR_VERSION_SSE_LEVEL2_ID);
  mdf_msg->set_minor_version(MINOR_VERSION_SSE_LEVEL2_ID);
  mdf_msg->set_message_id(SSEL2_ORDERQUEUE_ID);
  MarketMessageHeader* header = new MarketMessageHeader();
  header->place_holder = 0;
	mdf_msg->set_packet_num(++packet_id_);
  mdf_msg->set_header(header);
  mdf_msg->set_record_count((int32_t)codes.size());

  L2OrderQueue* datas = new L2OrderQueue[codes.size()];
  for(uint32_t i=0;i<codes.size();i++)
  {


    #pragma warning(disable : 4996)
    strcpy(datas[i].wind_code,codes[i].c_str());
    #pragma warning(default : 4996)  

    int value = SimulatorUtils::GetRandomValue(0,20);
    if(value ==1 )
    {      
      GenerateFullImageOrderQueue(&datas[i]);
    }
    else
    {
      if(value=2)
      {
         GenerateAddDeleteOrderQueue(&datas[i]);
      }
      else
      {
         GenerateUpdateOrderQueue(&datas[i]);
      }
    }  
  }
  mdf_msg->set_body(datas);
  PublishMdfMessage(mdf_msg);
}

void wmdf::SimulatorAdapter::GenerateFullImageOrderQueue(L2OrderQueue* orderQueue)
{
  L2MarketSnapshot* marketData = markets[orderQueue->wind_code];
  L2OrderQueuePriceLevel* orderQueueLevel=NULL;		
  L2OrderQueueItem* order_queue_level_qty=NULL;

  orderQueue->data_time_stamp=DateTime::get_current_time();
  orderQueue->order_side = SimulatorUtils::GetRandomValue(1,3);
  orderQueue->image_status = 1;
  orderQueue->price_level_count=1;

  orderQueueLevel = new L2OrderQueuePriceLevel[orderQueue->price_level_count];    
  int value = SimulatorUtils::GetRandomValue(1,100);
  for(int j=0;j<orderQueue->price_level_count;j++)
  {
    orderQueueLevel[j].operator_type=0;
    if(orderQueue->order_side ==1)
      orderQueueLevel[j].order_price=marketData->bid_price1;
    else
      orderQueueLevel[j].order_price=marketData->ask_price1;

    orderQueueLevel[j].total_number=value+SimulatorUtils::GetRandomValue(0,50);
    orderQueueLevel[j].orders_number=value;

    order_queue_level_qty = new L2OrderQueueItem[orderQueueLevel[j].orders_number];        
    for(int k=0;k<orderQueueLevel[j].orders_number;k++)
    {         
      order_queue_level_qty[k].operator_type=0;
      order_queue_level_qty[k].entry_id=0;
      order_queue_level_qty[k].size= SimulatorUtils::GetRandomValue(1,1000);      
    }
    orderQueueLevel[j].order_items = order_queue_level_qty;
  }
  orderQueue->price_levels = orderQueueLevel;
}

void wmdf::SimulatorAdapter::GenerateUpdateOrderQueue(L2OrderQueue* orderQueue)
{
  L2MarketSnapshot* marketData = markets[orderQueue->wind_code];
  L2OrderQueuePriceLevel* orderQueueLevel=NULL;		
  L2OrderQueueItem* order_queue_level_qty=NULL;

  orderQueue->data_time_stamp=DateTime::get_current_time();
  orderQueue->order_side = SimulatorUtils::GetRandomValue(1,3);
  orderQueue->image_status = 2;//update
  orderQueue->price_level_count=1;

  orderQueueLevel = new L2OrderQueuePriceLevel[orderQueue->price_level_count];    
  int value = SimulatorUtils::GetRandomValue(1,50);
  for(int j=0;j<orderQueue->price_level_count;j++)
  {
    orderQueueLevel[j].operator_type=2;
    if(orderQueue->order_side ==1)
      orderQueueLevel[j].order_price=marketData->bid_price1;
    else
      orderQueueLevel[j].order_price=marketData->ask_price1;

    orderQueueLevel[j].total_number=value+SimulatorUtils::GetRandomValue(0,50);
    orderQueueLevel[j].orders_number=value;

    order_queue_level_qty = new L2OrderQueueItem[orderQueueLevel[j].orders_number];        
    for(int k=0;k<orderQueueLevel[j].orders_number;k++)
    {         
      order_queue_level_qty[k].operator_type=2;//update
      order_queue_level_qty[k].entry_id=SimulatorUtils::GetRandomValue(0,10);
      order_queue_level_qty[k].size= SimulatorUtils::GetRandomValue(1,1000);      
    }
    orderQueueLevel[j].order_items = order_queue_level_qty;
  }
  orderQueue->price_levels = orderQueueLevel;  
}

void wmdf::SimulatorAdapter::GenerateAddDeleteOrderQueue(L2OrderQueue* orderQueue)
{
  L2MarketSnapshot* marketData = markets[orderQueue->wind_code];
  L2OrderQueuePriceLevel* orderQueueLevel=NULL;		
  L2OrderQueueItem* order_queue_level_qty=NULL;

  orderQueue->data_time_stamp=DateTime::get_current_time();
  orderQueue->order_side = SimulatorUtils::GetRandomValue(1,3);
  orderQueue->image_status = 2;//update
  orderQueue->price_level_count=2;

  orderQueueLevel = new L2OrderQueuePriceLevel[orderQueue->price_level_count];    
  int value = SimulatorUtils::GetRandomValue(1,50);
  for(int j=0;j<orderQueue->price_level_count;j++)
  {
    orderQueueLevel[j].operator_type=j*2+1;//1:add、3:delete
    if(orderQueueLevel[j].operator_type==3)
    {
      if(orderQueue->order_side ==1)
        orderQueueLevel[j].order_price=marketData->bid_price1;
      else
        orderQueueLevel[j].order_price=marketData->ask_price1;

      orderQueueLevel[j].total_number=value+SimulatorUtils::GetRandomValue(0,50);
      orderQueueLevel[j].orders_number=0;
      orderQueueLevel[j].order_items = NULL;;
    }
    else
    {
      if(orderQueue->order_side ==1)
        orderQueueLevel[j].order_price=marketData->bid_price1+0.01;
      else
        orderQueueLevel[j].order_price=marketData->ask_price1-0.01;

      orderQueueLevel[j].total_number=value+SimulatorUtils::GetRandomValue(0,50);
      orderQueueLevel[j].orders_number=value;

      order_queue_level_qty = new L2OrderQueueItem[orderQueueLevel[j].orders_number];        
      for(int k=0;k<orderQueueLevel[j].orders_number;k++)
      {         
        order_queue_level_qty[k].operator_type=0;//2 update
        order_queue_level_qty[k].entry_id=0;
        order_queue_level_qty[k].size= SimulatorUtils::GetRandomValue(1,1000);      
      }
      orderQueueLevel[j].order_items = order_queue_level_qty;
    }
  }
  orderQueue->price_levels = orderQueueLevel;
}

bool wmdf::SimulatorAdapter::is_connected()
{
    return is_working_;
}



