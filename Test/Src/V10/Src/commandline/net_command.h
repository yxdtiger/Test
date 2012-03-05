#ifndef NET_COMMAND_H
#define NET_COMMAND_H

#include "console_command.h"

namespace wmdf
{
  class NetCommand: public ConsoleCommand
  {
  public:
    NetCommand(std::string command_tag, std::string alias);
    virtual ~NetCommand(void);
    virtual void Execute(const std::string& command_line);
    virtual void OnHelp();
  };
}

#endif
