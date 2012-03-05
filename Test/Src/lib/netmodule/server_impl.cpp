#include "server_impl.h"
#include "connection.h"
#include "conn_manager.h"
#include "message_id_def.h"
#include "event_factory.h"

int wmdf::Server::ServerImpl::Start()
{
  assert(connected_callback_!=NULL);
  assert(close_callback_!=NULL);
  assert(recv_callback_!=NULL);
  assert(event_callback_!=NULL);

  if(statrted_)
    return 0;
  statrted_=true;
  //conn_manager_->AddConnectionGroup();//其中0组管理当前连接的所有对象
  acceptor_.open(endpoint_.protocol());
  acceptor_.set_option(boost::asio::ip::tcp::acceptor::reuse_address(reuse_address_));
  //add by aza.tang begin 2011/12/16   
  //disable Nagle
  //acceptor_.set_option(boost::asio::ip::tcp::no_delay(true));

//   //disable keepalive
//   asio::socket_base::keep_alive keepAlive(false);
//   acceptor_.set_option(keepAlive);

  //end;
	boost::system::error_code error;
  acceptor_.bind(endpoint_,error);
	if(error)
	{
		char message[80];
		memcpy(message,error.message().c_str(),error.message().length()>79?79:error.message().length());
		OnEvent(-1,wmdf::EventFactory::CreateEventMsg(E_OPEN,message,MSG_ERROR));
		return -1;
	}
  acceptor_.listen();
  //SetHeartbeatExpiry(seconds_);
	//acceptor_.accept()
  //AcceptOne();
  
  serve_thread_.reset(new boost::thread(boost::bind( &ServerImpl::Run, this )));
	accept_thread_.reset(new boost::thread(boost::bind( &ServerImpl::AcceptRun, this )));
  return 1;
}

void wmdf::Server::ServerImpl::Stop()
{
	if(statrted_){
		acceptor_.close();
		conn_manager_->Close();
		statrted_=false;
		state_ = NULL;
	}
}

void wmdf::Server::ServerImpl::Run()
{
  work_.reset(new boost::asio::io_service::work(io_service_));
  for(int i=0;i<max_thread_num_;i++)
  {
    threads_.create_thread( boost::bind(&boost::asio::io_service::run,&io_service_));
  }
  threads_.join_all();
  statrted_=false;
}




void wmdf::Server::ServerImpl::SetOption( Option option,bool value )
{
  switch(option)
  {
  case no_delay:
    no_delay_=value;
  case reuse_address:
    reuse_address_=value;
  default:
    //logger_->LogError(E_UNKNOW,"opetion error");
    assert(0&&"option error");
  }
}

void wmdf::Server::ServerImpl::SetConnectedCallback( ConnectedCallback callback )
{
  connected_callback_=callback;
}

void wmdf::Server::ServerImpl::SetCloseCallback( CloseCallback callback )
{
  close_callback_=callback;
}

void wmdf::Server::ServerImpl::SetRecvCallback( RecvCallback callback )
{
  recv_callback_=callback;
}

void wmdf::Server::ServerImpl::SetEventCallback( EventCallback callback )
{
  event_callback_=callback;
}

void wmdf::Server::ServerImpl::SendMsg( int socket_id,Message* msg )
{
  conn_manager_->SendMsg(socket_id,msg);
}

void wmdf::Server::ServerImpl::BroadcastMsg( int group_id,Message* msg )
{
  conn_manager_->BroadcastMsg(group_id,msg);
}


void wmdf::Server::ServerImpl::OnRecvMsg( int socket_id, Message* msg )
{
  //进行消息处理
  //TODO:添加回调函数
  MsgCallbackArgs args;
  args.handle = parent_;
  args.socket_id = socket_id;
  args.msg = msg;
  args.state = state_;
  recv_callback_(args);
}

void wmdf::Server::ServerImpl::OnConnected( int socket_id, EventMsg* event_msg )
{
  //将对象加入到conn_manager的0组
  //TODO:添加回调函数
  EventCallbackArgs args;
  args.handle = parent_;
  args.socket_id = socket_id;
  args.event_msg = event_msg;
  args.state = state_;
  connected_callback_(args);
}

void wmdf::Server::ServerImpl::OnSendMsg( int socket_id, Message* msg )
{
   //TODO：添加流量统计和数据包等工作
  //throw std::exception("The method or operation is not implemented.");
}

void wmdf::Server::ServerImpl::OnClose( int socket_id, EventMsg* event_msg )
{
  //TODO：进行连接器的处理操作
  conn_manager_->RemoveConnection(socket_id);
  EventCallbackArgs args;
  args.handle = parent_;
  args.socket_id = socket_id;
  args.event_msg = event_msg;
  args.state = state_;
  close_callback_(args);
}

void wmdf::Server::ServerImpl::OnEvent( int socket_id, EventMsg* event_msg )
{
  EventCallbackArgs args;
  args.handle = parent_;
  args.socket_id = socket_id;
  args.event_msg = event_msg;
  args.state = state_;
  event_callback_(args);
}

void wmdf::Server::ServerImpl::HeartbeatHandle(const boost::system::error_code& e)
{
  //检测其是否正常
  //发送心跳包
  Message* heartbeat_msg = wmdf::MessageFactory::CreateMessage(0);
  heartbeat_msg->product_id=HEART_PACKET;
  BroadcastMsg(0,heartbeat_msg);
  
}

void wmdf::Server::ServerImpl::SetHeartbeatExpiry( size_t timeout_seconds )
{
  if (timeout_seconds == 0)
    return;

  BOOST_ASSERT(heartbeat_timer_.get() != 0);

  //++heartbeat_timer_count_;
  heartbeat_timer_->expires_from_now(boost::posix_time::seconds(timeout_seconds));
  heartbeat_timer_->async_wait(boost::bind(&ServerImpl::HeartbeatHandle,
    this,
    boost::asio::placeholders::error));
}

void wmdf::Server::ServerImpl::AcceptRun()
{
	while(statrted_)
	{
		ConnectionPtr connect=ConnectionPtr(new wmdf::Connection(io_service_,this,max_packet_size_,max_cache_msg_count_));
		connect->Init();
		int socket_id = conn_manager_->AddConnection(connect.get());
		if(socket_id==-1)//没有找到合适的位置
		{
			OnEvent(socket_id,wmdf::EventFactory::CreateEventMsg(E_CONNECTED,"can't find socket id ,the max socket",MSG_INFO));
			connect.reset();
			SleepTime(1000);
			continue;
		}
		connect->set_socket_id(socket_id);
		connect->Bind();
		acceptor_.accept(connect->socket()->lowest_layer());
		connect->Accept();
	}
}

void wmdf::Server::ServerImpl::CloseConnection( int socket_id )
{
		conn_manager_->CloseConnection(socket_id);
}
