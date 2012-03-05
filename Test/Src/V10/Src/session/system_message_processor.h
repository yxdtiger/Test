#ifndef SYSTEM_MESSAGE_PROCESSOR_H
#define SYSTEM_MESSAGE_PROCESSOR_H
#include <map>
#include "../include/wmdf_api.h"
#include "../protocol/protocol_description.h"
#include "../protocol/mdf_message.h"

namespace wmdf
{
  class FeedServer;
  class SystemMessageHandler;
  class ProtocolProcessor;
  class SystemMessageProcessor
  {
  public:
    SystemMessageProcessor(ProtocolDescription* protocol_description);
    virtual ~SystemMessageProcessor(void);
    void AddMessageHandle(int16_t message_id,SystemMessageHandler*  handle);
    int64_t unique_id() {return protocol_description_->unique_id();}
    void ProcessMessage(SharedMdfMessage shared_mdf_message,FeedServer* feed_server,ProtocolProcessor* processor);
    SystemMessageHandler* GetSystemMessageHandler( int16_t message_id );
  private:
    inline SystemMessageHandler* GetSysMessageHandle(int16_t message_id);
    ProtocolDescription* protocol_description_;
    std::map<int16_t,SystemMessageHandler*> system_message_handles_;
  };
}

#endif
