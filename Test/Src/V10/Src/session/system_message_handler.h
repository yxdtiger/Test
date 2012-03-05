#ifndef SYSTEM_MESSAGE_HANDLER_H
#define SYSTEM_MESSAGE_HANDLER_H
#include "../protocol/mdf_message.h"

namespace wmdf
{
  
  class FeedServer;
  class ProtocolProcessor;
  class SystemMessageHandler
  {
  public:
    SystemMessageHandler(void);
    virtual ~SystemMessageHandler(void);
    virtual void ProcessMessage(SharedMdfMessage message,FeedServer* feed_server,ProtocolProcessor* processor)=0;
  protected:
    SharedMdfMessage CreateMdfMessage(SharedMdfMessage message, void* record, int16_t message_id);
    void SendResponseMessage( SharedMdfMessage mdf_message, ProtocolProcessor* processor, FeedServer* feed_server );
  };

  class LoginRequest_V_1_0_Handle:public SystemMessageHandler
  {
  public:
    LoginRequest_V_1_0_Handle(void);
    virtual ~LoginRequest_V_1_0_Handle(void);
    virtual void ProcessMessage(SharedMdfMessage message,FeedServer* feed_server,ProtocolProcessor* processor);
  private:
    //void AddSubscriber(LoginResponse* response);
  };

  class Logout_V_1_0_Handle:public SystemMessageHandler
  {
  public:
    Logout_V_1_0_Handle(void);
    virtual ~Logout_V_1_0_Handle(void);
    virtual void ProcessMessage(SharedMdfMessage message,FeedServer* feed_server,ProtocolProcessor* processor);
  };

  class FetchConfigInfo_V_1_0_Handle:public SystemMessageHandler
  {
  public:
    FetchConfigInfo_V_1_0_Handle(void);
    virtual ~FetchConfigInfo_V_1_0_Handle(void);
    virtual void ProcessMessage(SharedMdfMessage message,FeedServer* feed_server,ProtocolProcessor* processor);
  };

  class UpdateConfigInfo_V_1_0_Handle:public SystemMessageHandler
  {
  public:
    UpdateConfigInfo_V_1_0_Handle(void);
    virtual ~UpdateConfigInfo_V_1_0_Handle(void);
    virtual void ProcessMessage(SharedMdfMessage message,FeedServer* feed_server,ProtocolProcessor* processor);
  };
}

#endif
