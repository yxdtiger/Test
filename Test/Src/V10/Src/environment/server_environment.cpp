#include "server_environment.h"
#include "file.h"
#include "utils.h"
#include "directory.h"
#include "system_config.h"
#include "../session/feed_server.h"
#include "application_directory.h"
#include "../usermanager/default_authenticator.h"
#include "../commandline/console_command_manager.h"
#include "../usermanager/user_manager.h"
#include "../product/product_manager.h"
#include "../product/product_source_manager.h"
#include "../session/dispatching_center.h"
#include "../usermanager/authenticator.h"
#include "../monitor/monitor_state.h"
#include "../session/feed_scheduler.h"
#include "../monitor/monitor_adapter.h"
#include "../monitor/logger.h"
#include "../product/adapter/adapter_guard.h"
#include "../monitor/monitor_event_collector.h"

wmdf::ConsoleCommandManager* wmdf::ServerEnvironment::console_command_manager_ = new wmdf::ConsoleCommandManager();
wmdf::UserManager* wmdf::ServerEnvironment::user_manager_ = new wmdf::UserManager();
wmdf::ProductSourceManager* wmdf::ServerEnvironment::product_source_manager_ = new wmdf::ProductSourceManager();
wmdf::SystemConfig* wmdf::ServerEnvironment::system_config_ = new wmdf::SystemConfig();
wmdf::DispatchingCenter* wmdf::ServerEnvironment::dispatching_center_ = new wmdf::DispatchingCenter();
wmdf::Authenticator* wmdf::ServerEnvironment::authenticator_ = new wmdf::DefaultAuthenticator();
wmdf::MonitorState* wmdf::ServerEnvironment::mointor_state_ = new wmdf::MonitorState();
wmdf::FeedServer* wmdf::ServerEnvironment::feed_server_ = new wmdf::FeedServer();
wmdf::FeedServer* wmdf::ServerEnvironment::monitor_server_ = new wmdf::FeedServer();
wmdf::FeedScheduler* wmdf::ServerEnvironment::feed_scheduler_ = new wmdf::FeedScheduler();
wmdf::MonitorAdapter* wmdf::ServerEnvironment::monitor_adapter_ = new wmdf::MonitorAdapter();

wmdf::ServerEnvironment::ServerEnvironment(void)
{
}

wmdf::ServerEnvironment::~ServerEnvironment(void)
{
}

void wmdf::ServerEnvironment::Initialize()
{
  //start logger;
  Logger::instance()->StartFileAppender();
  Logger::instance()->SetMonitorEventCollector(MonitorEventCollector::instance());

  //load system config
  if (File::Exists(ApplicationDirectory::SystemDescriptionFile()))
  {
    Utils::Deserialize(ApplicationDirectory::SystemDescriptionFile(),ServerEnvironment::system_config());
  }

  //load product
  if (File::Exists(ApplicationDirectory::ProductDescriptionFile()))
  {
    Utils::Deserialize(ApplicationDirectory::ProductDescriptionFile(),ServerEnvironment::product_manager());
  }
  //load user
  if (File::Exists(ApplicationDirectory::UserDescriptionFile()))
  {
    Utils::Deserialize(ApplicationDirectory::UserDescriptionFile(),ServerEnvironment::user_manager());
  }

  //load source
  if (File::Exists(ApplicationDirectory::SourceDescriptionFile()))
  {
    Utils::Deserialize(ApplicationDirectory::SourceDescriptionFile(),ServerEnvironment::product_source_manager());
  }

	if (!Directory::Exists(ApplicationDirectory::LogPath()))
	{
		Directory::CreateDirectories(ApplicationDirectory::LogPath());
	}
}

void wmdf::ServerEnvironment::StartServer()
{
  //启动数据服务
  ServerEnvironment::feed_server()->Start(ServerEnvironment::system_config()->server_port());
  Logger::instance()->WriteLog(LL_INFO,"feed server started.");
  Utils::SleepTime(100);
  if (ServerEnvironment::dispatching_center()->adapters_count() > 0)
  {
    ServerEnvironment::dispatching_center()->StartAllAdapters();
    Logger::instance()->WriteLog(LL_INFO,"all adapters started.");
    Utils::SleepTime(100);
  }

  //启动监控服务
  if (ServerEnvironment::system_config()->monitor_server_on())
  {
    ServerEnvironment::mointor_state()->set_monitoring(true);
    Utils::SleepTime(100);
  }

  //启动Adapter的守护线程
  ServerEnvironment::adapter_guard()->Startup();
  Logger::instance()->WriteLog(LL_INFO,"adapter guard started.");

  //启动服务器操作线程
  ServerEnvironment::feed_scheduler()->Startup();
  Logger::instance()->WriteLog(LL_INFO,"feed scheduler started.");
}

void wmdf::ServerEnvironment::StopServer()
{
  //停止Adapter的守护线程
  ServerEnvironment::adapter_guard()->Shutdown();

  //停止守护线程
  ServerEnvironment::feed_scheduler()->Shutdown();

  //停止所有Adapter
  ServerEnvironment::dispatching_center()->StopAllAdapters();

  //停止Feed服务
  ServerEnvironment::feed_server()->Stop();

  //停掉监控服务
  ServerEnvironment::mointor_state()->set_monitoring(false);

  //停掉日志记录
  ServerEnvironment::mointor_state()->set_logging(false);
}

wmdf::ProductManager* wmdf::ServerEnvironment::product_manager()
{
  return ProductManager::instance();
}

wmdf::AdapterGuard* wmdf::ServerEnvironment::adapter_guard()
{
  return AdapterGuard::instance();
}

void wmdf::ServerEnvironment::PrintServerInfo()
{
  printf("**********************************************************************\n");
  printf("* Wind Market Data Feed Server(wmdf) Copyright (C) 2011              *\n");
  printf("* Developers:Wind MDS Team                                           *\n");
  printf("**********************************************************************\n");
  printf("\n");
}
