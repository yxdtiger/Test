#include "client.h"
#include "client_impl.h"


wmdf::Client::Client( int  max_packet_size,int max_buf_msg,int thread_num/*=1*/ )
  :impl_(new ClientImpl(max_packet_size,max_buf_msg,thread_num))
{

}

wmdf::Client::~Client()
{
  delete impl_;
}

int wmdf::Client::Connect( const char* ip_address,int port,size_t timeout/*=0*/ )
{
  impl_->SetParent(this);
  return impl_->Connect(ip_address,port,timeout);
}

void wmdf::Client::SendMsg( Message* msg )
{
  impl_->SendMsg(msg);
}

Message* wmdf::Client::RecvMsg()
{
  return impl_->RecvMsg();
}


void wmdf::Client::Close()
{
  impl_->Close();
}



void wmdf::Client::SetCloseCallback( CloseCallback callback )
{
  impl_->SetCloseCallback(callback);
}


void wmdf::Client::SetEventCallback( EventCallback callback )
{
  impl_->SetEventCallback(callback);
}

void wmdf::Client::Init(int client_type)
{
	impl_->Init(client_type);
}

NET_API void wmdf::Client::SetRecvCallback( RecvCallback callback )
{
	impl_->SetRecvCallback(callback);
}

NET_API void wmdf::Client::SetState( void* state )
{
  impl_->SetState(state);
}
