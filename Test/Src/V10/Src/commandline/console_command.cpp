#include "console_command.h"
#include "console_command_parser.h"
#include <string>
#ifdef __linux__
#include <cstdlib>
#endif
using namespace std;

namespace wmdf
{

	// implementation of class "BaseConsoleCommand"
	ConsoleCommand::ConsoleCommand(const string& command_tag, const string& alias)
	:command_tag_(command_tag),alias_(alias)
	{
	}

	ConsoleCommand::~ConsoleCommand(void)
	{

	}

	std::list<string>& ConsoleCommand::result_list()
	{
		return result_list_;
	}

	string ConsoleCommand::command_tag()
	{
		return command_tag_;
	}

	string ConsoleCommand::alias()
	{
		return alias_;
	}

	void ConsoleCommand::OnHelp()
	{
		result_list_.push_back(command_tag_);
	}

	// implementation of class "NullConsoleCommand"
	NullConsoleCommand::NullConsoleCommand(const string& command_tag, const string& alias)
		: ConsoleCommand(command_tag, alias)
	{
	}
	NullConsoleCommand::~NullConsoleCommand(void)
	{

	}

	void NullConsoleCommand::Execute(const string& command_line)
	{
    result_list_.clear();
    if ("" != command_line)
    {
		  result_list_.push_back( string("\"") + command_line + string("\" ") + string("not found!"));
    }
	}


	// implementation of class "ExitConsoleCommand"
	ExitConsoleCommand::ExitConsoleCommand(const string& command_tag, const string& alias)
		: ConsoleCommand(command_tag, alias)
	{
	}
	ExitConsoleCommand::~ExitConsoleCommand(void)
	{

	}

	void ExitConsoleCommand::Execute(const string& command_line)
	{
    result_list_.clear();
		result_list_.push_back(string("not yet implemented."));
	}

  // implementation of class "ClearScreenConsoleCommand"
  ClearScreenConsoleCommand::ClearScreenConsoleCommand(const string& command_tag, const string& alias)
    : ConsoleCommand(command_tag, alias)
  {
  }
  ClearScreenConsoleCommand::~ClearScreenConsoleCommand(void)
  {

  }

  void ClearScreenConsoleCommand::Execute(const string& command_line)
  {
    result_list_.clear();
    vector<string> parameters = ConsoleCommandParser::ParseCommandParameters(command_line);
    if (parameters.size() == 1)
    {
#ifdef WIN32
      system("cls");
#elif __linux__
      system("clear");
#endif
    }
    else if (parameters.size() > 1)
    {
      if (parameters[1] == "-h")
      {
        OnHelp();
      }
      else
      {
        result_list_.push_back(command_tag_+": "+"incorrect parameters");
        result_list_.push_back("try '"+command_tag_+" -h' for more information");
      }
    }
    else
    {
      result_list_.push_back(command_tag_+": "+"missing parameters");
      result_list_.push_back("try '"+command_tag_+" -h' for more information");
    }
  }

  void ClearScreenConsoleCommand::OnHelp()
  {
    result_list_.push_back("--"+command_tag_+ "" );
    result_list_.push_back("   clear console");
    result_list_.push_back("   -h display this help");
  }


  SystemGroupConsoleCommand::SystemGroupConsoleCommand( const string& command_tag, const string& alias )
    : ConsoleCommand(command_tag, alias)
  {

  }

  SystemGroupConsoleCommand::~SystemGroupConsoleCommand( void )
  {

  }

  void SystemGroupConsoleCommand::Execute( const string& command_line )
  {

  }
}
