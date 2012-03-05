
#include "connection.h"
#include "message_factory.h"
#include "service.h"
#include "event_factory.h"
#include "conn_manager.h"

#ifdef _DEBUG
#include <iostream>
#endif // _DEBUG

#ifdef WIN32
#include <windows.h>
#else
#include <pthread.h>
#include <unistd.h>
#endif


void wmdf::SleepTime(int millsecond)
{
#ifdef WIN32
	Sleep(millsecond);
#else
	usleep(1000*millsecond);
#endif
}



//有问题的解决方法
 static boost::mutex reconnMutex;    
wmdf::Connection::Connection(io_service_type& io_service,Service* service,size_t read_buffer_size,size_t max_cache_msg_count/*=20*/):io_service_(io_service),
                                                                    service_(service),
                                                                    send_msg_queue_(),
                                                                    read_buffer_size_(read_buffer_size),
                                                                    connect_state_(INIT),
                                                                    packet_header_len_(sizeof(Message)-sizeof(char)),
                                                                    connect_timer_(),
                                                                    socket_(),
                                                                    connect_timer_count_(0),
                                                                    close_pending_(false),
																																		send_close_(false),
                                                                    ip_address_(),
                                                                    current_message_(),
                                                                    send_pending_(0),
																																		big_msg_(false),
																																		to_read_(0),
																																		read_size_(read_buffer_size),
																																		max_cache_msg_count_(max_cache_msg_count),
                                                                    total_send_size_(0),
																																		current_big_msg_(NULL)
{
}


wmdf::Connection::~Connection(void)
{
	 boost::mutex::scoped_lock lock(reconnMutex);
#ifdef _DEBUG
  //std::cout<<socket_id_<<": destory"<<std::endl;
#endif // _DEBUG
  delete read_buffer_;
  delete packet_header_;
}


void wmdf::Connection::HandleWrite( const error_code& error, size_t bytes_transferred )
{
  if(close_pending_)
    return;
  if(error)
  {
		boost::mutex::scoped_lock lock(mutex_);
		send_pending_--;
		send_close_=true;
		//assert(send_pending_!=0);
		if(send_pending_!=0)
		{
			ResetQueue();
			Close(error);
		}
    return;
  }
  boost::mutex::scoped_lock lock(mutex_);
  send_pending_--;
  total_send_size_ += bytes_transferred;
  //std::cout<<(int)current_message_->product_id<<std::endl;
  service_->OnSendMsg(socket_id_,current_message_.get());
  current_message_.reset();

  if(!send_msg_queue_.empty())
  {
    assert(send_pending_==0);
    current_message_= send_msg_queue_.front();
    send_msg_queue_.pop_front();
    send_pending_++;
    async_write(*socket_,boost::asio::buffer(current_message_.get(),current_message_->msg_len),
      bind(&Connection::HandleWrite,shared_from_this(),
      boost::asio::placeholders::error,
      boost::asio::placeholders::bytes_transferred));

  }

}

void wmdf::Connection::HandleRecvMsg( const error_code& error, size_t bytes_transferred )
{
  if(!error)
  {
		if(!big_msg_){
			read_buffer_->produce(bytes_transferred);
			DecodeMsgHeader(read_buffer_->data(),read_buffer_->end(),bytes_transferred);
			read_buffer_->consume(bytes_transferred);
		}else
			DecodeMsgHeader(read_pos_,read_pos_+bytes_transferred,bytes_transferred);

    AnsyRead();//read next msg
  }else
  {
    Close(error);
  }
}



void wmdf::Connection::HandleConnect( const error_code& error )
{
  if(!error)
  {
    service_->OnConnected(socket_id_,EventFactory::CreateEventMsg(E_CONNECTED,ip_address_+":connected success",MSG_INFO));
    if (connect_timer_count_ != 0)
    connect_timer_->cancel();
  }else{
		ResetQueue();
    Close(error);
  }

}

void wmdf::Connection::HandleClose( const error_code& error )
{
  int error_value=E_UNKNOW;
  switch(error.value()) {
  case boost::asio::error::eof:
    error_value=E_EOF;
    break;
#ifdef _WIN32
  case ERROR_CONNECTION_ABORTED:
#endif
  case boost::asio::error::connection_aborted:
  case boost::asio::error::operation_aborted:
  case boost::asio::error::connection_reset:
    error_value=E_DISCONNECTED;
    break;
  default:
    error_value=E_UNKNOW;
    break;
  }
  service_->OnClose(socket_id_,EventFactory::CreateEventMsg(error_value,ip_address_+":"+error.message(),MSG_ERROR));
}



