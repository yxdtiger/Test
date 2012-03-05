#include "freplay_command.h"
#include <vector>
#include "console_command_parser.h"
#include "../environment/server_environment.h"
#include "../product/feed_product_replayer.h"
#include "../environment/file.h"

using namespace std;

wmdf::FReplayCommand::FReplayCommand( string command_tag, string alias )
  :ConsoleCommand(command_tag,alias)
{

}

wmdf::FReplayCommand::~FReplayCommand( void )
{

}

void wmdf::FReplayCommand::Execute( const string& command_line )
{
  result_list_.clear();
  vector<string> parameters = ConsoleCommandParser::ParseCommandParameters(command_line);
  if (parameters.size() > 1)
  {
    if (parameters[1] == "-s")
    {
      if (parameters.size() > 3)  // 5 parameters
      {
        if (!FeedProductReplayer::instance()->is_replaying())
        {
          if (File::Exists(parameters[2]))
          {
            FeedProductReplayer::instance()->StartReplay(parameters[2],atof(parameters[3].c_str()));
            result_list_.push_back("replay started.");
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
      if (FeedProductReplayer::instance()->is_replaying())
      {
        FeedProductReplayer::instance()->PauseReplay();
        result_list_.push_back("replay paused");
      }
      else
      {
        result_list_.push_back("no replay,please start first.");
      }
    }
    else if (parameters[1] == "-r")
    {
      if (((FeedProductReplayer::instance()->is_replaying()) && FeedProductReplayer::instance()->is_paused()))
      {
        FeedProductReplayer::instance()->ResumeReplay();
        result_list_.push_back("replay resumed");
      }
    }
    else if (parameters[1] == "-t")
    {
      if (FeedProductReplayer::instance()->is_replaying())
      {
        FeedProductReplayer::instance()->TerminateReplay();
        result_list_.push_back("replay terminated");
      }
      else
      {
        result_list_.push_back("no replay,please start first.");
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

void wmdf::FReplayCommand::OnHelp()
{
  result_list_.push_back("--"+command_tag_);
  result_list_.push_back("   -s [file] [speed] --start replay a log file.");
  result_list_.push_back("   -p                --pause replay");
  result_list_.push_back("   -r                --resume replay");
  result_list_.push_back("   -t                --terminate replay");
  result_list_.push_back("   -h                --display this help");
}
