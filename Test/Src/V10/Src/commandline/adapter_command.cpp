#include "adapter_command.h"
#include "../environment/server_environment.h"
#include "../environment/splitter.h"
#include "console_command_parser.h"
#include "../product/adapter/iproduct_adapter.h"
#include "../session/dispatching_center.h"
#include "../environment/strutils.h"
#include <vector>

wmdf::AdapterCommand::AdapterCommand( std::string command_tag, std::string alias )
  :ConsoleCommand(command_tag,alias)
{

}
wmdf::AdapterCommand::~AdapterCommand(void)
{
}

void wmdf::AdapterCommand::Execute( const std::string& command_line )
{
  result_list_.clear();
  std::vector<std::string> parameters = ConsoleCommandParser::ParseCommandParameters(command_line);
  if (parameters.size() > 1)
  {
    if (parameters[1] == "-s")
    {
      result_list_.push_back("index   product    ip         port   connected   description ");
      for (int32_t index =0;index<ServerEnvironment::dispatching_center()->adapters_count();index++)
      {
        IProductAdapter* pa = ServerEnvironment::dispatching_center()->adapters(index);
        ProductSource* ps = pa->product_source();
        if (ps != NULL)
        {
          std::string str = StrUtils::formatstring(100,"%d        %d     %s    %d   %s        %s",
            index,ps->product_id(),ps->ip_address().c_str(),ps->port(),ps->which_is_in_use() == 0 ? "true":"false",ps->description().c_str());
          result_list_.push_back(str);
          for(int j=0;j<ps->backup_source_count();j++)
          {
            std::string str = StrUtils::formatstring(100,"                  %s    %d   %s        ",
              ps->backup_sources(j)->ip_address().c_str(),ps->backup_sources(j)->port(),(ps->which_is_in_use() == (j+1))?"true":"false");
            result_list_.push_back(str);
          }
        }
      }
    }
    else if (parameters[1] == "-e")
    {
      if (parameters.size() > 2)
      {
        int32_t index = atoi(parameters[2].c_str());
        if ((index>=0) && (index < ServerEnvironment::dispatching_center()->adapters_count()))
        {
          if (!ServerEnvironment::dispatching_center()->adapters(index)->is_connected())
          {
            ServerEnvironment::dispatching_center()->adapters(index)->Start();
          }
          ServerEnvironment::dispatching_center()->adapters(index)->product_source()->set_enabled(true);
        }
      }
      else
      {
        result_list_.push_back(command_tag_+": "+"missing parameters");
        result_list_.push_back("try '"+command_tag_+" -h' for more information");
      }
    }
    else if (parameters[1] == "-d")
    {
      if (parameters.size() > 2)
      {
        int32_t index = atoi(parameters[2].c_str());
        if ((index>=0) && (index < ServerEnvironment::dispatching_center()->adapters_count()))
        {
          ServerEnvironment::dispatching_center()->adapters(index)->product_source()->set_enabled(false);
          if (ServerEnvironment::dispatching_center()->adapters(index)->is_connected())
          {
            ServerEnvironment::dispatching_center()->adapters(index)->Stop();
          }
        }
      }
      else
      {
        result_list_.push_back(command_tag_+": "+"missing parameters");
        result_list_.push_back("try '"+command_tag_+" -h' for more information");
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

void wmdf::AdapterCommand::OnHelp()
{
  result_list_.push_back("--"+command_tag_);
  result_list_.push_back("   -s display status of adapters");
  result_list_.push_back("   -e [adapter] enable a specified adapter");
  result_list_.push_back("   -d [adapter] disable a specified adapter");
  result_list_.push_back("   -h display this help");
}
