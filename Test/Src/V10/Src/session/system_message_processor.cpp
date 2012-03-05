#include "system_message_processor.h"
#include "system_message_handler.h"
#include "feed_server.h"


using namespace std;

wmdf::SystemMessageProcessor::SystemMessageProcessor(ProtocolDescription* protocol_description)
{
  protocol_description_ = protocol_description;
}

wmdf::SystemMessageProcessor::~SystemMessageProcessor(void)
{
}

void wmdf::SystemMessageProcessor::AddMessageHandle( int16_t message_id,SystemMessageHandler* handle)
{
  system_message_handles_.insert(make_pair(message_id,handle));
}

void wmdf::SystemMessageProcessor::ProcessMessage( SharedMdfMessage shared_mdf_message,FeedServer* feed_server,wmdf::ProtocolProcessor* processor)
{
  SystemMessageHandler* handler = GetSystemMessageHandler(shared_mdf_message->message_id());
  if (NULL != handler)
  {
    handler->ProcessMessage(shared_mdf_message,feed_server,processor);
  }
}

wmdf::SystemMessageHandler* wmdf::SystemMessageProcessor::GetSystemMessageHandler( int16_t message_id )
{
  map<int16_t,SystemMessageHandler*>::iterator iter = system_message_handles_.find(message_id);
  if (iter != system_message_handles_.end())
  {
    return iter->second;
  }
  return NULL;
}
