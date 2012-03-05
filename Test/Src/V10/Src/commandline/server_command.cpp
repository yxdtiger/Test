#include "server_command.h"
#include <vector>
#include "../environment/server_environment.h"
#include "../environment/system_config.h"
#include "../environment/application_directory.h"
#include "../environment/utils.h"
#include "console_command_parser.h"
#include "../environment/file.h"
#include "../environment/strutils.h"

wmdf::ServerCommand::ServerCommand( string command_tag, string alias )
  :ConsoleCommand(command_tag,alias)
{

}

wmdf::ServerCommand::~ServerCommand( void )
{

}

void wmdf::ServerCommand::Execute( const string& command_line )
{
  result_list_.clear();
  std::vector<string> parameters = ConsoleCommandParser::ParseCommandParameters(command_line);
  if (parameters.size() > 1)
  { 
    if (parameters[1] == "-c")
    {      
      SystemConfig* system_config = ServerEnvironment::system_config();      
        if (NULL != system_config)
        {
          
          result_list_.push_back("********************************** server info ********************************");
          
          std::string str = StrUtils::formatstring(100," Server->Port:%d\tThread Num:%d\tMax Connection:%d",
            system_config->server_port(),system_config->server_thread_num(),system_config->max_connection_count());          
          result_list_.push_back(str);
          
          str = StrUtils::formatstring(100," HeartBeatInterval:%d\n MaxPacketSize:%d\tMaxCacheMsgCount:%d",system_config->heart_beat_interval(),system_config->max_packet_size(),system_config->max_cache_msg_count());
          result_list_.push_back(str);

          str = StrUtils::formatstring(100," ServerResetOperationHour:%d\tPreMarketOperationHour:%d",system_config->server_reset_operation_hour(),system_config->pre_market_operation_hour());
          result_list_.push_back(str);
          
          str = StrUtils::formatstring(100," MonitorServerOn:%d\tMonitorServerPort:%d\tMonitorInterval:%d",system_config->monitor_server_on(),system_config->monitor_server_port(),system_config->monitor_interval());
          result_list_.push_back(str);

          result_list_.push_back("************************************ end **************************************");
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

void wmdf::ServerCommand::OnHelp()
{
  result_list_.push_back("-c display the server config");
  result_list_.push_back("-h display this help");
}