#ifndef CONSOLE_COMMAND_MANAGER_H
#define CONSOLE_COMMAND_MANAGER_H

#include <string>
#include <map>




namespace wmdf
{
  class ConsoleCommand;
  class ConsoleCommandGroup;

	class ConsoleCommandManager
	{
	public:
		ConsoleCommandManager(void);
		~ConsoleCommandManager(void);
	public:
    // 通过group tag和command tag获取console command
		ConsoleCommand* GetConsoleCommand(std::string group_tag, std::string command_tag);

		void RegisterCommandGroup(ConsoleCommandGroup* command_group, ConsoleCommand* entry_command);

    ConsoleCommand* GetEntryCommand(std::string command_id);

    void OnHelp();

	private:
    std::map<std::string, ConsoleCommandGroup*> map_command_groups_;
    // vector<string> arr_entry_command_groups_;
    std::map<std::string, ConsoleCommand*> map_entry_command_;
    ConsoleCommand* null_command_;
	};
}

#endif
