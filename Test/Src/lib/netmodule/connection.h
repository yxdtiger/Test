#ifndef WIND_NET_CONNECTION_
#define WIND_NET_CONNECTION_

#include <deque>
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/asio.hpp>
#include <boost/enable_shared_from_this.hpp>

#include "net_module.h"
#include "io_buffer.h"




namespace wmdf{

void SleepTime(int millsecond);

class Service;
class ConnManager;

class Connection
  :private boost::noncopyable,
  public boost::enable_shared_from_this<Connection>
{
  enum ConnectState{INIT,OPEN,CONNECTED,CLOSE};
protected:
  typedef boost::shared_ptr<boost::asio::ip::tcp::socket> socket_type;
  typedef boost::asio::io_service io_service_type;
  typedef boost::system::error_code error_code;
  typedef boost::shared_ptr<Message> MessagePtr;
  typedef std::deque<MessagePtr> MessageQueue;
  typedef boost::shared_ptr<boost::asio::deadline_timer> timer_ptr;
  typedef boost::mutex::scoped_lock lock;
public:
  Connection(io_service_type& io_service,Service* service,size_t read_buffer_size=1024,size_t max_cache_msg_count=20);
  virtual ~Connection(void);



public:// inline method
  socket_type& socket(){return socket_;}

  int socket_id(){return socket_id_;}

  void set_socket_id(int socket_id){ socket_id_=socket_id;}

  size_t heartbeat_miss_count(){return heartbeat_miss_count_;}

  void SetOption(Option option,bool  value);

  void SetConnManager(ConnManager* manager){conn_manager_=manager;}

  //获取发送的总字节数
  int64_t total_send_size(){return total_send_size_;}
  //获取发送缓存队列的消息个数
  int32_t msg_in_queue(){return (int32_t)send_msg_queue_.size();}

public:
  void Init();
  void Accept();
  void Bind();
  void AnsyRead();
  void SendMsg(MessagePtr msg);
  int Connect(const char* ip,int port,size_t timeout=0);
  void Close() ;
  void Close(const error_code& error);

private:
  void HandleWrite(const error_code& error, size_t bytes_transferred);
  void HandleRecvMsg(const error_code& error, size_t bytes_transferred);
  void HandleConnect(const error_code& error);
  void HandleClose(const error_code& error);
  void HandleConnectedTimeout(const error_code& error);
  void HandleReadMsgBody( const error_code& error );
  int  HandleReadVersion(size_t bytes_transferred);
  void HandleSendVersion(const error_code& error, size_t bytes_transferred);
  void SetConnectedExpiry(size_t timeout_seconds);

	void ResetQueue();
	void GetBuffer();
protected:
	virtual bool DecodeMsgHeader( uint8_t* data,uint8_t* end,size_t& size );
protected:
  //connect owner(server or client)
  Service* service_;
  //only server owner used,when client conn_maager is null
  ConnManager* conn_manager_;
  static const int MAX_PACKET_COUNT=10;
  //当为server时，socket_id由conn_manger分配，client是socket_id为0
  uint32_t socket_id_;
  bool close_pending_;
  // send_pending 只能为1或0，有他确保发送包的顺序
  int send_pending_;
	bool send_close_;
  MessagePtr current_message_;
  boost::mutex mutex_;
  socket_type socket_;
  io_service_type& io_service_;
  //发送消息的暂存队列，当io_service中的消息达到一定数值时才会将消息放入此队列中
  MessageQueue send_msg_queue_;
  /// Buffer for incoming data.
  IoBuffer* read_buffer_;
  //每次从socket缓冲区的获取的数据包的最大值，其中他也是最大数据的值
  size_t read_buffer_size_;
  std::string ip_address_;
  ConnectState connect_state_;
  size_t packet_header_len_;
  uint8_t* packet_header_;
  Message* current_big_msg_;
  timer_ptr connect_timer_;
  //用于标记当前connect_timer提交的任务数，有利于任务取消的选择
  size_t connect_timer_count_;
  size_t heartbeat_miss_count_;
  int max_cache_msg_count_;
	uint8_t* read_pos_;
	size_t to_read_;
	bool big_msg_;
	size_t read_size_;
  volatile int64_t total_send_size_;
	const static int MAX_MSG_LEN=1024*1024*100;
  const static uint8_t MSG_START_TAG=01;
};



}//end namespace
#endif


