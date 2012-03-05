#ifndef CONSOLE_COMMAND_PRINTER_H
#define CONSOLE_COMMAND_PRINTER_H

#include <list>
#include <string>

namespace wmdf
{
	class ConsoleCommandPrinter
	{
	public:
		ConsoleCommandPrinter(void);
		~ConsoleCommandPrinter(void);

	public:
		static void PrintLines(std::list<std::string>& lines);

		static void PrintLine(const std::string& line);

		static void Print(const std::string& line);

		static void Clear();

	private:
	};
}

#endif
