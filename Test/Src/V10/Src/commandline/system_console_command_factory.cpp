#include "system_console_command_factory.h"
#include "console_command_define.h"
#include "console_command.h"
#include "console_command_group.h"
#include "monitor_command.h"
#include "logger_command.h"
#include "net_command.h"
#include "user_command.h"
#include "help_command.h"
#include "adapter_command.h"
#include "freplay_command.h"
#ifdef WMDFADAPTER_
#include "replay_command.h"
#endif

using namespace std;

namespace wmdf
{
  ConsoleCommand* CreateNullConsoleCommand()
  {
    return new NullConsoleCommand(NULL_CONSOLECOMMAND, NULL_CONSOLECOMMAND);
  }

  ConsoleCommandGroup* CreateSystemConsoleCommandGroup()
  {
    ConsoleCommandGroup* console_command_group = new ConsoleCommandGroup(SYSTEM_CONSOLECOMMAND);

    // add kinds of console commands here.
    console_command_group->AddConsoleCommand(CreateSystemConsoleCommand(HELP_CONSOLECOMMAND));
    console_command_group->AddConsoleCommand(CreateSystemConsoleCommand(CLS_CONSOLECOMMAND));
    console_command_group->AddConsoleCommand(CreateSystemConsoleCommand(MONITOR_CONSOLECOMMAND));
    console_command_group->AddConsoleCommand(CreateSystemConsoleCommand(LOGGER_CONSOLECOMMAND));
    console_command_group->AddConsoleCommand(CreateSystemConsoleCommand(NET_CONSOLECOMMAND));
    console_command_group->AddConsoleCommand(CreateSystemConsoleCommand(USER_CONSOLECOMMAND));
    console_command_group->AddConsoleCommand(CreateSystemConsoleCommand(ADAPTER_CONSOLECOMMAND));
    console_command_group->AddConsoleCommand(CreateSystemConsoleCommand(FREPLAY_CONSOLECOMMAND));
#ifdef WMDFADAPTER_
    console_command_group->AddConsoleCommand(CreateSystemConsoleCommand(RREPLAY_CONSOLECOMMAND));
#endif
    return console_command_group;
  }

  ConsoleCommand* CreateSystemConsoleCommand(string command_line)
  {
    if (SYSTEM_CONSOLECOMMAND == command_line)
    {
      return new SystemGroupConsoleCommand(SYSTEM_CONSOLECOMMAND, SYSTEM_CONSOLECOMMAND);
    }

    if (HELP_CONSOLECOMMAND == command_line)
    {
      return new HelpCommand(HELP_CONSOLECOMMAND, HELP_CONSOLECOMMAND_ALIAS);
    }
    else if (CLS_CONSOLECOMMAND == command_line)
    {
      return new ClearScreenConsoleCommand(CLS_CONSOLECOMMAND, CLS_CONSOLECOMMAND_ALIAS);
    }
    else if (MONITOR_CONSOLECOMMAND == command_line)
    {
      return new MonitorCommand(MONITOR_CONSOLECOMMAND,MONITOR_CONSOLECOMMAND_ALIAS);
    }
    else if(LOGGER_CONSOLECOMMAND == command_line)
    {
      return new LoggerCommand(LOGGER_CONSOLECOMMAND,LOGGER_CONSOLECOMMAND_ALIAS);
    }
    else if(NET_CONSOLECOMMAND == command_line)
    {
      return new NetCommand(NET_CONSOLECOMMAND,NET_CONSOLECOMMAND_ALIAS);
    }
    else if(USER_CONSOLECOMMAND == command_line)
    {
      return new UserCommand(USER_CONSOLECOMMAND,USER_CONSOLECOMMAND_ALIAS);
    }
    else if(FREPLAY_CONSOLECOMMAND == command_line)
    {
      return new FReplayCommand(FREPLAY_CONSOLECOMMAND,FREPLAY_CONSOLECOMMAND_ALIAS);
    }
#ifdef WMDFADAPTER_
    else if(RREPLAY_CONSOLECOMMAND == command_line)
    {
      return new RReplayCommand(RREPLAY_CONSOLECOMMAND,RREPLAY_CONSOLECOMMAND_ALIAS);
    }
#endif
    else if(ADAPTER_CONSOLECOMMAND == command_line)
    {
      return new AdapterCommand(ADAPTER_CONSOLECOMMAND,ADAPTER_CONSOLECOMMAND_ALIAS);
    }

    return NULL;
  }
}
