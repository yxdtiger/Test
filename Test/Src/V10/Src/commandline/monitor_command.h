#ifndef MONITOR_COMMAND_H
#define MONITOR_COMMAND_H

#include "console_command.h"

namespace wmdf
{
  class MonitorCommand : public ConsoleCommand
  {
  public:
    MonitorCommand(std::string command_tag, std::string alias);
    virtual ~MonitorCommand(void);
    virtual void Execute(const std::string& command_line);
    virtual void OnHelp();
  };



}

#endif
