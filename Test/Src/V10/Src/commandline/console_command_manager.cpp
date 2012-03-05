#include "console_command_manager.h"
#include "console_command_define.h"
#include "console_command_group.h"
#include "console_command.h"

using namespace std;
using namespace wmdf;

ConsoleCommandManager::ConsoleCommandManager(void)
{
  null_command_ = new NullConsoleCommand(NULL_CONSOLECOMMAND,NULL_CONSOLECOMMAND);
}

ConsoleCommandManager::~ConsoleCommandManager(void)
{
  map<string, ConsoleCommandGroup*>::iterator iter = map_command_groups_.begin();
  while (iter != map_command_groups_.end())
  {
    if(NULL != iter->second)
    {
      delete iter->second;
      iter->second = NULL;
    }
    iter++;
  }


  map_command_groups_.clear();
  if (NULL != null_command_)
  {
    delete null_command_;
    null_command_ = NULL;
  }
}

ConsoleCommand* ConsoleCommandManager::GetConsoleCommand(string group_tag, string command_tag)
{
  if(map_command_groups_.find(group_tag) != map_command_groups_.end())
    return map_command_groups_[group_tag]->GetConsoleCommand(command_tag);
  else
    return null_command_;
}

void ConsoleCommandManager::RegisterCommandGroup(ConsoleCommandGroup* command_group, ConsoleCommand* entry_command)
{
  if(map_entry_command_.find(entry_command->command_tag()) == map_entry_command_.end())
  {
    map_entry_command_[entry_command->command_tag()] = entry_command;
  }

  if(map_command_groups_.find(command_group->group_tag()) == map_command_groups_.end())
  {
    map_command_groups_[command_group->group_tag()] = command_group;
  }
}

ConsoleCommand* ConsoleCommandManager::GetEntryCommand(string command_id)
{
  if(map_entry_command_.find(command_id) != map_entry_command_.end())
    return map_entry_command_[command_id];
  else
    return null_command_;
}

void ConsoleCommandManager::OnHelp()
{
  map<string, ConsoleCommandGroup*>::iterator iter = map_command_groups_.begin();
  while (iter != map_command_groups_.end())
  {
    if(NULL != iter->second)
    {
      ConsoleCommandGroup* group = iter->second;
      group->OnHelp();
    }
    iter++;
  }
}