void wmdf::Connection::Close()
{

	ResetQueue();
  error_code error;
  Close(error);
}


void wmdf::Connection::Close( const error_code& error )
{
	boost::mutex::scoped_lock lock(reconnMutex);
  if (!close_pending_)
  {
		close_pending_ = true;
		send_close_=true;
    // Initiate graceful service_handler closure.
    boost::system::error_code ignored_ec;
		//io_service_.dispatch(boo)
    socket()->shutdown(boost::asio::ip::tcp::socket::shutdown_both, ignored_ec);
    socket()->close();

    // Timer has not been expired, or expired but not dispatched,
    // cancel it even if expired.
    if (connect_timer_count_ != 0)
      connect_timer_->cancel();
    
    HandleClose(error);
  }
}

void wmdf::Connection::Accept()
{
  //BOOST_ASSERT(conn_manager_!=NULL);
  connect_state_=OPEN;
  ip_address_=socket()->remote_endpoint().address().to_string();
  service_->OnConnected(socket_id_,EventFactory::CreateEventMsg(E_CONNECTED,ip_address_+": connected success",MSG_INFO));
  AnsyRead();
}

void wmdf::Connection::SetOption( Option option,bool value )
{
  switch(option)
  {
  case reuse_address:
    socket_->set_option(boost::asio::ip::tcp::socket::reuse_address(value));
    break;
  case no_delay:
    socket_->set_option(boost::asio::ip::tcp::no_delay(value));
    break;
  default:
    //logger_->LogError(E_UNKNOW,"option error");
    assert(0&&"option error");
    break;
  }
}

void wmdf::Connection::AnsyRead()
{
  read_buffer_->resize(read_buffer_size_);
  assert(read_buffer_->size()<read_buffer_size_);
  if (close_pending_)
    return;
	if(!big_msg_)
		read_pos_=read_buffer_->end();

  boost::asio::async_read(*socket_,boost::asio::buffer(read_pos_,read_size_),
    boost::asio::transfer_at_least(1),
    boost::bind(&Connection::HandleRecvMsg,shared_from_this(),
    boost::asio::placeholders::error,
    boost::asio::placeholders::bytes_transferred));
}

bool wmdf::Connection::DecodeMsgHeader(  uint8_t* data,uint8_t* end,size_t& size )
{
  uint8_t* start = data;
  size=end-start;
	read_size_ = read_buffer_size_;
	if(big_msg_)
	{
		if(to_read_<=size)
		{
			assert(to_read_==size);
			assert(current_big_msg_!=NULL);
      if(!close_pending_)
			  service_->OnRecvMsg(socket_id_,current_big_msg_);
			current_big_msg_=NULL;
			big_msg_=false;
			return false;
		}
		to_read_-=size;
		read_pos_+=size;
		if(to_read_<read_buffer_size_)
			read_size_=to_read_;
		return false;
	}
  do 
  {
    if(size<packet_header_len_)//不足包头长
      break;

    while (*data!=MSG_START_TAG&&data!=end)//找到消息的开始位置
    {
      assert(0);
      data++;
    }
    size=end-data;
    if(size<packet_header_len_)break;

    Message* temp_header =(Message*)data;
    int  temp_msg_len_ = temp_header->msg_len;
    if((size_t)temp_msg_len_<packet_header_len_||temp_msg_len_>MAX_MSG_LEN)//invalidate data
    {
			service_->OnEvent(socket_id_,EventFactory::CreateEventMsg(E_DECODE,"invalidate data",MSG_INFO));
      //查找新的包开始位置
      while(*data!=MSG_START_TAG&&data!=end){assert(0);data++;}
      size=end-data;//将其面的去不数据丢弃
      continue;
    }
		if((size_t)temp_msg_len_>read_buffer_size_)//对于大的的消息体进行zero copy 的操作
		{
			to_read_=temp_msg_len_;
			current_big_msg_=MessageFactory::CreateRawMessage(to_read_) ;
			read_pos_=(uint8_t*)current_big_msg_;
			int temp_size = to_read_<size?to_read_:size;
			memcpy(read_pos_,data,temp_size);
			assert(size<=read_buffer_size_*2);
			read_pos_+=size;
			to_read_-=temp_size;
			data+=temp_size;
			if(to_read_==0)
				continue;
			if(to_read_<read_buffer_size_)
				read_size_=to_read_;
			big_msg_=true;
			break;
		}
    if (size<(size_t)temp_msg_len_)//不够包体的长度，需要和下一次的流进行拼接
      break;
    else{
			if(temp_header->check_sum!=MessageFactory::CheckSum(data,packet_header_len_))
			{
				//查找新的包开始位置
				while(*data!=MSG_START_TAG&&data!=end){assert(0);data++;}
				size=end-data;//将其面的去不数据丢弃
				service_->OnEvent(socket_id_,EventFactory::CreateEventMsg(E_DECODE,"invalidate data",MSG_INFO));
				continue;
			}

      Message* newMessage = MessageFactory::CreateRawMessage(temp_msg_len_) ;
      memcpy(newMessage,data,temp_msg_len_);
      if(!close_pending_)
        service_->OnRecvMsg(socket_id_,newMessage);
      data+=temp_msg_len_;
      size=end-data;
    }

  } while (true);
  size=data-start;//已经被解析的数据流
  return true;
}


