#include "monitor_environment.h"
#include "serializer_utils.h"
#include "../../environment/application_directory.h"
#include "../../environment/utils.h"
#include "../../environment/strutils.h"
#include "../../include/wmdf_api.h"
#include "../common/studio_utility.h"


using namespace std;
using namespace wmdf;

wmdf::MonitorEnvironment* wmdf::MonitorEnvironment::instance_ = NULL;  

wmdf::MonitorEnvironment::MonitorEnvironment(void)
{
  is_sound_enable_ = true;
  invoker_ = new MonitorInvoke();
   watch_dog_ = new WatchDog(5000);
}

wmdf::MonitorEnvironment::~MonitorEnvironment(void)
{
  if(NULL!=watch_dog_)
  {
    delete watch_dog_;
    watch_dog_ = NULL;
  }
  if(invoker_!=NULL)
  {
    invoker_->Disconnect();
    delete invoker_;
    invoker_ = NULL;
  }
}

void wmdf::MonitorEnvironment::Initialize()
{
  trading_time_manager_ = new TradingTimeManager();  
  string filePath = ApplicationDirectory::ApplicationPath()+std::string("market_times.xml");
  if (StudioUtility::IsFileExist(filePath.c_str()))
  {
    SerializerUtils::Deserialize(filePath,trading_time_manager_);   
  }
}

void wmdf::MonitorEnvironment::Finalize()
{
  string filePath = ApplicationDirectory::ApplicationPath()+std::string("market_times.xml");
  SerializerUtils::Serialize(filePath,trading_time_manager_);
  delete trading_time_manager_;
  trading_time_manager_ = NULL;
}

void wmdf::MonitorEnvironment::StartMonitor()
{
  if(!watch_dog_->IsWorking())
    watch_dog_->StartWork();
  invoker_->Connect();
}

void wmdf::MonitorEnvironment::StartMonitorOne( int32_t conn_id )
{
  invoker_->ConnectOne(conn_id);
  if(!watch_dog_->IsWorking())
    watch_dog_->StartWork();
}

void wmdf::MonitorEnvironment::EndMonitor()
{
  watch_dog_->Stop();
  invoker_->Disconnect();
}

void wmdf::MonitorEnvironment::StopMonitorOne(int32_t conn_id)
{
  invoker_->DisconnectOne(conn_id);
}

MonitorEnvironment* MonitorEnvironment::Instance()
{
  if(NULL == instance_)  
    instance_ = new MonitorEnvironment();  
  return instance_;
}




