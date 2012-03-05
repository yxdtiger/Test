#include "net_module.h"
#include <stdlib.h>
#include "client.h"
#include "server.h"

typedef wmdf::Server server_t;
typedef wmdf::Client client_t;

NET_API void ReleaseEventMsg( EventMsg* data )
{
  delete data;
  data=NULL;
}

NET_API void ReleaseMessage( Message* msg )
{
  free(msg);
  msg=NULL;
}

NET_API void* C_Client( int max_packet_size,int max_buf_msg )
{
	client_t* client = new wmdf::Client(max_packet_size,max_buf_msg);
	client->Init();
	return client;
}

NET_API int  C_Connect( void* client, const char* ip_address,int port,size_t timeout )
{
	client_t* client_ =(client_t*)client;
	return client_->Connect(ip_address,port,timeout);
}

NET_API void C_SendMsg( void* client,Message* msg )
{
	client_t* client_ =(client_t*)client;
	client_->SendMsg(msg);
}

NET_API Message* C_RecvMsg( void* client )
{
	client_t* client_ =(client_t*)client;
	return client_->RecvMsg();
}

NET_API void C_Close( void* client )
{
	client_t* client_ =(client_t*)client;
	client_->Close();
}

NET_API void C_SetCloseCallback( void* client,CloseCallback callback )
{
	client_t* client_ =(client_t*)client;
	client_->SetCloseCallback(callback);
}

NET_API void C_SetEventCallback( void* client,EventCallback callback )
{
	client_t* client_ =(client_t*)client;
	client_->SetEventCallback(callback);
}

NET_API void* S_Server( const char* ip_address,int port,int thread_num,int max_connection_count,int max_packet_size,int max_cache_msg_count )
{
	server_t* server = new server_t(ip_address,port,thread_num,max_connection_count,max_packet_size,max_cache_msg_count);
	return server;
}

NET_API int S_Start( void* server )
{
	server_t* server_ = (server_t*)server;
	return server_->Start();
}

NET_API void S_Stop( void* server )
{
	server_t* server_ = (server_t*)server;
	server_->Stop();
}

NET_API void S_SendMsg( void* server,int socket_id,Message* msg )
{
	server_t* server_ = (server_t*)server;
	server_->SendMsg(socket_id,msg);
}

NET_API void S_BroadcastMsg( void* server,int group_id,Message* msg )
{
	server_t* server_ = (server_t*)server;
	server_->BroadcastMsg(group_id,msg);
}

NET_API void S_SetConnectedCallback( void* server,ConnectedCallback callback )
{
	server_t* server_ = (server_t*)server;
	server_->SetConnectedCallback(callback);
}

NET_API void S_SetCloseCallback( void* server,CloseCallback callback )
{
	server_t* server_ = (server_t*)server;
	server_->SetCloseCallback(callback);
}

NET_API void S_SetRecvCallback( void* server,RecvCallback callback )
{
	server_t* server_ = (server_t*)server;
	server_->SetRecvCallback(callback);
}

NET_API void S_SetEventCallback( void* server,EventCallback callback )
{
	server_t* server_ = (server_t*)server;
	server_->SetEventCallback(callback);
}

NET_API Message* CreateMessage( int size )
{
	return wmdf::MessageFactory::CreateMessage(size);
}

NET_API void C_SetRecvCallback( void* client,RecvCallback callback )
{
	client_t* client_ =(client_t*)client;
	client_->SetRecvCallback(callback);
}

