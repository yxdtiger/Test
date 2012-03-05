#ifndef SYSTEM_CONSOLE_COMMAND_FACTORY_H
#define SYSTEM_CONSOLE_COMMAND_FACTORY_H

#include <string>


namespace wmdf
{
  class ConsoleCommand;
  class ConsoleCommandGroup;

  // system command factory
  // 创建一个无效的ConsoleCommand
  ConsoleCommand* CreateNullConsoleCommand();

  // 创建system组
  ConsoleCommandGroup* CreateSystemConsoleCommandGroup();

  // 在system组中，创建ConsoleCommand
  // 若返回值为空，代表未定义对应command_line的ConsoleCommand类型
  ConsoleCommand* CreateSystemConsoleCommand(std::string command_name);

}

#endif
