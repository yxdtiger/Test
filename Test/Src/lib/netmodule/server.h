#ifndef WIND_NET_SERVER_
#define WIND_NET_SERVER_

#include "net_module.h"
#include "service.h"
#include "message_factory.h"
#include "conn_manager.h"

namespace wmdf{

class ConnManager;

NET_API class Server
{
public:
  //port 监听端口号
  //thread_num io_service的启动线程数
  //max_connection_count 最大连接数
  //interval 心跳包间隔时间（单位：秒（S））
  NET_API Server(const char* ip_address,int port,int thread_num,int max_connection_count,int max_packet_size=1024,int max_cache_msg_count=20);
  NET_API ~Server(void);

  NET_API int Start();
  NET_API void Stop();
  NET_API void SetOption(Option option,bool  value);
  NET_API ConnManager* GetConnManager();

  NET_API void SendMsg(int socket_id,Message* msg);
  NET_API void BroadcastMsg(int group_id,Message* msg);

  NET_API void SetConnectedCallback(ConnectedCallback callback);
  NET_API void SetCloseCallback(CloseCallback callback);
  NET_API void SetRecvCallback(RecvCallback callback);
  NET_API void SetEventCallback(EventCallback callback);
  NET_API void SetState(void* state);
	void CloseConnection(int socket_id);

private:
  class ServerImpl;
  ServerImpl* impl_;
};

}//end namespace
#endif
