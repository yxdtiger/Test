#ifndef SYSTEM_CONFIG_H
#define SYSTEM_CONFIG_H

#include "persistable.h"
#include "memento.h"
#include "../include/wmdf_api.h"

namespace wmdf
{
  class SystemConfig : public Persistable
  {
  public:
    SystemConfig(void);
    virtual ~SystemConfig(void);
    virtual bool SaveState(Memento* m);
    virtual bool LoadState(Memento* m);
    int32_t server_port(){return server_port_;}
    void set_server_port(int32_t server_port){server_port_=server_port;}
    int32_t server_thread_num(){return server_thread_num_;}
    void set_server_thread_num(int32_t server_thread_num){server_thread_num_=server_thread_num;}
    int32_t max_connection_count(){return max_connection_count_;}
    void set_max_connection_count(int32_t max_connection_count){max_connection_count_=max_connection_count;}
    int32_t max_packet_size(){return max_packet_size_;}
    void set_max_packet_size(int32_t max_packet_size){max_packet_size_=max_packet_size;}
    int32_t heart_beat_interval(){return heart_beat_interval_;}
    void set_heart_beat_interval(int32_t heart_beat_interval){heart_beat_interval_=heart_beat_interval;}
    int32_t max_cache_msg_count(){return max_cache_msg_count_;}
    void set_max_cache_msg_count(int32_t max_cache_msg_cout){max_cache_msg_count_=max_cache_msg_cout;}
    int32_t monitor_server_port(){return monitor_server_port_;}
    void set_monitor_server_port(int32_t monitor_server_port){monitor_server_port_=monitor_server_port;}

    bool monitor_server_on(){return monitor_server_on_;}
    void set_monitor_server_on(bool monitor_server_on){monitor_server_on_=monitor_server_on;}

    int32_t monitor_interval(){return monitor_interval_;}
    void set_monitor_interval(int32_t monitor_interval){monitor_interval_=monitor_interval;}
    int32_t server_reset_operation_hour(){return server_reset_operation_hour_;}
    void set_server_reset_operation_hour(int32_t server_reset_operation_hour){server_reset_operation_hour_=server_reset_operation_hour;}

    int32_t pre_market_operation_hour(){return pre_market_operation_hour_;}
    void set_pre_market_operation_hour(int32_t pre_market_operation_hour){pre_market_operation_hour_=pre_market_operation_hour;}
    int32_t after_market_operation_hour(){return after_market_operation_hour_;}
    void set_after_market_operation_hour(int32_t after_market_operation_hour){after_market_operation_hour_=after_market_operation_hour;}
  private:
    int32_t server_port_;
    int32_t server_thread_num_;
    int32_t max_connection_count_;
    int32_t max_packet_size_;
    int32_t heart_beat_interval_;
    int32_t max_cache_msg_count_;
    int32_t monitor_server_port_;
    bool    monitor_server_on_;
    int32_t monitor_interval_;   //监控执行周期
    int32_t server_reset_operation_hour_; //服务重置时间，关掉服务重起
    int32_t pre_market_operation_hour_;   //开盘前需要做一些事情
    int32_t after_market_operation_hour_; //收盘以后做一些事情
  };
}

#endif
