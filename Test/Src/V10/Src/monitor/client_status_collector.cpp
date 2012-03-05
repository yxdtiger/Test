#include "client_status_collector.h"
#include <stdio.h>
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include "../environment/server_environment.h"
#include "../session/subscriber_manager.h"
#include "../session/subscriber.h"
#include "../session/feed_server.h"
#include "../protocol/protocol_description.h"
#include "../environment/utils.h"
#include "../usermanager/user.h"
#include "../usermanager/user_manager.h"
#include "../include/date_time.h"
#include "../session/message_publisher.h"


using namespace wmdf;
wmdf::ClientStatusCollector::ClientStatusCollector(void)
{
  
}

wmdf::ClientStatusCollector::~ClientStatusCollector(void)
{
  std::map<int32_t,ClientStatus*>::iterator iter = client_statuses_.begin();
  while(iter != client_statuses_.end())
  {
    Utils::DestroyClientStatus(iter->second);
    iter++;
  }
  client_statuses_.clear();
}

void wmdf::ClientStatusCollector::CollectClientStatus(time_stamp_t time_span)
{
  boost::mutex::scoped_lock lock(mutex_);
  time_span_ = time_span;
  ServerEnvironment::feed_server()->subscriber_manager()->for_each_subscriber(boost::bind(&wmdf::ClientStatusCollector::HandleSubscriber,this,_1));

  std::map<int32_t,ClientStatus*>::iterator iter = client_statuses_.begin();
  while(iter != client_statuses_.end())
  {
    if (!IsSocketValid(iter->first))
    {
      Utils::DestroyClientStatus(iter->second);
      client_statuses_.erase(iter++);
    }
    else
    {
      ++iter;
    }
  }
  valid_socket_.clear();
}

void wmdf::ClientStatusCollector::HandleSubscriber( Subscriber* subscriber )
{
  valid_socket_.push_back(subscriber->socket_id());
  ClientStatus* client_status = FetchClientStatus(subscriber->socket_id());
  client_status->socket_id = subscriber->socket_id();
  User* user = ServerEnvironment::user_manager()->FindUser(subscriber->user_id());
  memcpy(client_status->client_ip,subscriber->ip_address().c_str(),subscriber->ip_address().length());
  if (NULL != user)
  {
    memcpy(client_status->user_name,user->user_name().c_str(),user->user_name().length());
  }
  client_status->duration = DateTime::calctimespan(DateTime::get_current_time(),subscriber->connected_time());
  client_status->send_bytes = subscriber->get_send_size();
  client_status->send_data_flow = double((client_status->send_bytes - client_status->pre_send_bytes) /1024.0 *1000.0/time_span_); //kb/s
  client_status->pre_send_bytes = client_status->send_bytes;
  client_status->msg_in_queue = subscriber->get_msg_in_queue();
  if (client_status->product_count == 0)
  {
    client_status->product_count = subscriber->subscribed_product_count();
    client_status->product_infos = new ProductInfo[client_status->product_count];
  }
  else
  {
    client_status->product_count = subscriber->subscribed_product_count();
    delete[] client_status->product_infos;
    client_status->product_infos = new ProductInfo[client_status->product_count];
  }
  for(int j=0;j<subscriber->subscribed_product_count();j++)
  {
    client_status->product_infos[j].product_id = subscriber->subscribed_products(j)->product_id();
    client_status->product_infos[j].major_version = subscriber->subscribed_products(j)->major_version();
    client_status->product_infos[j].minor_version = subscriber->subscribed_products(j)->minor_version();
    client_status->product_infos[j].codec = subscriber->subscribed_products(j)->codec_type();
  }
}

