#include "net_command.h"
#include <vector>
#include "../environment/server_environment.h"
#include "../environment/splitter.h"
#include "console_command_parser.h"
#include "../monitor/monitor_state.h"
#include "../monitor/monitor_adapter.h"
#include "../monitor/client_status_collector.h"
#include "../monitor/transfer_status_collector.h"
#include "../session/feed_server.h"
#include "../environment/strutils.h"
#include "../environment/system_config.h"

using namespace std;
wmdf::NetCommand::NetCommand( string command_tag, string alias )
:ConsoleCommand(command_tag,alias)
{

}

wmdf::NetCommand::~NetCommand( void )
{

}

void wmdf::NetCommand::Execute( const string& command_line )
{
  result_list_.clear();
  vector<string> parameters = ConsoleCommandParser::ParseCommandParameters(command_line);
  if (parameters.size() > 1)
  {
    if (parameters[1] == "-c")
    {
      ServerEnvironment::monitor_adapter()->client_status_collector()->PrintClientStatus();
    }
    else if (parameters[1] == "-t")
    {
      ServerEnvironment::monitor_adapter()->transfer_status_collector()->PrintTransferStatus();
    }
    else if (parameters[1] == "-k")
    {
      if (parameters.size() > 2)
      {
        int32_t socket_id= atoi(parameters[2].c_str());
        ServerEnvironment::feed_server()->KickSubscriber(socket_id);
        result_list_.push_back("client successfully kicked.");
      }
      else
      {
        result_list_.push_back(command_tag_+": "+"incorrect parameters");
        result_list_.push_back("try '"+command_tag_+" -h' for more information");
      }
    }
    else if (parameters[1] == "-s")
    {
      SystemConfig* system_config = ServerEnvironment::system_config();
      if (NULL != system_config)
      {

        result_list_.push_back("********************************** server info ********************************");

        std::string str = StrUtils::formatstring(100," Server->Port:%d\tThread Num:%d\tMax Connection:%d",
          system_config->server_port(),system_config->server_thread_num(),system_config->max_connection_count());
        result_list_.push_back(str);

        str = StrUtils::formatstring(100," HeartBeatInterval:%d\tMaxPacketSize:%d\tMaxCacheMsgCount:%d",system_config->heart_beat_interval(),system_config->max_packet_size(),system_config->max_cache_msg_count());
        result_list_.push_back(str);

        str = StrUtils::formatstring(100," PreOpenHour:%d\tCloseMarketHour:%d\tResetServerHour:%d",system_config->pre_market_operation_hour(),system_config->after_market_operation_hour(),system_config->server_reset_operation_hour());
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

void wmdf::NetCommand::OnHelp()
{
  result_list_.push_back("--"+command_tag_);
  result_list_.push_back("   -a display status of each adapter");
  result_list_.push_back("   -c display status of each client");
  result_list_.push_back("   -t display transfer status of each product");
  result_list_.push_back("   -k [num] kick a specified client");
  result_list_.push_back("   -s display server config");
  result_list_.push_back("   -h display this help");
}
