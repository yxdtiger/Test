#ifndef CONSOLE_COMMAND_DEFINE_H
#define CONSOLE_COMMAND_DEFINE_H

namespace wmdf
{
  static const char* CONSOLECOMMAND_PRELOAD_SYMBOL = ">";

  static const char* CONSOLECOMMAND_DELIMITER = " ";

  static const char* NULL_CONSOLECOMMAND = "";

  static const char* SYSTEM_CONSOLECOMMAND = "system";

  static const char* QUIT_CONSOLECOMMAND = "quit";

  static const char* QUIT_CONSOLECOMMAND_ALIAS = "-q";

  static const char* BACK_CONSOLECOMMAND = "\\\\";

  static const char* CLS_CONSOLECOMMAND = "clear";
  static const char* CLS_CONSOLECOMMAND_ALIAS = "cls";

  static const char* HELP_CONSOLECOMMAND = "help";

  static const char* HELP_CONSOLECOMMAND_ALIAS = "?";

  //监控开关命令
  static const char* MONITOR_CONSOLECOMMAND = "monitor";
  static const char* MONITOR_CONSOLECOMMAND_ALIAS = "-m";
  //日志开关命令
  static const char* LOGGER_CONSOLECOMMAND = "logger";
  static const char* LOGGER_CONSOLECOMMAND_ALIAS = "-l";

  //网络通信相关命令
  static const char* NET_CONSOLECOMMAND = "net";
  static const char* NET_CONSOLECOMMAND_ALIAS = "-n";

  //用户信息命令
  static const char* USER_CONSOLECOMMAND = "user";
  static const char* USER_CONSOLECOMMAND_ALIAS = "-u";

  //adapter状态信息
  static const char* ADAPTER_CONSOLECOMMAND = "adapter";
  static const char* ADAPTER_CONSOLECOMMAND_ALIAS = "-a";

  //replay data record by mdf 回放命令
  static const char*  FREPLAY_CONSOLECOMMAND = "freplay";
  static const char*  FREPLAY_CONSOLECOMMAND_ALIAS = "-fr";

  //replay rawdata of exchange回放命令
  static const char*  RREPLAY_CONSOLECOMMAND = "rreplay";
  static const char*  RREPLAY_CONSOLECOMMAND_ALIAS = "-rr";
}

#endif