void wmdf::ClientStatusCollector::PrintClientStatus()
{
  boost::mutex::scoped_lock lock(mutex_);
  printf("******************************* client status *********************************\n");
  printf("active clients: %d \n",client_statuses_.size());
  if (client_statuses_.size())
  {
    printf("id  user_name    ip    duration    flow(KB/S)  send_size(MB)  queued_msg \n");
    std::map<int32_t,ClientStatus*>::iterator iter = client_statuses_.begin();
    while(iter != client_statuses_.end())
    {
      ClientStatus* cs = iter->second;
      printf("%d   %s   %s   %s       %.2f      %.2f        %d \n",cs->socket_id,cs->user_name,cs->client_ip,DateTime::time2hms(cs->duration,":").c_str(),cs->send_data_flow,(double)cs->send_bytes/1024.0/1024.0,cs->msg_in_queue);
      for (int32_t j=0;j< cs->product_count;j++ )
      {
        ProductInfo* pi = &cs->product_infos[j];
        printf("    subscribed: %d      %d     %d     %d \n",pi->product_id,pi->major_version,pi->minor_version,pi->codec);
      }
      iter++;
    } 
  }
  printf("********************************** end ****************************************\n");  
}

void wmdf::ClientStatusCollector::PublishClientStatus()
{
  boost::mutex::scoped_lock lock(mutex_);
  if (client_statuses_.size() <= 0) return;
  ClientStatus* client_statuses = new ClientStatus[client_statuses_.size()];
  memset(client_statuses,0,sizeof(ClientStatus)*client_statuses_.size());
  int32_t index=0;
  std::map<int32_t,ClientStatus*>::iterator iter = client_statuses_.begin();
  while(iter != client_statuses_.end())
  {
    ClientStatus* src = iter->second;
    ClientStatus* dst = &client_statuses[index];
    memcpy(dst,src,sizeof(ClientStatus));
    dst->product_infos = new ProductInfo[dst->product_count];
    memcpy(dst->product_infos,src->product_infos,sizeof(ProductInfo)*dst->product_count);
    /*dst->socket_id = src->socket_id;
    memcpy(dst->client_ip,src->client_ip,sizeof(src->client_ip));
    memcpy(dst->user_name,src->user_name,sizeof(src->user_name));
    dst->duration = src->duration;
    dst->send_data_flow = src->send_data_flow;
    dst->product_count = src->product_count;
    dst->product_infos = new ProductInfo[dst->product_count];
    for (int32_t j=0;j< dst->product_count;j++ )
    {
      dst->product_infos[j].product_id = src->product_infos[j].product_id;
      dst->product_infos[j].major_version = src->product_infos[j].major_version;
      dst->product_infos[j].minor_version = src->product_infos[j].minor_version;
      dst->product_infos[j].codec = src->product_infos[j].codec;
    }    */
    iter++;
    index++;
  }
  

  MdfMessage* message = new MdfMessage();
  message->set_local_time(DateTime::get_current_time());
  message->set_packet_num(0);
  message->set_socket_id(0);
  message->set_product_id(PRODEUCT_MONITOR_ID);
  message->set_message_id(MONITOR_CLIENT_STATUS_ID);
  message->set_major_version(MAJOR_VERSION_MONITOR_ID);
  message->set_minor_version(MINOR_VERSION_MONITOR_ID);
  message->set_header(NULL);
  message->set_record_count((int32_t)client_statuses_.size());
  message->set_body(client_statuses);
  
  SharedMdfMessage shared_mdf_message(message);
  MessagePublisher::PublishMonitorMessage(shared_mdf_message);
}

ClientStatus* wmdf::ClientStatusCollector::FetchClientStatus( int32_t socket_id )
{
  std::map<int32_t,ClientStatus*>::iterator iter = client_statuses_.find(socket_id);
  if (iter != client_statuses_.end())
  {
    return iter->second;
  }
  else
  {
    ClientStatus* cs= new ClientStatus();
    memset(cs,0,sizeof(ClientStatus));
    client_statuses_.insert(make_pair(socket_id,cs));
    return cs;
  }
}

bool wmdf::ClientStatusCollector::IsSocketValid( int32_t socket_id )
{
  for (uint32_t index=0;index < valid_socket_.size();index++)
  {
    if (valid_socket_[index] == socket_id)
    {
      return true;
    }
  }
  return false;
}
