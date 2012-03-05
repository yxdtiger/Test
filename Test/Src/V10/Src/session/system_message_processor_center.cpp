#include "system_message_processor_center.h"
#include "system_message_processor.h"

using namespace std;
using namespace wmdf;

wmdf::SystemMessageProcessorCenter::SystemMessageProcessorCenter(void)
{
}

wmdf::SystemMessageProcessorCenter::~SystemMessageProcessorCenter(void)
{
  std::map<int64_t, SystemMessageProcessor*>::iterator iter=system_message_processors_.begin();
  while (iter != system_message_processors_.end())
  {
    if(NULL != iter->second)
    {
      delete iter->second;
      iter->second = NULL;
    }
    iter++;
  }
  system_message_processors_.clear();
}

void wmdf::SystemMessageProcessorCenter::AddSystemMessageProcessor( SystemMessageProcessor* system_message_processor )
{
  system_message_processors_.insert(make_pair(system_message_processor->unique_id(),system_message_processor));
}

wmdf::SystemMessageProcessor* wmdf::SystemMessageProcessorCenter::GetSystemMessageProcessor( int64_t protocol_unique_id )
{
  map<int64_t,SystemMessageProcessor*>::iterator iter = system_message_processors_.find(protocol_unique_id);
  if (iter != system_message_processors_.end())
  {
    return iter->second;
  }
  return NULL;
}