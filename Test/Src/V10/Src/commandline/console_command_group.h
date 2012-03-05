#ifndef CONSOLE_COMMAND_GROUP_H
#define CONSOLE_COMMAND_GROUP_H

#include <string>
#include <map>

#include "console_command.h"

namespace wmdf
{
	class ConsoleCommandGroup
	{
	public:
		ConsoleCommandGroup(std::string group_tag = "");
		~ConsoleCommandGroup(void);
	public:
		void AddConsoleCommand(ConsoleCommand* command);
		ConsoleCommand* GetConsoleCommand(std::string command_tag);
    void OnHelp();
		std::string group_tag();

	private:
		std::string group_tag_;
		std::map<std::string, ConsoleCommand*> map_command_group_;
    NullConsoleCommand* null_command_;
	};
}

#endif
