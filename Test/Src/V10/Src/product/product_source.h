#ifndef PRODUCT_SOURCE_H
#define PRODUCT_SOURCE_H

#include <string>
#include <vector>
#include "persistable.h"
#include "../include/wmdf_api.h"

namespace wmdf
{
  class Memento;
  class ProtocolDescription;
  
  class MarketTime: public Persistable
  {
  public:
    MarketTime();
    ~MarketTime();
    virtual bool LoadState(Memento* m);
    virtual bool SaveState(Memento* m);
    int32_t begin_time() {return begin_time_;}
    void set_begin_time(int32_t begin_time){begin_time_ = begin_time;}
    int32_t end_time() {return end_time_;}
    void set_end_time(int32_t end_time){end_time_ = end_time;}
  private:
    int32_t begin_time_;
    int32_t end_time_;
  };

  class BackupSource: public Persistable
  {
  public:
    BackupSource();
    ~BackupSource();
    virtual bool LoadState(Memento* m);
    virtual bool SaveState(Memento* m);
    std::string ip_address() const {return ip_address_;}
    void set_ip_address(std::string ip_address){ip_address_=ip_address;}
    int32_t port() const {return port_;}
    void set_port(int32_t port){port_=port;}
    std::string user_name() const {return user_name_;}
    void set_user_name(std::string user_name){user_name_=user_name;}
    std::string pass_word() const {return pass_word_;}
    void set_pass_word(std::string pass_word){pass_word_=pass_word;}
  private:
    std::string ip_address_;
    int32_t port_;
    std::string user_name_;
    std::string pass_word_;
  };

  class ProductSource: public Persistable
  {
  public:
    ProductSource(void);
    virtual ~ProductSource(void);
    virtual bool LoadState(Memento* m);
    virtual bool SaveState(Memento* m);

    bool enabled() const{return enabled_;}
    void set_enabled(bool enabled){enabled_=enabled;}
    int16_t product_id() const{return product_id_;}
    void set_product_id(int16_t product_id){product_id_=product_id;}

    std::string ip_address() const {return ip_address_;}
    std::string in_use_ip_address();
    void set_ip_address(std::string ip_address){ip_address_=ip_address;}
    int32_t port() const {return port_;}
    int32_t in_use_port() ;
    void set_port(int32_t port){port_=port;}
    std::string user_name() const {return user_name_;}
    std::string in_use_user_name();
    void set_user_name(std::string user_name){user_name_=user_name;}
    std::string pass_word() const {return pass_word_;}
    std::string in_use_pass_word();
    void set_pass_word(std::string pass_word){pass_word_=pass_word;}
    CodecType codec_type() const {return codec_type_;}
    void set_codec_type(CodecType codec_type){codec_type_=codec_type;}

    int32_t retry_times() const {return retry_times_;}
    void set_retry_times(int32_t retry_times){retry_times_=retry_times;}

    int32_t timeout() const {return timeout_;}
    void set_timeout(int32_t timeout){timeout_=timeout;}

    std::string description() const {return description_;}
    void set_description(std::string description){description_=description;}
    int32_t subscribed_products_count() {return (int32_t)subscribed_products_.size();}
    ProtocolDescription* subscribed_products(int32_t index) {return subscribed_products_[index];}
    ProtocolDescription* FindSubscribedProduct(int16_t product_id);
    bool IsSubscribedProduct(int16_t product_id);
 

    int32_t market_time_count() {return (int32_t)market_times_.size();}
    MarketTime* market_times(int32_t index) {return market_times_[index];}
    int32_t backup_source_count() {return (int32_t)backup_sources_.size();}
    BackupSource* backup_sources(int32_t index) {return backup_sources_[index];}
    
    int32_t which_is_in_use() const {return which_is_in_use_;}
    void set_which_is_in_use(int32_t which_is_in_use){which_is_in_use_=which_is_in_use;}

    void add_market_time(MarketTime* market_time) {market_times_.push_back(market_time);}
    void add_backup_source(BackupSource* backup_source) {backup_sources_.push_back(backup_source);}
    void add_subscribed_product(ProtocolDescription* subscribed_product) {subscribed_products_.push_back(subscribed_product);}
    void delete_market_time(int32_t begin_time, int32_t end_time);
    void delete_market_time(MarketTime* ptr_market_time);
    void delete_backup_source(std::string ip_addr, int32_t port);
    void delete_backup_source(BackupSource* ptr_backup_source);
    void delete_subscribed_product(int32_t product_id, int8_t major_version, int8_t minor_version, CodecType codec);
    void delete_subscribed_product(ProtocolDescription* ptr_subscribed_product);
  public:
    ProductSource& operator= (const ProductSource& rhs);
  private:
    bool enabled_;
    int16_t product_id_;
    std::string ip_address_;   //如果是文件回放，则把ip Address设置为REPLAY_SOURCE;
    int32_t port_;             //从文件读取 port设为timeSpan
    std::string user_name_;
    std::string pass_word_;
    CodecType codec_type_;
    int32_t retry_times_;
    int32_t timeout_;
    std::string description_;
    std::vector<ProtocolDescription*> subscribed_products_;  //relay 时会用到
    std::vector<MarketTime*> market_times_;                //市场运行的时间
    std::vector<BackupSource*> backup_sources_;            //备源地址
    int32_t which_is_in_use_;

  };
}

#endif
