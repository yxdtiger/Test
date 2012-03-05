#include "test_command.h"
#include "console_command_parser.h"
#include "../protocol/test/testCodec.h"

wmdf::TestCommand::TestCommand( string command_tag , string alias )
 :ConsoleCommand(command_tag,alias)
{

}

wmdf::TestCommand::~TestCommand(void)
{
}

void wmdf::TestCommand::Execute( const string& command_line )
{
  result_list_.clear();
  vector<string> parameters = ConsoleCommandParser::ParseCommandParameters(command_line);
  if (parameters.size() > 1)
  {
    if(parameters[1] == "-c")
    {
      TestCodec::RunTest();
    }

    else if (parameters[1] == "-h")
    {
      OnHelp();
    }
  }
  else
  {
    result_list_.push_back(command_tag_+": "+"missing parameters");
    result_list_.push_back("try '"+command_tag_+" -h' for more information");
  }
}

void wmdf::TestCommand::OnHelp()
{
  result_list_.push_back("--"+command_tag_);
  result_list_.push_back("   -c test codec");
  result_list_.push_back("   -h display this help");
}