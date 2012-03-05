#ifndef WIND_NET_SERVER_IMPL_
#define WIND_NET_SERVER_IMPL_

#include <boost/asio.hpp>
#include <boost/thread.hpp>

#include "server.h"
#include "conn_manager.h"
#include "service.h"

namespace wmdf
{
  class Connection;
  class ConnManager;
}
class wmdf::Server::ServerImpl
  :public wmdf::Service
{
  typedef boost::shared_ptr<boost::thread> thread_ptr;
  typedef boost::scoped_ptr<boost::asio::io_service::work> work_ptr;
  typedef boost::shared_ptr<wmdf::Connection> ConnectionPtr;
  typedef wmdf::ConnManager* ConnectionManagerPtr;
  typedef boost::shared_ptr<Message> MessagePtr;
  typedef boost::shared_ptr<boost::asio::deadline_timer> timer_ptr;
public:
  ServerImpl(const char* ip_address,int port,int thread_num,int max_connection_count,int max_packet_size,int max_cache_msg_count):port_(port),
      max_thread_num_(thread_num),
      max_connection_count_(max_connection_count),
      max_packet_size_(max_packet_size),
	  max_cache_msg_count_(max_cache_msg_count),
      io_service_(),
      statrted_(false),
      heartbeat_timer_(new boost::asio::deadline_timer(io_service_)),
      acceptor_(io_service_),
      endpoint_(boost::asio::ip::address::from_string(ip_address), port),
      reuse_address_(true),
      no_delay_(false),
      connected_callback_(NULL),
      close_callback_(NULL),
      recv_callback_(NULL),
      event_callback_(NULL)
  {
    conn_manager_=ConnManager::GetInstance(max_connection_count_);
  }
  virtual ~ServerImpl(void)
  {

  }

  int Start();
  void Stop();
  void SetOption(Option option,bool  value);
  void SendMsg(int socket_id,Message* msg);
  void BroadcastMsg(int group_id,Message* msg);
	void CloseConnection(int socket_id);

  ConnectionManagerPtr GetConnManager(){return conn_manager_;}

  void SetConnectedCallback(ConnectedCallback callback);
  void SetCloseCallback(CloseCallback callback);
  void SetEventCallback(EventCallback callback);
  void SetRecvCallback(RecvCallback callback);
  void SetParent(wmdf::Server* parent){parent_=parent;}
  void SetState(void* state) {state_ = state;}
private:
  void Run();
	void AcceptRun();
  //void AcceptOne();
  //void HandleAccept(const boost::system::error_code& e,const ConnectionPtr& handler);

  virtual void OnRecvMsg( int socket_id, Message* msg );
  virtual void OnConnected( int socket_id, EventMsg* event_msg );
  virtual void OnSendMsg( int socket_id, Message* msg );
  virtual void OnClose( int socket_id, EventMsg* event_msg );
  virtual void OnEvent( int socket_id, EventMsg* event_msg );

  void HeartbeatHandle(const boost::system::error_code& e);
  void SetHeartbeatExpiry(size_t timeout_seconds);

private:
  wmdf::Server* parent_;
  void* state_;
  thread_ptr serve_thread_;
	thread_ptr accept_thread_;

  boost::asio::io_service io_service_;
  // The work that keeps the io_services running.
  work_ptr work_;
  boost::thread_group threads_;
  bool statrted_;
  int max_packet_size_;
  int port_;
  int max_thread_num_;
  int max_connection_count_;
  int max_cache_msg_count_;
  timer_ptr heartbeat_timer_;

  //用于标记当前heartbeat_timer_提交的任务数
  //size_t heartbeat_timer_count_;
  size_t seconds_;
  boost::asio::ip::tcp::acceptor acceptor_;
  boost::asio::ip::tcp::endpoint endpoint_;
  ConnectionManagerPtr conn_manager_;
  //
  //option set
  bool no_delay_;
  bool reuse_address_;

  ConnectedCallback connected_callback_;
  CloseCallback close_callback_;
  RecvCallback recv_callback_;
  EventCallback event_callback_;
};

#endif
