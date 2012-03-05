#include "system_config.h"

wmdf::SystemConfig::SystemConfig(void)
{
}

wmdf::SystemConfig::~SystemConfig(void)
{
}




bool wmdf::SystemConfig::SaveState( Memento* m )
{
  m->SetName("system");
  Memento* sub_m=m->CreateChild("Server_Port");
  sub_m->SetInteger("value",server_port_);
  sub_m=m->CreateChild("Server_Thread_Num");
  sub_m->SetInteger("value",server_thread_num_);
  sub_m=m->CreateChild("Max_Connection_Count");
  sub_m->SetInteger("value",max_connection_count_);
  sub_m=m->CreateChild("Heart_Beat_Interval");
  sub_m->SetInteger("value",heart_beat_interval_);

  sub_m=m->CreateChild("Max_Packet_Size");
  sub_m->SetInteger("value",max_packet_size_);
  sub_m=m->CreateChild("Max_Cache_Msg_Count");
  sub_m->SetInteger("value",max_cache_msg_count_);

  sub_m=m->CreateChild("Server_Reset_Operation_Hour");
  sub_m->SetInteger("value",server_reset_operation_hour_);
  sub_m=m->CreateChild("Pre_Market_Operation_Hour");
  sub_m->SetInteger("value",pre_market_operation_hour_);

  sub_m=m->CreateChild("After_Market_Operation_Hour");
  sub_m->SetInteger("value",after_market_operation_hour_);


  sub_m=m->CreateChild("Monitor_Server_Port");
  sub_m->SetInteger("value",monitor_server_port_);
  sub_m=m->CreateChild("Monitor_Server_On");
  sub_m->SetBoolean("value",monitor_server_on_);
  sub_m=m->CreateChild("Monitor_Interval");
  sub_m->SetInteger("value",monitor_interval_);

  return true;
}

bool wmdf::SystemConfig::LoadState( Memento* m )
{
  for (int32_t i=0;i<m->GetChildCount();i++)
  {
    Memento* sub_m=m->GetChild(i);
    if (sub_m->GetName()=="Server_Port")
    {
      server_port_=sub_m->GetInteger("value");
    }
    else if (sub_m->GetName()=="Server_Thread_Num")
    {
      server_thread_num_=sub_m->GetInteger("value");
    }
    else if (sub_m->GetName()=="Max_Connection_Count")
    {
      max_connection_count_=sub_m->GetInteger("value");
    }
    else if (sub_m->GetName()=="Heart_Beat_Interval")
    {
      heart_beat_interval_=sub_m->GetInteger("value");
    }
    else if (sub_m->GetName()=="Max_Cache_Msg_Count")
    {
      max_cache_msg_count_=sub_m->GetInteger("value");
    }
    else if (sub_m->GetName()=="Max_Packet_Size")
    {
      max_packet_size_=sub_m->GetInteger("value");
    }
    else if (sub_m->GetName()=="Monitor_Server_Port")
    {
      monitor_server_port_=sub_m->GetInteger("value");
    }
    else if (sub_m->GetName()=="Monitor_Server_On")
    {
      monitor_server_on_ = sub_m->GetBoolean("value");
    }
    else if (sub_m->GetName()=="Monitor_Interval")
    {
      monitor_interval_ = sub_m->GetInteger("value");
    }
    else if (sub_m->GetName()=="Server_Reset_Operation_Hour")
    {
      server_reset_operation_hour_ = sub_m->GetInteger("value");
    }
    else if (sub_m->GetName()=="Pre_Market_Operation_Hour")
    {
      pre_market_operation_hour_ = sub_m->GetInteger("value");
    }
    else if (sub_m->GetName()=="After_Market_Operation_Hour")
    {
      after_market_operation_hour_ = sub_m->GetInteger("value");
    }
  }
  return true;
}
