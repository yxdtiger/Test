#ifndef HELP_COMMAND_H
#define HELP_COMMAND_H

#include "console_command.h"

namespace wmdf
{
  class HelpCommand:public ConsoleCommand
  {
  public:
    HelpCommand(std::string command_tag, std::string alias);
    virtual ~HelpCommand(void);
    virtual void Execute(const std::string& command_line);
    virtual void OnHelp();
  };
}

#endif
