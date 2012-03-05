#ifndef ADAPTER_COMMAND_H
#define ADAPTER_COMMAND_H

#include "console_command.h"

namespace wmdf
{
  class AdapterCommand:public ConsoleCommand
  {
  public:
    AdapterCommand(std::string command_tag, std::string alias);
    virtual ~AdapterCommand(void);
    virtual void Execute(const std::string& command_line);
    virtual void OnHelp();
  };
}

#endif
