#ifndef WIND_NET_CLIENT_
#define WIND_NET_CLIENT_

#include "net_module.h"
#include "service.h"
#include "message_factory.h"

namespace wmdf{

NET_API class Client
{
public:
  //max_packet_size  最大包长度，当接收数据包长度大于它时，认为此包是非法数据包直接忽略
  //max_buf_msg 最大接收消息缓冲数，当累计的消息包个数大于max_buf_msg时，缓冲区中的全部消息将被清除
  //has_active_heart 是否主动回应心跳包（本client一直对外发送心跳包）
  //reconnect 断开后是否重连
  //thread_num 客服端拥有的线程数
  NET_API Client(int max_packet_size,int max_buf_msg, int thread_num=1);
  NET_API ~Client();
	NET_API void Init(int client_type=0);
  NET_API int Connect(const char* ip_address,int port,size_t timeout=0);
  NET_API void SendMsg(Message* msg);
  NET_API Message* RecvMsg();
  NET_API void Close();

  NET_API void SetCloseCallback(CloseCallback callback);
  NET_API void SetEventCallback(EventCallback callback);
	NET_API void SetRecvCallback(RecvCallback callback);
  NET_API void SetState(void* state);

private:
  class ClientImpl;
  ClientImpl* impl_;

};
}//end namespace
#endif

