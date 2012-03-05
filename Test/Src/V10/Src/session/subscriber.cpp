#include "subscriber.h"
#include "../protocol/protocol_description.h"
#include "connection.h"

using namespace wmdf;

wmdf::Subscriber::Subscriber(int32_t socketId,int32_t user_id,std::string& ip_address,time_stamp_t connected_time)
{
  socket_id_ = socketId;
  user_id_ = user_id;
  ip_address_ = ip_address;
  connected_time_= connected_time;
  connection_ = NULL;
  to_be_kicked_ = false;
}

wmdf::Subscriber::~Subscriber(void)
{
  for(uint32_t i=0;i<subscribed_products_.size();i++)
    delete subscribed_products_[i];
  subscribed_products_.clear();
  connection_ = NULL;
}

void wmdf::Subscriber::AddSubscribedProduct( ProtocolDescription* protocol_description )
{
  subscribed_products_.push_back(protocol_description);
}

int64_t wmdf::Subscriber::get_send_size()
{
  if (NULL != connection_)
  {
    return connection_->total_send_size();
  }
  return 0;
}

int32_t wmdf::Subscriber::get_msg_in_queue()
{
  if (NULL != connection_)
  {
    return connection_->msg_in_queue();
  }
  return 0;
}
