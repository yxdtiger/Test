#include "taq_console_command_factory.h"

//#include <boost/algorithm/string.hpp>
#include "console_command_define.h"
#include "console_command.h"
#include "console_command_group.h"


namespace wmdf
{
  // 创建taq组
  ConsoleCommandGroup* CreateTaqConsoleCommandGroup()
  {
    ConsoleCommandGroup* console_command_group_ptr = new ConsoleCommandGroup(TAQ_CONSOLECOMMAND);

    // add kinds of console commands here.
    console_command_group_ptr->AddConsoleCommand(CreateTaqConsoleCommand(TAQ_CONSOLECOMMAND_COMMAND_1));
    console_command_group_ptr->AddConsoleCommand(CreateTaqConsoleCommand(TAQ_CONSOLECOMMAND_COMMAND_2));

    return console_command_group_ptr;
  }

  // 在taq组中，创建ConsoleCommand
  IConsoleCommand* CreateTaqConsoleCommand(string command_line)
  {
    //to_lower(command_line);

    // create kinds of console command types
    if(TAQ_CONSOLECOMMAND_COMMAND_1 == command_line)
    {
      return new TaqConsoleCommand1(TAQ_CONSOLECOMMAND_COMMAND_1, TAQ_CONSOLECOMMAND_COMMAND_1);
    }
    else
      if(TAQ_CONSOLECOMMAND_COMMAND_2 == command_line)
      {
        return new TaqConsoleCommand2(TAQ_CONSOLECOMMAND_COMMAND_2, TAQ_CONSOLECOMMAND_COMMAND_2);
      }

      return NULL;
  }

  // 在taq组中，创建entry command
  IConsoleCommand* CreateTaqEntryCommand()
  {
    return new TaqEntryCommand(TAQ_CONSOLECOMMAND, TAQ_CONSOLECOMMAND);
  }

}