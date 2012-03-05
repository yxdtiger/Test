#ifndef USER_COMMAND_H
#define USER_COMMAND_H

#include "console_command.h"

namespace wmdf
{
  class UserCommand: public ConsoleCommand
  {
  public:
    UserCommand(std::string command_tag, std::string alias);
    virtual ~UserCommand(void);
    virtual void Execute(const std::string& command_line);
    virtual void OnHelp();
  };
}

#endif
