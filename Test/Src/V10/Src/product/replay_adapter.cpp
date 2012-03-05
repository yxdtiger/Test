#include "replay_adapter.h"
#include "../protocol/protocol_description.h"
#include "../protocol/mdf_message.h"
#include "../session/message_publisher.h"


wmdf::ReplayAdapter::ReplayAdapter( ProtocolDescription* protocol_description )
{
  protocol_description_ = protocol_description;
}

wmdf::ReplayAdapter::~ReplayAdapter()
{
  protocol_description_ = NULL;
}

void wmdf::ReplayAdapter::OnRecvMsg( Message* msg )
{
  RelayMdfMessage* mdf_message = new RelayMdfMessage();
  mdf_message->local_time = msg->local_time;
  mdf_message->packet_num = msg->packet_num;
  mdf_message->socket_id = 0;
  mdf_message->length = msg->msg_len-MSG_HEADER_LEN;
  mdf_message->product_id = msg->product_id;
  mdf_message->message_id = msg->message_id;
  mdf_message->major_version = protocol_description_->major_version();
  mdf_message->minor_version = protocol_description_->minor_version();
  mdf_message->data = new int8_t[mdf_message->length];
  memcpy(mdf_message->data,msg->data,mdf_message->length);
  SharedRelayMdfMessage shared_message(mdf_message);
  MessagePublisher::PublishOneMessage(shared_message);
}

void wmdf::ReplayAdapter::OnCloseCallback( EventMsg* args )
{

}

void wmdf::ReplayAdapter::OnEventCallback( EventMsg* args )
{

}

void wmdf::ReplayAdapter::ResetContext()
{

}
