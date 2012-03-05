#ifndef TAQ_CONSOLE_COMMAND_FACTORY_H
#define TAQ_CONSOLE_COMMAND_FACTORY_H

#include <string>
using namespace std;

namespace wmdf
{
  class IConsoleCommand;
  class ConsoleCommandGroup;

  // taq command factory
  // 创建taq组
  ConsoleCommandGroup* CreateTaqConsoleCommandGroup();

  // 在taq组中，创建ConsoleCommand
  IConsoleCommand* CreateTaqConsoleCommand(string command_line);

  // 在taq组中，创建entry command
  IConsoleCommand* CreateTaqEntryCommand();

}

#endif
