#include "console_command_parser.h"
#include "../environment/splitter.h"
#include "console_command_define.h"
#include "../include/wmdf_api.h"
#include <assert.h>

using namespace std;

namespace wmdf
{
  ConsoleCommandParser::ConsoleCommandParser(void)
  {
  }

  ConsoleCommandParser::~ConsoleCommandParser(void)
  {
  }

  string ConsoleCommandParser::ParseCommandName(const string& command_line)
  {
      Splitter splitter(command_line,CONSOLECOMMAND_DELIMITER);
      assert(splitter.size() >= 1);
      return splitter[0];
  }

  vector<string> ConsoleCommandParser::ParseCommandParameters(const string& command_line)
  {
    Splitter splitter(command_line,CONSOLECOMMAND_DELIMITER);
    assert(splitter.size() >= 1);
    vector<string> parameters;
    for (uint32_t index=0;index < splitter.size();index++)
    {
      parameters.push_back(splitter[index]);
    }
    return parameters;
  }
}
