#include "replay_command.h"
#include <vector>
#include "console_command_parser.h"
#include "../environment/server_environment.h"
#include "../product/feed_product_replayer.h"
#include "../product/rawdata_product_replayer.h"
#include "../environment/file.h"

using namespace std;

wmdf::RReplayCommand::RReplayCommand( std::string command_tag, std::string alias )
 :ConsoleCommand(command_tag,alias)
{

}

wmdf::RReplayCommand::~RReplayCommand( void )
{

}

void wmdf::RReplayCommand::Execute( const std::string& command_line )
{
  result_list_.clear();
  vector<string> parameters = ConsoleCommandParser::ParseCommandParameters(command_line);
  if (parameters.size() > 1)
  {
    if (parameters[1] == "-s")
    {
      if (parameters.size() > 4)  // 4 parameters + command
      {
        if (!RawDataProductReplayer::instance()->is_replaying())
        {
          if (File::Exists(parameters[3]))
          {
            RawDataProductReplayer::instance()->StartReplay(atoi(parameters[2].c_str()),parameters[3], atoi(parameters[4].c_str()));
            result_list_.push_back("replay started");
          }
          else
          {
            result_list_.push_back("replay file not exist.");
          }
        }
        else
        {
          result_list_.push_back("replay already started.");
        }
      }
      else
      {
        result_list_.push_back(command_tag_+": "+"incorrect parameters");
        result_list_.push_back("try '"+command_tag_+" -h' for more information");
      }
    }
    else if (parameters[1] == "-p")
    {
      if (RawDataProductReplayer::instance()->is_replaying())
      {
        RawDataProductReplayer::instance()->PauseReplay();
        result_list_.push_back("replay paused");
      }
      else
      {
        result_list_.push_back("no replay,please start first.");
      }
    }
    else if (parameters[1] == "-r")
    {
      if (((RawDataProductReplayer::instance()->is_replaying()) && RawDataProductReplayer::instance()->is_paused()))
      {
        RawDataProductReplayer::instance()->ResumeReplay();
        result_list_.push_back("replay resumed");
      }
    }
    else if (parameters[1] == "-t")
    {
      if (RawDataProductReplayer::instance()->is_replaying())
      {
        RawDataProductReplayer::instance()->TerminateReplay();
        result_list_.push_back("replay terminated");
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

void wmdf::RReplayCommand::OnHelp()
{
  result_list_.push_back("--"+command_tag_);
  result_list_.push_back("   -s [product_id] [file] [delay] start replay a log file.");
  result_list_.push_back("   -p pause replay");
  result_list_.push_back("   -r resume replay");
  result_list_.push_back("   -t terminate replay");
  result_list_.push_back("   -h display this help");
}
