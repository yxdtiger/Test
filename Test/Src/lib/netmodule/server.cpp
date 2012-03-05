#include "server.h"
#include "server_impl.h"



wmdf::Server::Server(const char* ip_address, int port,int thread_num,int max_connection_count,int max_packet_size/*=1024*/,int max_cache_msg_count/*=20*/):
  impl_(new ServerImpl(ip_address,port,thread_num,max_connection_count,max_packet_size,max_cache_msg_count))
{

}

wmdf::Server::~Server( void )
{
  delete impl_;
}

int wmdf::Server::Start()
{
  impl_->SetParent(this);
  return impl_->Start();
}

void wmdf::Server::Stop()
{
  impl_->Stop();
}

void wmdf::Server::SetOption( Option option,bool value )
{
  impl_->SetOption(option,value);
}

void wmdf::Server::SetConnectedCallback( ConnectedCallback callback )
{
  impl_->SetConnectedCallback(callback);
}

void wmdf::Server::SetCloseCallback( CloseCallback callback )
{
  impl_->SetCloseCallback(callback);
}


void wmdf::Server::SetRecvCallback( RecvCallback callback )
{
  impl_->SetRecvCallback(callback);
}

wmdf::ConnManager* wmdf::Server::GetConnManager()
{
  return impl_->GetConnManager();
}



void wmdf::Server::SetEventCallback( EventCallback callback )
{
  impl_->SetEventCallback(callback);
}

void wmdf::Server::SendMsg( int socket_id,Message* msg )
{
  impl_->SendMsg(socket_id,msg);
}

void wmdf::Server::BroadcastMsg( int group_id,Message* msg )
{
  impl_->BroadcastMsg(group_id,msg);
}

NET_API void wmdf::Server::SetState( void* state )
{
  impl_->SetState(state);
}

void wmdf::Server::CloseConnection( int socket_id )
{
	impl_->CloseConnection(socket_id);
}



