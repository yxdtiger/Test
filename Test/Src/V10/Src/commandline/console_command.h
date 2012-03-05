#ifndef CONSOLE_COMMAND_H
#define	CONSOLE_COMMAND_H

#include <string>
#include <list>


namespace wmdf
{
  class ConsoleCommandProcessor;


	// 控制台命令的基类。
	class ConsoleCommand
	{
	public:
		ConsoleCommand(const std::string& command_tag, const std::string& alias);
		virtual ~ConsoleCommand(void);
		virtual void Execute(const std::string& command_line) = 0;
		virtual std::list<std::string>& result_list();
		virtual std::string command_tag();
		virtual std::string alias();
		virtual void OnHelp();
	protected:
		std::string command_tag_;
		std::string alias_;
		std::list<std::string> result_list_;
	};

  // 空控制台命令类
	class NullConsoleCommand : public ConsoleCommand
	{
	public:
		NullConsoleCommand(const std::string& command_tag, const std::string& alias);
		~NullConsoleCommand(void);
	public:
		void Execute(const std::string& command_line);
	};

  class SystemGroupConsoleCommand:public ConsoleCommand
  {
  public:
    SystemGroupConsoleCommand(const std::string& command_tag, const std::string& alias);
    ~SystemGroupConsoleCommand(void);
  public:
    void Execute(const std::string& command_line);
  };

	// 系统退出控制台命令类
	class ExitConsoleCommand : public ConsoleCommand
	{
	public:
		ExitConsoleCommand(const std::string& command_tag, const std::string& alias);
		~ExitConsoleCommand(void);
	public:
		void Execute(const std::string& command_line);
	};

  //
  class HelpConsoleCommand : public ConsoleCommand
  {
  public:
    HelpConsoleCommand(const std::string& command_tag, const std::string& alias);
    ~HelpConsoleCommand(void);
  public:
    void Execute(const std::string& command_line);
  };

  //
  class ClearScreenConsoleCommand : public ConsoleCommand
  {
  public:
    ClearScreenConsoleCommand(const std::string& command_tag, const std::string& alias);
    ~ClearScreenConsoleCommand(void);
    void Execute(const std::string& command_line);
    virtual void OnHelp();
  };

}

#endif
