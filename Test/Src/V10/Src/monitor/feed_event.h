#ifndef FEED_EVENT_H
#define FEED_EVENT_H

#include "../include/wmdf_api.h"

namespace wmdf
{

const int32_t MONITOR_TRANSFER_STATUS_EVENT = 1;   //传输状态
const int32_t MONITOR_RECEIVE_STATUS_EVENT = 2;    //接收状态

  enum MonitorPoint
  {
    MP_UNKNOWN= 0,
    MP_RECEIVED = 1,       
    MP_BEFORE_ENCODING = 2,
    MP_ENCODED = 3,
    MP_SENDED = 4
  };

  class FeedEvent
  {
  public:
    FeedEvent(void);
    virtual ~FeedEvent(void);
    int32_t event_id(){return event_id_;}
    void set_event_id(int32_t event_id){event_id_ = event_id;}
  private:
    int32_t event_id_;   
  };

  //吞吐量事件，在需要观察的节点提交该事件
  class TransferStatusEvent:public FeedEvent
  {
  public:
    TransferStatusEvent(void);
    virtual ~TransferStatusEvent(void);
    
    MonitorPoint mointor_point(){return monitor_point_;}
    void set_monitor_point(MonitorPoint monitor_point){monitor_point_ = monitor_point;}
    int16_t product_id(){return product_id_;}
    void set_product_id(int16_t product_id){product_id_ = product_id;}
    int16_t message_id(){return message_id_;}
    void set_message_id(int16_t message_id){message_id_ = message_id;}
    int32_t packed_id(){return packed_id_;}
    void set_packed_id(int32_t packed_id){packed_id_ = packed_id;}
    int32_t record_size(){return record_size_;}
    void set_record_size(int32_t record_size){record_size_ = record_size;}
    uint32_t binary_size(){return binary_size_;}
    void set_binary_size(uint32_t binary_size){binary_size_ = binary_size;}
    int32_t delay(){return delay_;}
    void set_delay(int32_t delay){delay_ = delay;}
  private:
    MonitorPoint monitor_point_;
    int16_t product_id_;
    int16_t message_id_;
    int32_t packed_id_;
    int32_t record_size_;  //记录原始大小
    uint32_t binary_size_; //编码后二进制大小
    int32_t delay_;        //延迟
  };

  class ReceiveStatusEvent: public FeedEvent
  {
  public:
    ReceiveStatusEvent(void);
    virtual ~ReceiveStatusEvent(void);

    MonitorPoint mointor_point(){return monitor_point_;}
    void set_monitor_point(MonitorPoint monitor_point){monitor_point_ = monitor_point;}
    int16_t product_id(){return product_id_;}
    void set_product_id(int16_t product_id){product_id_ = product_id;}
    uint32_t binary_size(){return binary_size_;}
    void set_binary_size(uint32_t binary_size){binary_size_ = binary_size;}
  private:
    MonitorPoint monitor_point_;
    int16_t product_id_;
    int32_t binary_size_;
  };



}

#endif
