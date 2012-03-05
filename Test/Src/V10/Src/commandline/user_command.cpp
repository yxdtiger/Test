#include "user_command.h"
#include <vector>
#include "../environment/server_environment.h"
#include "../usermanager/user_manager.h"
#include "../usermanager/user.h"
#include "../environment/application_directory.h"
#include "../environment/utils.h"
#include "console_command_parser.h"
#include "../environment/file.h"
#include "../environment/strutils.h"


wmdf::UserCommand::UserCommand( string command_tag, string alias )
  :ConsoleCommand(command_tag,alias)
{

}

wmdf::UserCommand::~UserCommand( void )
{

}

void wmdf::UserCommand::Execute( const string& command_line )
{
  result_list_.clear();
  std::vector<string> parameters = ConsoleCommandParser::ParseCommandParameters(command_line);
  if (parameters.size() > 1)
  {
    if (parameters[1] == "-l")
    {
      int32_t count_before_load = ServerEnvironment::user_manager()->users_count();
      //load user
      if (File::Exists(ApplicationDirectory::UserDescriptionFile()))
      {
        Utils::Deserialize(ApplicationDirectory::UserDescriptionFile(),ServerEnvironment::user_manager());
        string str = StrUtils::formatstring(100,"user config file loaded successfully.(%d -> %d)",count_before_load,ServerEnvironment::user_manager()->users_count());
        result_list_.push_back(string(str));
      }
    }
    else if (parameters[1] == "-f")
    {
      if (parameters.size() > 2)
      {
        std::string user_name = parameters[2];
        User* user = ServerEnvironment::user_manager()->FindUser(user_name);
        if (NULL != user)
        {
          result_list_.push_back("************************************ user *************************************");
          string str = StrUtils::formatstring(100,"Name:%s\tIP:%s\tCompany:%s\n  Privilege:%d\tDeadline:%d",user->user_name().c_str(),user->ip_address().c_str()
            ,user->company().c_str(),user->is_privileged(),user->authorization_deadline());
          result_list_.push_back(string(str));
          for(int i=0;i<user->authorized_products_count();i++)
          {
            string result="    ";
            str = StrUtils::formatstring(100,"product id:%d \tmessage id:",user->authorized_products(i)->product_id());
            result+=string(str);
            for(int j=0;j<user->authorized_products(i)->message_count();j++)
            {
              str = StrUtils::formatstring(100,"%d ",user->authorized_products(i)->message_id(j));
              result+=str;
            }
            result_list_.push_back(result);
          }
          result_list_.push_back("************************************ end **************************************");
        }
        else
        {
          result_list_.push_back("can't find user named "+ user_name);
        }
      }
      else
      {
        result_list_.push_back(command_tag_+": "+"incorrect parameters");
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

void wmdf::UserCommand::OnHelp()
{
  result_list_.push_back("--"+command_tag_);
  result_list_.push_back("   -l load user config file");
  result_list_.push_back("   -f [name] find a specified user");
  result_list_.push_back("   -h display this help");
}
