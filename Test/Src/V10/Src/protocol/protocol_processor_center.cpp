#include "protocol_processor_center.h"
#include "protocol_processor.h"

using namespace std;

wmdf::ProtocolProcessorCenter* wmdf::ProtocolProcessorCenter::instance_ = new wmdf::ProtocolProcessorCenter();


wmdf::ProtocolProcessorCenter::ProtocolProcessorCenter(void)
{

}

wmdf::ProtocolProcessorCenter::~ProtocolProcessorCenter(void)
{
  map<int64_t,ProtocolProcessor*> ::iterator iter = protocol_processors_.begin();
  while(iter != protocol_processors_.end())
  {
    delete iter->second;
    iter++;
  }
  protocol_processors_.clear();
}

void wmdf::ProtocolProcessorCenter::AddProtocolProcessor( ProtocolProcessor* protocol_processor )
{
  protocol_processors_.insert(make_pair(protocol_processor->unique_id(),protocol_processor));
}

wmdf::ProtocolProcessor* wmdf::ProtocolProcessorCenter::GetProtocolProcessor( int64_t protocol_unique_id )
{
  map<int64_t,ProtocolProcessor*>::iterator iter = protocol_processors_.find(protocol_unique_id);
  if (iter != protocol_processors_.end())
  {
    return iter->second;
  }
  return NULL;
}

wmdf::ProtocolProcessorCenter* wmdf::ProtocolProcessorCenter::instance()
{
  return instance_;
}