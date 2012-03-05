#ifndef DATA_RECEIVER_H_
#define DATA_RECEIVER_H_

#include <string>
#include <vector>
#include "client.h"
#include "net_module.h"
#include "../include/wmdf_api.h"

namespace wmdf
{
  class ProductSource;
  class MdfMessage;
  class ProtocolProcessor;

  class IClientCallback
  {
  public:
    IClientCallback();
    virtual ~IClientCallback();
    virtual void OnRecvMsg(Message* msg)=0;
    virtual void OnCloseCallback(EventMsg* args)=0;
    virtual void OnEventCallback(EventMsg* args)=0;
    virtual void ResetContext() = 0;
  };


  class FeedClient
  {
  public:
    FeedClient(int32_t client_type);
    virtual ~FeedClient();
    void SetClientCallback(IClientCallback* callback);
    bool Connect(const std::string& server_address, const int32_t& server_port, const int32_t retry_times,int32_t timeout);
    void DisConnect();
    void SendMsg(Message* msg);
    void DoRecvCallback(Message* msg);
    void DoCloseCallback(EventMsg* msg);
    void DoEventCallback(EventMsg* msg);
    bool is_connected(){return is_connected_;}

    std::string server_address(){return server_address_;}
    int32_t server_port(){return server_port_;}
  private:

    Client* client_;  //客户端连接句柄
    volatile bool is_connected_;
    int32_t max_packet_size_;   //最大包大小
    int32_t max_buf_msg_;       //消息缓存个数
    int32_t thread_num_;        //
    int32_t client_type_;
    std::string server_address_;
    int32_t server_port_;
    IClientCallback* client_callback_;
  };
}

#endif
