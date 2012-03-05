#ifndef WMDF_INVOKE_H
#define WMDF_INVOKE_H
#include <vector>
#include <map>

#include "../../include/wmdf_structs.h"
#include "server_manager.h"

using namespace wmdf;
class WmdfInvoke
{
public:
  WmdfInvoke();
  virtual ~WmdfInvoke(void);
  void Connect();
  void Disconnect();
  void OnRecordCallback(RecordArgs* args);
};


#endif


