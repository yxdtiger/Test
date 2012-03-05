#ifndef SYSTEM_MESSAGE_PROCESSOR_FACTORY_H
#define SYSTEM_MESSAGE_PROCESSOR_FACTORY_H

namespace wmdf
{
  class SystemMessageProcessor;
  class SystemMessageProcessorCenter;
  class SystemMessageProcessorFactory
  {
  public:
    SystemMessageProcessorFactory(void);
    virtual ~SystemMessageProcessorFactory(void);
    static void InitSysMessageProcessorCenter(SystemMessageProcessorCenter* sys_msg_processor_center);
  private:
    static void Create_System_1_0_Handles(SystemMessageProcessor* systemMessageProcessor);
    static void CreateSysMessageHandles(SystemMessageProcessor* systemMessageProcessor);
  };
}

#endif
