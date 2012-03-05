#ifndef FEED_INFO_H
#define FEED_INFO_H

#include <string>
#include <vector>
#include "product_target.h"


namespace wmdf
{
  class FeedInfo:public Persistable
  {
  public:
    FeedInfo(void);
    virtual ~FeedInfo(void);

    virtual bool LoadState(Memento* m);
    virtual bool SaveState(Memento* m);   

    std::string source_ip() const {return source_ip_;}
    void set_source_ip(std::string src_ip){source_ip_=src_ip;}

    int32_t source_port() const {return source_port_;}
    void set_source_port(int32_t src_port){source_port_=src_port;}   

    int32_t codec_type() const {return codec_type_;}
    void set_codec_type(int32_t codec){codec_type_=codec;}  
    
    std::string user_name() const {return user_name_;}
    void set_user_name(std::string name){user_name_=name;}

    std::string password() const {return password_;}
    void set_password(std::string keyword){password_=keyword;}

    ProductTarget* targets(int32_t index){return targets_[index];}
    int32_t target_info_count(){return (int32_t)targets_.size();}
  private:    
    std::string source_ip_;
    int32_t source_port_;  
    int32_t codec_type_;
    std::string user_name_;
    std::string password_;
    std::vector<ProductTarget*> targets_;
  };
}

#endif
