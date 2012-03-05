#ifndef FREPLAY_COMMAND_H
#define FREPLAY_COMMAND_H

#include "console_command.h"

namespace wmdf
{
  //replay data record by mdf;
  class FReplayCommand: public ConsoleCommand
  {
  public:
    FReplayCommand(std::string command_tag, std::string alias);
    virtual ~FReplayCommand(void);
    virtual void Execute(const std::string& command_line);
    virtual void OnHelp();
  };
}

#endif
