#include "subscriber_group.h"

#include <iostream>
#include "net_module.h"
#include "feed_server.h"
#include "../protocol/message_codec.h"
#include "../protocol/header_codec.h"
#include "../environment/server_environment.h"
#include "../include/date_time.h"


wmdf::SubscriberGroup::SubscriberGroup()
{
  send_binary_size_ = 0;   
  send_record_size_ = 0;
  msg_delay_ = 0;
  msg_count_ = 0;
  delayed_msg_count_ = 0;
}

wmdf::SubscriberGroup::~SubscriberGroup(void)
{
  if (NULL != header_codec_ )
  {
    delete header_codec_;
    header_codec_ = NULL;
  }
  if (NULL != message_codec_)
  {
    delete message_codec_;
    message_codec_ = NULL;
  }
}

void wmdf::SubscriberGroup::PublishMessage( SharedMdfMessage message, FeedServer* feed_server )
{
  if (feed_server->GetConnectionCount(group_id_) <= 0) return; 
  int header_size = header_codec_->EncodeData();
  int buffer_size = message_codec_->EncodeData(message->body(),message->record_count());
  Message* raw_message = CreateMessage(header_size+buffer_size);
  raw_message->local_time = message->local_time();
  raw_message->packet_num = message->packet_num();
  raw_message->product_id = message->product_id();
  raw_message->message_id = message->message_id();
  if (header_size > 0)
  {
    header_codec_->DequeueBuffer(raw_message->data);
  }
  if (buffer_size > 0)
  {
    message_codec_->DequeueBuffer(raw_message->data+header_size);
  }
  
  //记录发送的流量及延迟
  send_binary_size_ += (header_size+buffer_size);
  send_record_size_ += sizeof(MdfRecord) + (header_codec_->SizeOfRecords()+ message_codec_->SizeOfRecords(message->body(),message->record_count()));
  time_stamp_t delay = DateTime::calctimespan( DateTime::get_current_time(), message->local_time()); 
  if (delay > 0) 
  {
     msg_delay_ += delay;
    delayed_msg_count_++;
  }
  msg_count_+=1;

  //发送数据
  feed_server->BroadcastMsg(group_id_,raw_message);
}

void wmdf::SubscriberGroup::PublishMessage( SharedRelayMdfMessage message, FeedServer* feed_server )
{
  if (feed_server->GetConnectionCount(group_id_) <= 0) return; 
  Message* raw_message = CreateMessage(message->length);
  raw_message->local_time = message->local_time;
  raw_message->packet_num = message->packet_num;
  raw_message->product_id = message->product_id;
  raw_message->message_id = message->message_id;
  memcpy(raw_message->data,message->data,message->length);
  //记录发送的流量及延迟
  send_binary_size_ += message->length;
  send_record_size_ += message->length;
  time_stamp_t delay = DateTime::calctimespan(DateTime::get_current_time(), message->local_time); 
  if (delay > 0) 
  {
    msg_delay_ += delay;  
    delayed_msg_count_++;
  }
  msg_count_ += 1;
  
  //发送数据
  feed_server->BroadcastMsg(group_id_,raw_message);
}

void wmdf::SubscriberGroup::set_group_id( int32_t group_id )
{
  group_id_ = group_id;
}

