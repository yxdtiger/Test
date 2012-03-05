#include "console_command_group.h"
#include "console_command_define.h"
#include "console_command_printer.h"

using namespace wmdf;
using namespace std;

ConsoleCommandGroup::ConsoleCommandGroup(string group_tag /* = "" */)
{
  group_tag_ = group_tag;
  null_command_ = new NullConsoleCommand(NULL_CONSOLECOMMAND,NULL_CONSOLECOMMAND);
}

ConsoleCommandGroup::~ConsoleCommandGroup(void)
{
  map<string, ConsoleCommand*>::iterator iter=map_command_group_.begin();
  while (iter != map_command_group_.end())
  {
    if(NULL !=iter->second)
    {
      delete iter->second;
      iter->second = NULL;
    }
    iter++;
  }
  map_command_group_.clear();
  if (NULL != null_command_)
  {
    delete null_command_;
    null_command_ = NULL;
  }
}

void ConsoleCommandGroup::AddConsoleCommand(ConsoleCommand*  command_ptr)
{
  map_command_group_[command_ptr->command_tag()] = command_ptr;
  map_command_group_[command_ptr->alias()] = command_ptr;
}

ConsoleCommand* ConsoleCommandGroup::GetConsoleCommand(string command_tag)
{
  if( map_command_group_.find(command_tag) != map_command_group_.end() )
    return map_command_group_[command_tag];

  return null_command_;
}

string ConsoleCommandGroup::group_tag()
{
  return group_tag_;
}

void ConsoleCommandGroup::OnHelp()
{
  map<string, ConsoleCommand*>::iterator iter=map_command_group_.begin();
  while (iter != map_command_group_.end())
  {
    if(iter->second->command_tag() != HELP_CONSOLECOMMAND)
    {
      if (iter->first == iter->second->command_tag())
      {
        iter->second->Execute(iter->second->command_tag()+" -h");
        ConsoleCommandPrinter::PrintLines(iter->second->result_list());  
      }
      
    }
    iter++;
  }
}
