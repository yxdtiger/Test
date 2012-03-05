#ifndef SUBSCRIBER_H
#define SUBSCRIBER_H

#include <string>
#include <vector>
#include "../include/wmdf_api.h"

namespace wmdf
{
  class ProtocolDescription;
  class Connection;

  class Subscriber
  {
  public:
    Subscriber(int32_t socketId,int32_t user_id,std::string& ip_address,time_stamp_t connected_time);
    virtual ~Subscriber(void);
    void AddSubscribedProduct(ProtocolDescription* protocol_description);
    int32_t user_id() {return user_id_;}
    void set_user_id(int32_t user_id) {user_id_ = user_id;}
    time_stamp_t connected_time() {return connected_time_;}
		int32_t socket_id(){return socket_id_;}
    std::string ip_address(){return ip_address_;}
    bool to_be_kicked(){return to_be_kicked_;}
    void set_to_be_kicked(bool to_be_kicked){to_be_kicked_=to_be_kicked;}
    int64_t get_send_size();
    int32_t get_msg_in_queue();
    void set_connection(Connection* connection){connection_ = connection;}
    int32_t subscribed_product_count(){return (int32_t)subscribed_products_.size();}
    ProtocolDescription* subscribed_products(int32_t index){return subscribed_products_[index];}
  private:
    int32_t socket_id_;  //订阅者的socketid
    int32_t user_id_;    //订阅的用户ID
    time_stamp_t connected_time_; //连接的时间
    std::string ip_address_; //订阅用户的IP地址
    std::vector<ProtocolDescription*> subscribed_products_;
    Connection* connection_;
    bool to_be_kicked_;
  };
}

#endif
