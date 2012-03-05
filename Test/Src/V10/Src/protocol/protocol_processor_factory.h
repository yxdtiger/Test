#ifndef PROTOCOL_PROCESSOR_FACTORY_H
#define PROTOCOL_PROCESSOR_FACTORY_H

#include "../product/product_manager.h"
#include "protocol_processor.h"

namespace wmdf
{
  class HeaderCodec;
  class MessageCodec;

  class ProtocolProcessorFactory
  {
  public:
    ProtocolProcessorFactory(void);
    virtual ~ProtocolProcessorFactory(void);

    static ProtocolProcessor* CreateProtocolCodec(ProtocolDescription* protocolDescription);
    static MessageCodec*      CreateMessageCodec(int64_t protocol_unique_id, int16_t message_id);
    static HeaderCodec*       CreateHeaderCodec(int64_t protocol_unique_id);
  private:
	  static HeaderCodec* CreateSystemHeaderCodec(int8_t major_version,int8_t minor_version);
    static HeaderCodec* CreateMonitorHeaderCodec();
    static HeaderCodec* CreateMarketHeaderCodec();

	  static MessageCodec* Create_System_1_0_Binary_Codec(int16_t message_id);
	  static MessageCodec* Create_Monitor_1_0_Binary_Codec(int16_t message_id);

	  static MessageCodec* Create_SSEL1_1_0_Fast_Codec(int16_t message_id);
	  static MessageCodec* Create_SSEL1_1_0_Binary_Codec(int16_t message_id);

    static MessageCodec* Create_SSEL2_1_0_Fast_Codec(int16_t message_id);
    static MessageCodec* Create_SSEL2_1_0_Binary_Codec(int16_t message_id);

    static MessageCodec* Create_SSEL2_1_1_Fast_Codec(int16_t message_id);
    static MessageCodec* Create_SSEL2_1_1_Binary_Codec(int16_t message_id);

	  static MessageCodec* Create_SZSEL1_1_0_Fast_Codec(int16_t message_id);
	  static MessageCodec* Create_SZSEL1_1_0_Binary_Codec(int16_t message_id);

    static MessageCodec* Create_SZSEL2_1_0_Fast_Codec(int16_t message_id);
	  static MessageCodec* Create_SZSEL2_1_0_Binary_Codec(int16_t message_id);

    static MessageCodec* Create_SZSEL2_1_1_Fast_Codec(int16_t message_id);
    static MessageCodec* Create_SZSEL2_1_1_Binary_Codec(int16_t message_id);

	  static MessageCodec* Create_CFFEL1_1_0_Fast_Codec(int16_t message_id);
	  static MessageCodec* Create_CFFEL1_1_0_Binary_Codec(int16_t message_id);

    static MessageCodec* Create_CFFEL2_1_0_Fast_Codec(int16_t message_id);
	  static MessageCodec* Create_CFFEL2_1_0_Binary_Codec(int16_t message_id);

    static MessageCodec* Create_CFFEL2_1_1_Fast_Codec(int16_t message_id);
    static MessageCodec* Create_CFFEL2_1_1_Binary_Codec(int16_t message_id);

	  static MessageCodec* Create_SHFEL1_1_0_Fast_Codec(int16_t message_id);
	  static MessageCodec* Create_SHFEL1_1_0_Binary_Codec(int16_t message_id);

	  static MessageCodec* Create_DCEL1_1_0_Fast_Codec(int16_t message_id);
	  static MessageCodec* Create_DCEL1_1_0_Binary_Codec(int16_t message_id);

	  static MessageCodec* Create_CZCEL1_1_0_Fast_Codec(int16_t message_id);
	  static MessageCodec* Create_CZCEL1_1_0_Binary_Codec(int16_t message_id);

    static MessageCodec* Create_FTSEL1_1_0_Fast_Codec(int16_t message_id);
    static MessageCodec* Create_FTSEL1_1_0_Binary_Codec(int16_t message_id);

		static MessageCodec* Create_SWIndex_1_0_Fast_Codec(int16_t message_id);
		static MessageCodec* Create_SWIndex_1_0_Binary_Codec(int16_t message_id);

		static MessageCodec* Create_CSIndex_1_0_Fast_Codec(int16_t message_id);
		static MessageCodec* Create_CSIndex_1_0_Binary_Codec(int16_t message_id);

		static MessageCodec* Create_CIQIndex_1_0_Fast_Codec(int16_t message_id);
		static MessageCodec* Create_CIQIndex_1_0_Binary_Codec(int16_t message_id);

		static MessageCodec* Create_WindIndex_1_0_Fast_Codec(int16_t message_id);
		static MessageCodec* Create_WindIndex_1_0_Binary_Codec(int16_t message_id);
  };
}


#endif
