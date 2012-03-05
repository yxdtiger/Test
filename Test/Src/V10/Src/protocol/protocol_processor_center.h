#ifndef PROTOCOL_PROCESSOR_CENTER_H
#define PROTOCOL_PROCESSOR_CENTER_H

#include <map>
#include "../include/wmdf_api.h"

namespace wmdf
{
  class ProtocolProcessor;

  class ProtocolProcessorCenter
  {
  public:
    static ProtocolProcessorCenter* instance();
    void AddProtocolProcessor(ProtocolProcessor* protocol_processor);
    ProtocolProcessor* GetProtocolProcessor(int64_t protocol_unique_id);
  private:
    ProtocolProcessorCenter(void);
    virtual ~ProtocolProcessorCenter(void);
    //unique id generate by protocol description, protocolprocessor
    std::map<int64_t,ProtocolProcessor*> protocol_processors_;
    static ProtocolProcessorCenter* instance_;
  };  
}

#endif
