#ifndef PROTOCOL_PROCESSOR_H
#define PROTOCOL_PROCESSOR_H

#include <map>
#include "mdf_message.h"
#include "net_module.h"
#include "protocol_description.h"

namespace wmdf
{
  class HeaderCodec;
  class MessageCodec;

  class ProtocolProcessor
  {
  public:
    ProtocolProcessor(ProtocolDescription* protocol_description);
    virtual ~ProtocolProcessor(void);
    Message* Encode(SharedMdfMessage message);
    Message* Encode(SharedMdfMessage message,MessageCodec* message_codec);
    MdfMessage* Decode(Message* raw_message);
    MdfRecord* DecodeToRecord(Message* raw_message);
    void AddMessageCodec(int16_t message_id,MessageCodec* message_codec);
    void set_protocol_description(ProtocolDescription* protocol_description);
    void set_header_codec(HeaderCodec* header_codec);
    int64_t unique_id() {return protocol_description_->unique_id();}
    int16_t product_id(){return protocol_description_->product_id();}
  private:
    inline MessageCodec* GetMessageCodec(int16_t message_id);
    ProtocolDescription* protocol_description_;
    HeaderCodec* header_codec_;
    //messageid,messagecodec;
    std::map<int16_t,MessageCodec*> message_codecs_;
  };
}

#endif
