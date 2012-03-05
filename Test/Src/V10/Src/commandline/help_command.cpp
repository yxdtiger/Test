#include "help_command.h"
#include <vector>
#include "console_command_parser.h"
#include "../environment/server_environment.h"
#include "console_command_manager.h"

using namespace std;

wmdf::HelpCommand::HelpCommand(string command_tag, string alias)
:ConsoleCommand(command_tag,alias)
{
}

wmdf::HelpCommand::~HelpCommand(void)
{
}

void wmdf::HelpCommand::Execute( const string& command_line )
{
  result_list_.clear();
  std::vector<string> parameters = ConsoleCommandParser::ParseCommandParameters(command_line);
  if (parameters.size() > 0)
  {
    ServerEnvironment::console_command_manager()->OnHelp();
  }
}

void wmdf::HelpCommand::OnHelp()
{
  result_list_.push_back("--"+command_tag_);
  result_list_.push_back("   display help of commands");
}
