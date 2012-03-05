#ifndef REPLAY_COMMAND_H
#define REPLAY_COMMAND_H

#include "console_command.h"

namespace wmdf
{
   //replay rawdata of exchange
  class RReplayCommand: public ConsoleCommand
  {
  public:
    RReplayCommand(std::string command_tag, std::string alias);
    virtual ~RReplayCommand(void);
    virtual void Execute(const std::string& command_line);
    virtual void OnHelp();
  };
}

#endif
