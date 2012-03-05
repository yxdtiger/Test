#ifndef WIND_NET_GROUP_CONN_MANAGER_
#define WIND_NET_GROUP_CONN_MANAGER_

#include "net_module.h"

namespace wmdf{

class Connection;

//实现单例模式
NET_API class ConnManager
{
  friend class Server;
public:
  //由于此对象在程序的运行期间不用析构，所以不用考虑内存释放的操作
  NET_API static ConnManager* GetInstance(int max_connection)
  {
    if(instance_==NULL)
      instance_ = new ConnManager(max_connection);
    return instance_;
  }

  NET_API ~ConnManager(void);
  //线程不安全的
  NET_API int AddConnectionGroup();
  //线程不安全的
  NET_API void AddConnection(int group_id,int socket_id);
  NET_API void RemoveConnection(int group_id,int socket_id);
	NET_API int GetConnectionCount(int group_id);
  NET_API void RemoveConnection(int socket_id);
  //返回值为connection的socket_id
  int AddConnection(Connection* connection);
	void CloseConnection(int socket_id);
  NET_API void Close();
  //由于需要频繁操作，为提高效率直接内联实现
  NET_API Connection* GetConnection(int pos)
  {
    return connections_[pos];
  }

private:
  //只有server可以访问
  void SendMsg(int socket_id,Message* msg);
  void BroadcastMsg(int group_id,Message* msg);
private:
  ConnManager(int max_connection);
  class ConnGroupManagerImpl;
  ConnGroupManagerImpl* impl_;
  Connection** connections_;
  static ConnManager* instance_;
};

}//end namespace
#endif

