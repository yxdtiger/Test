#include "console_command_processor.h"
#include "console_command_printer.h"
#include "console_command_parser.h"
#include "console_command_group.h"
#include "system_console_command_factory.h"
#include "taq_console_command_factory.h"
#include "console_command_define.h"
#include "console_command_manager.h"
#include "../environment/server_environment.h"
#include "../environment/utils.h"
#include "../environment/strutils.h"

namespace wmdf
{
  ConsoleCommandProcessor::ConsoleCommandProcessor(void)
  {
    console_command_manager_ = ServerEnvironment::console_command_manager();
    current_group_tag_ = SYSTEM_CONSOLECOMMAND;

    ConsoleCommandGroup* command_group = CreateSystemConsoleCommandGroup();

    // add "null" command into command_manager
    console_command_manager_->RegisterCommandGroup(command_group, CreateSystemConsoleCommand(SYSTEM_CONSOLECOMMAND));
  }

  ConsoleCommandProcessor::~ConsoleCommandProcessor(void)
  {
    if(NULL != console_command_manager_)
    {
      delete console_command_manager_;
      console_command_manager_ = NULL;
    }
  }

  void ConsoleCommandProcessor::EnterMDFConsole()
  {
    processing_ = true;
    ConsoleCommandPrinter::Print(CONSOLECOMMAND_PRELOAD_SYMBOL);
  }

  void ConsoleCommandProcessor::ExitMDFConsole()
  {
    processing_ = false;
  }

  void ConsoleCommandProcessor::ProcessConsoleCommand(const string& command_line)
  {
    ConsoleCommand* console_command = NULL;
    string command_string = command_line;
    if (command_line != "")
    {
      command_string = StrUtils::Trim(command_line);
    }

    string command_tag = ConsoleCommandParser::ParseCommandName(command_string);

    // first, find in system group
    console_command = console_command_manager_->GetConsoleCommand(SYSTEM_CONSOLECOMMAND, command_tag);
    if(console_command->command_tag() != NULL_CONSOLECOMMAND)
    {
      current_group_tag_ = SYSTEM_CONSOLECOMMAND;
      console_command->Execute(command_string);
      ConsoleCommandPrinter::PrintLines(console_command->result_list());
      ConsoleCommandPrinter::Print(CONSOLECOMMAND_PRELOAD_SYMBOL);
    }
    else
    {
      // if can't find it in system group, find it in entry commands.
      if(current_group_tag_ == SYSTEM_CONSOLECOMMAND)
      {
        console_command = console_command_manager_->GetEntryCommand(command_tag);
        //can't find entry command, execute a null command
        if(NULL_CONSOLECOMMAND == console_command->command_tag())
        {
          current_group_tag_ = SYSTEM_CONSOLECOMMAND;
          console_command->Execute(command_string);
          ConsoleCommandPrinter::PrintLines(console_command->result_list());
          ConsoleCommandPrinter::Print(CONSOLECOMMAND_PRELOAD_SYMBOL);
        }
        else
        {
          //find in entry commands, switch command group
          current_group_tag_ = console_command->command_tag();
          console_command->Execute(command_string);
          ConsoleCommandPrinter::PrintLines(console_command->result_list());
          ConsoleCommandPrinter::Print(current_group_tag_+CONSOLECOMMAND_PRELOAD_SYMBOL);
        }
      }
      else
      {
        //find command in currect command group and execute it.
        console_command = console_command_manager_->GetConsoleCommand(current_group_tag_, command_tag);
        console_command->Execute(command_string);
        ConsoleCommandPrinter::PrintLines(console_command->result_list());
        ConsoleCommandPrinter::Print(current_group_tag_+CONSOLECOMMAND_PRELOAD_SYMBOL);
      }
    }
  }
}
