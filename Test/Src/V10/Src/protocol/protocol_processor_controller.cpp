#include "protocol_processor_controller.h"
#include "protocol_processor.h"
#include "mdf_message.h"

using namespace std;

wmdf::ProtocolProcessorController::ProtocolProcessorController(void)
{
}

wmdf::ProtocolProcessorController::~ProtocolProcessorController(void)
{
  std::map<int16_t,ProtocolProcessor*> ::iterator iter = protocol_processors_.begin();
  while(iter != protocol_processors_.end())
  {
    delete iter->second;
    iter++;
  }
  protocol_processors_.clear();
}

void wmdf::ProtocolProcessorController::AddProtocolProcessor( ProtocolProcessor* protocol_processor )
{
  protocol_processors_.insert(make_pair(protocol_processor->product_id(),protocol_processor));
}

wmdf::ProtocolProcessor* wmdf::ProtocolProcessorController::GetProtocolProcessor( int32_t processor_id )
{
  std::map<int16_t,ProtocolProcessor*>::iterator iter = protocol_processors_.find(processor_id);
  if (iter != protocol_processors_.end())
  {
    return iter->second;
  }
  return NULL;
}

wmdf::MdfMessage* wmdf::ProtocolProcessorController::Decode( Message* raw_message )
{
  std::map<int16_t,ProtocolProcessor*>::iterator iter = protocol_processors_.find(raw_message->product_id);
  if (iter != protocol_processors_.end())
  {
    return iter->second->Decode(raw_message);
  }
  return NULL;
}

MdfRecord* wmdf::ProtocolProcessorController::DecodeToRecord( Message* raw_message )
{
  std::map<int16_t,ProtocolProcessor*>::iterator iter = protocol_processors_.find(raw_message->product_id);
  if (iter != protocol_processors_.end())
  {
    return iter->second->DecodeToRecord(raw_message);
  }
  return NULL;
}