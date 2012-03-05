#ifndef SYSTEM_MESSAGE_PROCESSOR_CENTER_H
#define SYSTEM_MESSAGE_PROCESSOR_CENTER_H

#include <map>
#include "../include/wmdf_api.h"


namespace wmdf
{
  class SystemMessageProcessor;
  
  class SystemMessageProcessorCenter
  {
  public:
    SystemMessageProcessorCenter(void);
    virtual ~SystemMessageProcessorCenter(void);
    void AddSystemMessageProcessor(SystemMessageProcessor* system_message_processor);
    SystemMessageProcessor* GetSystemMessageProcessor(int64_t protocol_unique_id);
  private:
    std::map<int64_t, SystemMessageProcessor*> system_message_processors_;
  };
}

#endif
