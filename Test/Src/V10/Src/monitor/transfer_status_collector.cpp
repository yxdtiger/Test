#include "transfer_status_collector.h"
#include "../environment/utils.h"
#include "../include/date_time.h"
#include "../session/subscriber_manager.h"
#include "../session/subscriber_group.h"
#include "../environment/server_environment.h"
#include "../session/feed_server.h"
#include "../session/dispatching_center.h"
#include "../session/message_publisher.h"
#include "../product/adapter/iproduct_adapter.h"

using namespace std;

wmdf::TransferStatusCollector::TransferStatusCollector(void)
{
}

wmdf::TransferStatusCollector::~TransferStatusCollector(void)
{
}

void wmdf::TransferStatusCollector::CollectTransferStatus(time_stamp_t time_span)
{
  boost::mutex::scoped_lock lock(mutex_);

  //reset
  std::map<int16_t,TransferDataStat*>::iterator iter = transferStat_.begin();
  while (iter != transferStat_.end())
  {
    TransferDataStat* stat = iter->second;
    stat->receive_data_flow = 0;
    stat->receive_pack_flow = 0;
    stat->receive_packet_count = 0;

    stat->receive_data_size = 0;

    stat->send_data_flow = 0;
    stat->send_pack_flow = 0;
    stat->send_packet_count = 0;

    stat->encoded_size = 0;
    stat->binary_struct_size = 0;

    stat->compression_ratio = 0;
    stat->msg_avg_delay = 0;
    stat->total_msg_delay = 0;
    stat->delayed_msg_count = 0;
    iter++;
  }


  //收集数据
  SubscriberManager* sm = ServerEnvironment::feed_server()->subscriber_manager();
  for (int32_t index=0;index<sm->subscriber_group_count();index++)
  {
    SubscriberGroup* sg = sm->subscriber_groups(index);
    TransferDataStat* stat = FetchTransferDataStat(sg->product_id());
    stat->send_packet_count += sg->msg_count();
    stat->encoded_size += sg->send_binary_size();
    stat->binary_struct_size += sg->send_record_size();
    stat->total_msg_delay += sg->msg_delay();
    stat->delayed_msg_count += sg->delayed_msg_count();

    DispatchingCenter* dc = ServerEnvironment::dispatching_center();

    for (int32_t index=0;index<dc->adapters_count();index++)
    {
      IProductAdapter* pa = dc->adapters(index);
      if ((pa->get_product_id()>= PRODUCT_RELAY_BEGIN_ID)
        && (pa->get_product_id()<=PRODUCT_RELAY_END_ID))
      {
        if (pa->product_source()->IsSubscribedProduct(sg->product_id()))
        {
          TransferDataStat* stat = FetchTransferDataStat(pa->get_product_id());
          stat->send_packet_count += sg->msg_count();
          stat->encoded_size += sg->send_binary_size();
          stat->binary_struct_size += sg->send_record_size();
          stat->total_msg_delay += sg->msg_delay();
          stat->delayed_msg_count += sg->delayed_msg_count();
          break;
        }
      }
    }
  }

  DispatchingCenter* dc = ServerEnvironment::dispatching_center();
  for (int32_t index=0;index<dc->adapters_count();index++)
  {
    IProductAdapter* pa = dc->adapters(index);
    TransferDataStat* stat = FetchTransferDataStat(pa->get_product_id());
    if(pa->product_source()->codec_type()==CT_DBFDATA)
      memcpy(stat->source_ip,pa->product_source()->description().c_str(),pa->product_source()->description().length());
    else
      memcpy(stat->source_ip,pa->product_source()->in_use_ip_address().c_str(),pa->product_source()->in_use_ip_address().length());
    stat->source_port = pa->product_source()->in_use_port();
    stat->source_status = pa->is_connected();
    stat->receive_data_size = pa->receive_size();
    stat->receive_packet_count = pa->receive_packet_count();
    stat->receive_data_flow = double((stat->receive_data_size - stat->pre_receive_data_size ) /1024.0 *1000.0/time_span); //kb/s
    stat->receive_pack_flow = double((stat->receive_packet_count - stat->pre_receive_packet_count)*1000.0/time_span); //packet/s
    stat->pre_receive_data_size = stat->receive_data_size;
    stat->pre_receive_packet_count = stat->receive_packet_count;
  }


  //计算流量
  iter = transferStat_.begin();
  while (iter != transferStat_.end())
  {
    TransferDataStat* stat = iter->second;
    stat->send_data_flow = double((stat->encoded_size-stat->pre_encoded_size) /1024.0*1000.0/time_span); //kb/s
    stat->pre_encoded_size = stat->encoded_size;
    stat->send_pack_flow = double((stat->send_packet_count-stat->pre_send_packet_count) *1000.0 / time_span); //packet/s
    stat->send_packet_delta = stat->send_packet_count-stat->pre_send_packet_count;
    stat->pre_send_packet_count = stat->send_packet_count;
    stat->compression_ratio = (stat->encoded_size/1.0) / ((stat->binary_struct_size > 0 ? stat->binary_struct_size:1)/1.0); //%
    stat->msg_avg_delay = (stat->total_msg_delay-stat->pre_total_msg_delay) / ((stat->delayed_msg_count-stat->pre_delayed_msg_count) > 0 ? (stat->delayed_msg_count-stat->pre_delayed_msg_count):1); //ms
    stat->delayed_msg_delta = stat->delayed_msg_count-stat->pre_delayed_msg_count;
    stat->pre_total_msg_delay = stat->total_msg_delay;
    stat->pre_delayed_msg_count = stat->delayed_msg_count;
    iter++;
  }
}

