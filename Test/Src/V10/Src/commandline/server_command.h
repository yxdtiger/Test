#ifndef SERVER_COMMAND_H
#define SERVER_COMMAND_H

#include "console_command.h"

namespace wmdf
{
  class ServerCommand: public ConsoleCommand
  {
  public:
    ServerCommand(string command_tag, string alias);
    virtual ~ServerCommand(void);
    virtual void Execute(const string& command_line);
    virtual void OnHelp();
  };
}

#endif
