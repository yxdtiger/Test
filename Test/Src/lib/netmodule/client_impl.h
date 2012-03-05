#ifndef WIND_NET_CLIENT_IMPL_
#define WIND_NET_CLIENT_IMPL_



#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include "pipe.h"
#include "client.h"
#include "connection.h"
#include "service.h"

class wmdf::Client::ClientImpl
  :public wmdf::Service
{
public:
  ClientImpl(int max_packet_size,int max_cache_msg_count,int thread_num=1):
      max_thread_num_(thread_num),
      max_packet_size_(max_packet_size),
	  max_cache_msg_count_(max_cache_msg_count),
      conn_ptr_(),
      work_(),
      statrted_(false),
      no_delay_(false),
      reuse_address_(true),
      close_callback_(NULL),
      event_callback_(NULL),
			recv_callback_(NULL),
			connection_type_(0)
  {
  }
  ~ClientImpl(void);
  //TODO：可能需要添加回调函数,max_buf_msg需要用来检测buf的溢出
	void Init(int connection_type=0);
  int Connect(const char* ip_address,int port,size_t timeout=0);
  void SendMsg(Message* msg);
  Message* RecvMsg();
  void Close();
public:
  void SetCloseCallback( CloseCallback callback );
  void SetEventCallback( EventCallback callback );
	void SetRecvCallback( RecvCallback callback );
  void SetParent(wmdf::Client* parent){parent_=parent;}
  void SetState(void* state){state_ = state;}
private:
    typedef boost::shared_ptr<boost::thread> thread_ptr;
    typedef boost::shared_ptr<wmdf::Connection> ConnectionPtr;
    typedef boost::scoped_ptr<boost::asio::io_service::work> work_ptr;
    typedef boost::shared_ptr<Message> MessagePtr;
		typedef wmdf::ypipe_t<Message*,256>MsgQueue;

    void Run();
    virtual void OnRecvMsg( int socket_id, Message* msg );
    virtual void OnConnected( int socket_id, EventMsg* event_msg );
    virtual void OnSendMsg( int socket_id, Message* msg );
    virtual void OnClose( int socket_id, EventMsg* event_msg );
    virtual void OnEvent( int socket_id, EventMsg* event_msg );
		void Clear();

    
private:
    wmdf::Client* parent_;
    void* state_;
    bool reconnect_;
    bool statrted_;
    bool has_active_heart;
    thread_ptr serve_thread_;
    boost::asio::io_service io_service_;
    thread_ptr client_thread_;
    boost::thread_group threads_;
    int max_thread_num_;
    size_t max_packet_size_;
	  int max_cache_msg_count_;
    ConnectionPtr conn_ptr_;
    work_ptr work_;
    boost::asio::ip::tcp::endpoint endpoint_;
    //option set
    bool no_delay_;
    bool reuse_address_;
    MsgQueue msg_queue_;
    boost::mutex mutex_;
		int connection_type_;//0 为connection 1 为relay_connection

    CloseCallback close_callback_;
    EventCallback event_callback_;
		RecvCallback recv_callback_;
};

#endif