TransferDataStat* wmdf::TransferStatusCollector::FetchTransferDataStat( int16_t product_id )
{
  std::map<int16_t,TransferDataStat*>::iterator iter = transferStat_.find(product_id);
  if (iter != transferStat_.end())
  {
    return iter->second;
  }
  else
  {
    TransferDataStat* stat= new TransferDataStat();
    memset(stat,0,sizeof(TransferDataStat));
    stat->product_id = product_id;
    transferStat_.insert(make_pair(product_id,stat));
    return stat;
  }
}

void wmdf::TransferStatusCollector::PrintTransferStatus()
{
  boost::mutex::scoped_lock lock(mutex_);
  printf("******************************* transfer status *******************************\n");
  printf("time  product  delay(ms) r_flow(KB/S) s_flow(KB/S)  r_size(MB)  s_size(MB)  compress(%)  r_total_pack  s_total_pack\n");
  if(transferStat_.size()>0)
  {

    int32_t time=DateTime::time2hms(DateTime::get_current_time());
    std::map<int16_t,TransferDataStat*>::iterator iter = transferStat_.begin();
    while(iter!=transferStat_.end())
    {
      TransferDataStat* stat = iter->second;
      printf("%d %d    %d(%d/%d)      %.2f      %.2f     %.2f     %.2f     %.2f     %d     %d\n",
        time,stat->product_id, stat->msg_avg_delay,stat->delayed_msg_delta,stat->send_packet_delta,
        stat->receive_data_flow, stat->send_data_flow,
        (double)stat->receive_data_size/(1024.0*1024.0),(double)stat->encoded_size/(1024.0*1024.0),
        stat->compression_ratio,stat->receive_packet_count,stat->send_packet_count);
      iter++;
    }
  }
  printf("********************************** end ****************************************\n");
}

void wmdf::TransferStatusCollector::PublishTransferStatus()
{
  boost::mutex::scoped_lock lock(mutex_);
  if (transferStat_.size() <= 0) return;
  int32_t index=0;
  TransferDataStat* transfer_datas = new TransferDataStat[transferStat_.size()];
  memset(transfer_datas,0,sizeof(TransferDataStat)*transferStat_.size());
  std::map<int16_t,TransferDataStat*>::iterator iter = transferStat_.begin();
  while(iter != transferStat_.end())
  {
    memcpy(&transfer_datas[index],iter->second,sizeof(TransferDataStat));
    iter++;
    index++;
  }

  MdfMessage* message = new MdfMessage();
  message->set_local_time(DateTime::get_current_time());
  message->set_socket_id(0);
  message->set_product_id(PRODEUCT_MONITOR_ID);
  message->set_message_id(MONITOR_TRANSFER_STATUS_ID);
  message->set_major_version(MAJOR_VERSION_MONITOR_ID);
  message->set_minor_version(MINOR_VERSION_MONITOR_ID);
  message->set_record_count((int32_t)transferStat_.size());
  message->set_body(transfer_datas);
  message->set_header(NULL);

  SharedMdfMessage shared_mdf_message(message);
  MessagePublisher::PublishMonitorMessage(shared_mdf_message);
}

TransferDataStat* wmdf::TransferStatusCollector::FindTransferDataStat( int16_t product_id )
{
  std::map<int16_t,TransferDataStat*>::iterator iter = transferStat_.find(product_id);
  if (iter != transferStat_.end())
  {
    return iter->second;
  }
  return NULL;
}
