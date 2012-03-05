#ifndef CONSOLE_COMMAND_PROCESSOR_H
#define CONSOLE_COMMAND_PROCESSOR_H

#include <string>

namespace wmdf
{
  class ConsoleCommandManager;

	class ConsoleCommandProcessor
	{
	public:
		ConsoleCommandProcessor(void);
		~ConsoleCommandProcessor(void);

	public:
		void EnterMDFConsole();
		void ExitMDFConsole();
		void ProcessConsoleCommand(const std::string& command_line);
    bool processing() {return processing_;}
    void set_processing(bool processing) {processing_ = processing;}

	private:
		std::string current_group_tag_;
		ConsoleCommandManager* console_command_manager_;
		bool processing_;
	};
}

#endif
