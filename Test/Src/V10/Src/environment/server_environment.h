#ifndef SERVERENVIRONMENT_H
#define SERVERENVIRONMENT_H


namespace wmdf
{

  class FeedServer;
  class ConsoleCommandManager;
  class UserManager;
  class ProductManager;
  class ProductSourceManager;
  class SystemConfig;
  class DispatchingCenter;
  class Authenticator;
  class MonitorState;
  class MonitorAdapter;
  class FeedScheduler;
  class AdapterGuard;

  class ServerEnvironment
  {
  public:
	  ServerEnvironment(void);
	  ~ServerEnvironment(void);
    static void Initialize();
    static void StartServer();
    static void StopServer();
    static void PrintServerInfo();
    static FeedServer* feed_server() {return feed_server_;}
    static FeedServer* monitor_server() {return monitor_server_;}
    static ConsoleCommandManager* console_command_manager() {return console_command_manager_;}
    static UserManager* user_manager() {return user_manager_;}
    static ProductManager* product_manager();
    static ProductSourceManager* product_source_manager() {return product_source_manager_;}
    static SystemConfig* system_config() {return system_config_;}
    static DispatchingCenter* dispatching_center() {return dispatching_center_;}
    static Authenticator* authenticator(){return authenticator_;}
    static MonitorState* mointor_state() { return mointor_state_;}
    static MonitorAdapter* monitor_adapter() { return monitor_adapter_;}
    static FeedScheduler* feed_scheduler() { return feed_scheduler_; }
    static AdapterGuard* adapter_guard();
  private:
    static FeedServer* feed_server_;
    static ConsoleCommandManager* console_command_manager_;
    static UserManager* user_manager_;
    static ProductSourceManager* product_source_manager_;
    static SystemConfig* system_config_;
    static DispatchingCenter* dispatching_center_;
    static Authenticator* authenticator_;
    static FeedServer* monitor_server_;
    static MonitorState* mointor_state_;
    static FeedScheduler* feed_scheduler_;
    static MonitorAdapter* monitor_adapter_;
  };
}

#endif

