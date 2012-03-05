#ifndef WIND_NET_RELAY_CONNECTION_
#define WIND_NET_RELAY_CONNECTION_

#include "connection.h"

namespace wmdf{

class RelayConnection
	:public Connection
{
public:
	RelayConnection(io_service_type& io_service,Service* service,size_t read_buffer_size=1024,size_t max_cache_msg_count=20):
			Connection(io_service,service,read_buffer_size,max_cache_msg_count)
	{

	}
	bool DecodeMsgHeader( uint8_t* data,uint8_t* end,size_t& size )
	{
		Message* newMessage = MessageFactory::CreateMessage(size) ;
		memcpy(newMessage->data,data,size);
		service_->OnRecvMsg(socket_id_,newMessage);
		return true;
	}
};

}


#endif
