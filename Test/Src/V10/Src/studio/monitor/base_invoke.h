#ifndef BASE_INVOKE_H
#define BASE_INVOKE_H
#include <vector>
#include <map>

#include "../../include/wmdf_structs.h"
#include "server_manager.h"



class BaseInvoke
{
public:
  BaseInvoke();
  virtual ~BaseInvoke(void);

  virtual void Connect() =0;
  virtual void Disconnect() = 0;  
  virtual void OnRecordCallback(RecordArgs* args) = 0;
  
};

#endif


