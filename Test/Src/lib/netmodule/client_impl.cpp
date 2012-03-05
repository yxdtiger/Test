#include "client_impl.h"

#include <boost/bind.hpp>
#include "client.h"
#include "relay_connection.h"
#include "event_factory.h"
#include "error.h"


int wmdf::Client::ClientImpl::Connect( const char* ip_address,int port,size_t timeout/*=0*/ )
{
  boost::mutex::scoped_lock lock(mutex_);
  assert(close_callback_!=NULL);
  assert(event_callback_!=NULL);
	Connection *connection;
	if(connection_type_==0)
		connection = new (std::nothrow) Connection(io_service_,this,max_packet_size_,max_cache_msg_count_);
	if(connection_type_==1)
		connection = new (std::nothrow) RelayConnection(io_service_,this,max_packet_size_,max_cache_msg_count_);
  alloc_assert (connection);

  conn_ptr_.reset(connection);
  conn_ptr_->Init();
  conn_ptr_->Bind();
	conn_ptr_->set_socket_id(0);
	statrted_=true;
  int ret = conn_ptr_->Connect(ip_address,port,timeout);
  if (ret == -1)
    conn_ptr_.reset();
  return ret;
}

void wmdf::Client::ClientImpl::SendMsg( Message* msg )
{
  MessagePtr msg_ptr(msg);
  conn_ptr_->SendMsg(msg_ptr);
}

Message* wmdf::Client::ClientImpl::RecvMsg()
{
  Message* msg=NULL;
	while(1)
	{
		if(msg_queue_.read(&msg))
			return msg;
		else
			SleepTime(1);
	}
	return msg;
}

void wmdf::Client::ClientImpl::Close()
{
	if(statrted_){
		boost::mutex::scoped_lock lock(mutex_);
		Clear();
		conn_ptr_->Close();
		conn_ptr_.reset();
		statrted_=false;
	}
}

void wmdf::Client::ClientImpl::Run()
{
  work_.reset(new boost::asio::io_service::work(io_service_));
  for(int i=0;i<max_thread_num_;i++)
  {
    threads_.create_thread( boost::bind(&boost::asio::io_service::run,&io_service_));
  }
  threads_.join_all();
	
}


void wmdf::Client::ClientImpl::OnRecvMsg( int socket_id, Message* msg )
{
	if(recv_callback_!=NULL)
  {
    MsgCallbackArgs args;
    args.handle = parent_;
    args.socket_id = 0;
    args.msg = msg;
    args.state = state_;
		recv_callback_(args);
		return;
	}
  //TODO:进行消息的处理
  if(msg_queue_.size()>max_cache_msg_count_&&max_cache_msg_count_>0)
  {
		int size = msg_queue_.size();
    Clear();
		OnEvent(socket_id,wmdf::EventFactory::CreateEventMsg(E_BUF_OVERFLOW,"recv data overflow",MSG_INFO));
  }
  msg_queue_.write(msg,false);
	msg_queue_.flush();
}

void wmdf::Client::ClientImpl::OnConnected( int socket_id, EventMsg* event_msg )
{
  EventCallbackArgs args;
  args.handle = parent_;
  args.socket_id = 0;
  args.event_msg = event_msg;
  args.state = state_;
  event_callback_(args);
}

void wmdf::Client::ClientImpl::OnSendMsg( int socket_id, Message* msg )
{
  //TODO：添加流量统计和数据包等工作
  //throw std::exception("The method or operation is not implemented.");
}

void wmdf::Client::ClientImpl::OnClose( int socket_id, EventMsg* event_msg )
{
  //TODO:在回调函数中进行关闭的相关处理可直接处理重连的工作
  EventCallbackArgs args;
  args.handle = parent_;
  args.socket_id = 0;
  args.event_msg = event_msg;
  args.state = state_;
  close_callback_(args);
}

void wmdf::Client::ClientImpl::OnEvent( int socket_id, EventMsg* event_msg )
{
  //TODO:在回调函数中处理事件消息
  EventCallbackArgs args;
  args.handle = parent_;
  args.socket_id = 0;
  args.event_msg = event_msg;
  args.state = state_;
  event_callback_(args);
}



void wmdf::Client::ClientImpl::SetCloseCallback( CloseCallback callback )
{
  close_callback_=callback;
}

void wmdf::Client::ClientImpl::SetEventCallback( EventCallback callback )
{
  event_callback_=callback;
}

void wmdf::Client::ClientImpl::Init(int connection_type)
{
	if(!statrted_)
	{
		client_thread_.reset(new boost::thread(boost::bind(&ClientImpl::Run,this)));
		statrted_=true;
	}
	connection_type_ = connection_type;
}

wmdf::Client::ClientImpl::~ClientImpl( void )
{
  boost::mutex::scoped_lock lock(mutex_);
	io_service_.stop();
  state_ = NULL;
}

void wmdf::Client::ClientImpl::Clear()
{
	Message* msg;
	while(msg_queue_.size()>0)
	{
		if(msg_queue_.read(&msg))
			free(msg);
	}
	msg=NULL;
}

void wmdf::Client::ClientImpl::SetRecvCallback( RecvCallback callback )
{
	recv_callback_=callback;
}


