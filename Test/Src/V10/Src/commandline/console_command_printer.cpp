#include "console_command_printer.h"
#include <iostream>
#include <cstdlib>
using namespace std;

namespace wmdf
{
	ConsoleCommandPrinter::ConsoleCommandPrinter(void)
	{
	}

	ConsoleCommandPrinter::~ConsoleCommandPrinter(void)
	{
	}

	void ConsoleCommandPrinter::PrintLines(list<string>& lines)
	{
		list<string>::iterator it;
		for(it = lines.begin(); it != lines.end(); ++it)
		{
			cout << *it << endl;
		}
	}

	void ConsoleCommandPrinter::PrintLine(const string& line)
	{
		cout << line << endl;
	}

	void ConsoleCommandPrinter::Print(const string& line)
	{
		cout << line;
	}

	void ConsoleCommandPrinter::Clear()
	{
#ifdef WIN32
		system("cls");
#elif __linux__
        system("clear");
#endif
	}
}
