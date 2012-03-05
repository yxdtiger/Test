#ifndef SUBSCRIBER_GROUP_H
#define SUBSCRIBER_GROUP_H

#include "../include/wmdf_api.h"
#include "../protocol/mdf_message.h"

namespace wmdf
{
  class HeaderCodec;
  class MessageCodec;
  class FeedServer;

  class SubscriberGroup
  {
  public:
    SubscriberGroup();
    virtual ~SubscriberGroup(void);
    void PublishMessage( SharedMdfMessage message, FeedServer* feed_server ); 
    void PublishMessage( SharedRelayMdfMessage message, FeedServer* feed_server ); 
    void set_header_codec(HeaderCodec* header_codec){header_codec_ = header_codec;}
    void set_message_codec(MessageCodec* message_codec){message_codec_ = message_codec;}
    int32_t group_id(){return group_id_;}
    void set_group_id(int32_t group_id);
    int16_t product_id() {return product_id_;}
    void set_product_id(int16_t product_id) {product_id_ = product_id;}
    int16_t message_id() {return message_id_;}
    void set_message_id(int16_t message_id) {message_id_ = message_id;}
    int64_t protocol_unique_id() {return protocol_unique_id_;}
    void set_protocol_unique_id(int64_t protocol_unique_id){protocol_unique_id_ = protocol_unique_id;}
  
    uint64_t send_binary_size() {return send_binary_size_;}
    uint64_t send_record_size() {return send_record_size_;}
    uint32_t msg_delay()        {return msg_delay_;} 
    uint32_t msg_count()        {return msg_count_;} 
    uint32_t delayed_msg_count(){return delayed_msg_count_;} 

  private:
    int32_t group_id_; // windnet::connmanager 中的ID
    int16_t product_id_;
    int16_t message_id_;
    int64_t protocol_unique_id_;
    HeaderCodec* header_codec_;
    MessageCodec* message_codec_;
    volatile uint64_t send_binary_size_;     //发送编码后的字节数
    volatile uint64_t send_record_size_;     //发送的原始记录的字节数
    volatile uint32_t msg_delay_;            //包的总延迟
    volatile uint32_t msg_count_;            //包的总数
    volatile uint32_t delayed_msg_count_;    //发生延迟的包的总数 
  };
}

#endif