int wmdf::Connection::Connect( const char* ip,int port,size_t timeout )
{
  connect_state_=OPEN;
  ip_address_=ip;
  boost::asio::ip::tcp::endpoint endpoint( boost::asio::ip::address::from_string(ip), port);
  SetConnectedExpiry(timeout);
  error_code error;
  socket_->connect(endpoint,error);
  if(!error)
  {
		service_->OnConnected(socket_id_,EventFactory::CreateEventMsg(E_CONNECTED,ip_address_+": connected success",MSG_INFO));
    if (connect_timer_count_ != 0)
      connect_timer_->cancel();
  }else{
    Close(error);
		return 0;
  }

  AnsyRead();
	return 1;
}

void wmdf::Connection::SetConnectedExpiry( size_t timeout_seconds )
{
  if (timeout_seconds == 0)
    return;

  BOOST_ASSERT(connect_timer_.get() != 0);

  ++connect_timer_count_;
  connect_timer_->expires_from_now(boost::posix_time::seconds(timeout_seconds));
  connect_timer_->async_wait(boost::bind(&Connection::HandleConnectedTimeout,
    shared_from_this(),
    boost::asio::placeholders::error));
}

void wmdf::Connection::HandleConnectedTimeout( const error_code& error )
{
  --connect_timer_count_;
  //connect关闭或connect_timer_已经被取消
  if(close_pending_||error==boost::asio::error::operation_aborted)
    return;
	ResetQueue();
  if (!error)
  {
    service_->OnEvent(socket_id_,EventFactory::CreateEventMsg(E_CONNECTED,"connected timeout",MSG_ERROR));
     Close(boost::system::error_code(boost::asio::error::timed_out, boost::system::get_system_category()));
  }else{
    Close(error);
  }
}

void wmdf::Connection::Bind()
{
  connect_timer_.reset(new boost::asio::deadline_timer(io_service_));
  socket_.reset(new boost::asio::ip::tcp::socket(io_service_));
}



void wmdf::Connection::SendMsg( MessagePtr msg )
{
  if (close_pending_||send_close_)
    return;

  boost::mutex::scoped_lock lock(mutex_);
	msg->check_sum=MessageFactory::CheckSum((uint8_t*)msg.get(),packet_header_len_);
  if(send_pending_)
  {
    if (send_msg_queue_.size()>(uint32_t)max_cache_msg_count_&&max_cache_msg_count_>0)
    {
			send_msg_queue_.clear();
			current_message_.reset();
			service_->OnEvent(socket_id_,EventFactory::CreateEventMsg(E_BUF_OVERFLOW," data overflow",MSG_INFO));
    }
    send_msg_queue_.push_back(msg);
  }else{
  current_message_=msg;
  assert(send_pending_==0);
  send_pending_++;
  async_write(*socket_,boost::asio::buffer(current_message_.get(),current_message_->msg_len),
    bind(&Connection::HandleWrite,shared_from_this(),
    boost::asio::placeholders::error,
    boost::asio::placeholders::bytes_transferred));
  }
}

void wmdf::Connection::Init()
{
  packet_header_=new uint8_t[packet_header_len_];
  read_buffer_= new IoBuffer(read_buffer_size_*MAX_PACKET_COUNT);
	read_pos_=read_buffer_->data();
}

void wmdf::Connection::ResetQueue()
{
	boost::mutex::scoped_lock lock(mutex_);
	
	send_msg_queue_.clear();
	current_message_.reset();
}







