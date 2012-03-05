#include "monitor_state.h"
#include "../environment/server_environment.h"
#include "../environment/system_config.h"
#include "../session/feed_server.h"
#include "../session/dispatching_center.h"
#include "monitor_adapter.h"


wmdf::MonitorState::MonitorState(void)
{
  monitoring_ = false;
  logging_ = false;
}

wmdf::MonitorState::~MonitorState(void)
{

}

void wmdf::MonitorState::set_monitoring( bool monitoring )
{
  monitoring_ = monitoring;
  if (monitoring_)
  {
    ServerEnvironment::monitor_server()->Start(ServerEnvironment::system_config()->monitor_server_port());
    ServerEnvironment::monitor_adapter()->Start();
  }
  else
  {
    ServerEnvironment::monitor_server()->Stop();
    ServerEnvironment::monitor_adapter()->Stop();
  }
}

void wmdf::MonitorState::set_logging( bool logging )
{
  logging_ = logging;
  if (logging_)
  {
    ServerEnvironment::dispatching_center()->StartAllLoggers();
  }
  else
  {
    ServerEnvironment::dispatching_center()->StopAllLoggers();
  }
}
