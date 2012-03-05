#ifndef CONSOLE_COMMAND_PARSER_H
#define CONSOLE_COMMAND_PARSER_H

#include <string>
#include <vector>

namespace wmdf
{

	class ConsoleCommandParser
  	{
	public:
		ConsoleCommandParser(void);
		~ConsoleCommandParser(void);

	public:
		static std::string ParseCommandName(const std::string& command_line);
		static std::vector<std::string> ParseCommandParameters(const std::string& command_line);
	};
}

#endif
