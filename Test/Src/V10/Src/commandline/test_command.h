#ifndef Test_COMMAND_H
#define Test_COMMAND_H

#include "console_command.h"

namespace wmdf
{
  class TestCommand : public ConsoleCommand
  {
  public:
    TestCommand(std::string command_tag, std::string alias);
    virtual ~TestCommand(void);
    virtual void Execute(const std::string& command_line);
    virtual void OnHelp();
  };
}

#endif
