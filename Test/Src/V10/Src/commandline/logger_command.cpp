#include "logger_command.h"
#include <vector>
#include "console_command_parser.h"
#include "../environment/server_environment.h"
#include "../monitor/monitor_state.h"

using namespace std;

wmdf::LoggerCommand::LoggerCommand(string command_tag, string alias)
  :ConsoleCommand(command_tag,alias)
{
}

wmdf::LoggerCommand::~LoggerCommand(void)
{
}

void wmdf::LoggerCommand::Execute( const string& command_line )
{
  result_list_.clear();
  vector<string> parameters = ConsoleCommandParser::ParseCommandParameters(command_line);
  if (parameters.size() > 1)
  {
    if (parameters[1] == "-s")
    {
      if (!ServerEnvironment::mointor_state()->logging())
      {
        ServerEnvironment::mointor_state()->set_logging(true);
        result_list_.push_back("logger started");
      }
    }
    else if (parameters[1] == "-t")
    {
      if (ServerEnvironment::mointor_state()->logging())
      {
        ServerEnvironment::mointor_state()->set_logging(false);
        result_list_.push_back("logger terminated");
      }
    }
    else if (parameters[1] == "-h")
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

void wmdf::LoggerCommand::OnHelp()
{
  result_list_.push_back("--"+command_tag_);
  result_list_.push_back("   -s startup logger");
  result_list_.push_back("   -t shutdown logger");
  result_list_.push_back("   -h display this help");
}
