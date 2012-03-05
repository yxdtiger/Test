#include "monitor_command.h"
#include "../environment/server_environment.h"
#include "../environment/splitter.h"
#include "console_command_parser.h"
#include "../monitor/monitor_state.h"
#include "../monitor/monitor_adapter.h"

using namespace std;


wmdf::MonitorCommand::MonitorCommand( string command_tag , string alias )
 :ConsoleCommand(command_tag,alias)
{

}

wmdf::MonitorCommand::~MonitorCommand(void)
{
}

void wmdf::MonitorCommand::Execute( const string& command_line )
{
  result_list_.clear();
  vector<string> parameters = ConsoleCommandParser::ParseCommandParameters(command_line);
  if (parameters.size() > 1)
  {
    if (parameters[1] == "-s")
    {
      if (ServerEnvironment::mointor_state()->monitoring())
      {
        result_list_.push_back("monitor is already running");
      }
      else
      {
        ServerEnvironment::mointor_state()->set_monitoring(true);
        result_list_.push_back("monitor startup.");
      }
    }
    else if (parameters[1] == "-t")
    {
      if (ServerEnvironment::mointor_state()->monitoring())
      {
        ServerEnvironment::mointor_state()->set_monitoring(false);
        result_list_.push_back("monitor shutdown.");
      }
      else
      {
        result_list_.push_back("monitor is not running");
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

void wmdf::MonitorCommand::OnHelp()
{
  result_list_.push_back("--"+command_tag_);
  result_list_.push_back("   -s startup monitor");
  result_list_.push_back("   -t shutdown monitor");
  result_list_.push_back("   -h display this help");
}





