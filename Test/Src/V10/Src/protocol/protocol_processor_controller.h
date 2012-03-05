#ifndef PROTOCOL_PROCESSOR_CONTROLLER_H
#define PROTOCOL_PROCESSOR_CONTROLLER_H

#include "net_module.h"
#include "../include/wmdf_api.h"
#include <map>

namespace wmdf
{
  class MdfMessage;
  class ProtocolProcessor;
  // this class have one version of every product
  class ProtocolProcessorController
  {
  public:
    ProtocolProcessorController(void);
    virtual ~ProtocolProcessorController(void);
    void AddProtocolProcessor(ProtocolProcessor* protocol_processor);
    ProtocolProcessor* GetProtocolProcessor(int32_t processor_id);
    MdfMessage* Decode(Message* raw_message);
    MdfRecord* DecodeToRecord(Message* raw_message);
  private:
    //productid, Protocol processor
    std::map<int16_t,ProtocolProcessor*> protocol_processors_;
  };
}

#endif
