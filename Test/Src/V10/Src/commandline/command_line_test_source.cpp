#include <iostream>
using namespace std;

#include "console_command_processor.h"
using namespace wmdf;

int main(int argc, char* argv[])
{
  // 创建命令行processor
  ConsoleCommandProcessor console_command_processor;

  // 进入wmdf命令行
  console_command_processor.EnterMDFConsole();

  // 命令行循环
  while(1)
  {
    string command_line;
    cin >> command_line;
    console_command_processor.ProcessConsoleCommand(command_line);
  }

  // 退出wmdf命令行
  console_command_processor.ExitMDFConsole();

	return 0;
}