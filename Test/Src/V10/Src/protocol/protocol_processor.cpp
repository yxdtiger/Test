#include "protocol_processor.h"
#include "../include/date_time.h"
#include "header_codec.h"
#include "message_codec.h"

using namespace std;

wmdf::ProtocolProcessor::ProtocolProcessor(ProtocolDescription* protocol_description)
{
  protocol_description_ = protocol_description;
}

wmdf::ProtocolProcessor::~ProtocolProcessor(void)
{
  map<int16_t,MessageCodec*>::iterator iter = message_codecs_.begin();
  while(iter != message_codecs_.end())
  {
    delete iter->second;
    iter++;
  }
  message_codecs_.clear();

  if (protocol_description_ != NULL)
  {
    delete protocol_description_;
    protocol_description_ = NULL;
  }
  if (header_codec_ != NULL)
  {
    delete header_codec_;
    header_codec_ = NULL;
  }
}

void wmdf::ProtocolProcessor::AddMessageCodec( int16_t message_id,MessageCodec* message_codec )
{
  message_codecs_.insert(make_pair(message_id,message_codec));
}

void wmdf::ProtocolProcessor::set_protocol_description( ProtocolDescription* protocol_description )
{
  protocol_description_ = protocol_description; 
}

void wmdf::ProtocolProcessor::set_header_codec( HeaderCodec* header_codec )
{
  header_codec_ = header_codec;
  
}

Message* wmdf::ProtocolProcessor::Encode( SharedMdfMessage message )
{
  MessageCodec* codec = GetMessageCodec(message->message_id());
  assert(NULL != codec);
  int header_size = header_codec_->EncodeData();
  int buffer_size = codec->EncodeData(message->body(),message->record_count());
  Message* raw_message = CreateMessage(header_size+buffer_size);
  raw_message->local_time = message->local_time();
  raw_message->packet_num = message->packet_num();
  raw_message->product_id = message->product_id();
  raw_message->message_id = message->message_id();
  if (header_size > 0)
  {
    header_codec_->DequeueBuffer(raw_message->data);
  }
  if (buffer_size > 0)
  {
    codec->DequeueBuffer(raw_message->data+header_size);
  }
  return raw_message;
}

Message* wmdf::ProtocolProcessor::Encode( SharedMdfMessage message,MessageCodec* message_codec )
{
  int header_size = header_codec_->EncodeData();
  int buffer_size = message_codec->EncodeData(message->body(),message->record_count());
  Message* raw_message = CreateMessage(header_size+buffer_size);
  raw_message->local_time = message->local_time();
  raw_message->packet_num = message->packet_num();
  raw_message->product_id = message->product_id();
  raw_message->message_id = message->message_id();
  if (header_size > 0)
  {
    header_codec_->DequeueBuffer(raw_message->data);
  }
  if (buffer_size > 0)
  {
    message_codec->DequeueBuffer(raw_message->data+header_size);
  }
  return raw_message;
}
wmdf::MdfMessage* wmdf::ProtocolProcessor::Decode(Message* raw_message )
{
  MdfMessage* mdf_message=NULL;
  MessageCodec* decoder = GetMessageCodec(raw_message->message_id);
  assert(decoder != NULL);
  if (NULL == decoder) return NULL;
  int32_t head_length=0;
  void* header = header_codec_->DecodeData(raw_message->data,head_length);
  int32_t record_count = 0;
  void* record = decoder->DecodeData(raw_message->data+head_length,raw_message->msg_len-MSG_HEADER_LEN-head_length,record_count);
  if (record_count > 0)
  {
    mdf_message = new MdfMessage();
    mdf_message->set_local_time(raw_message->local_time);
    mdf_message->set_packet_num(raw_message->packet_num);
    mdf_message->set_product_id(raw_message->product_id);
    mdf_message->set_message_id(raw_message->message_id);
    mdf_message->set_major_version(protocol_description_->major_version());
    mdf_message->set_minor_version(protocol_description_->minor_version());
    mdf_message->set_header(header);
    mdf_message->set_record_count(record_count);
    mdf_message->set_body(record);
    return mdf_message;
  }
  else
  {
    delete[] record;
  }
  return NULL;
}

MdfRecord* wmdf::ProtocolProcessor::DecodeToRecord( Message* raw_message )
{
  MdfRecord* mdf_record=NULL;
  MessageCodec* decoder = GetMessageCodec(raw_message->message_id);
  assert(decoder != NULL);
  if (NULL == decoder) return NULL;
  int32_t head_length=0;
  void* header = header_codec_->DecodeData(raw_message->data,head_length);
  int32_t record_count = 0;
  void* record = decoder->DecodeData(raw_message->data+head_length,raw_message->msg_len-MSG_HEADER_LEN-head_length,record_count);
  if (record_count > 0)
  {
    mdf_record = new MdfRecord();
    mdf_record->local_time = raw_message->local_time;
    mdf_record->packet_num = raw_message->packet_num;
    mdf_record->product_id = raw_message->product_id;
    mdf_record->message_id = raw_message->message_id;
    mdf_record->major_version = protocol_description_->major_version();
    mdf_record->minor_version = protocol_description_->minor_version();
    mdf_record->header = header;
    mdf_record->record_count = record_count;
    mdf_record->body = record;
    return mdf_record;
  }
  else
  {
    delete[] record;
  }
  return NULL;
}

wmdf::MessageCodec* wmdf::ProtocolProcessor::GetMessageCodec( int16_t message_id )
{
  map<int16_t,MessageCodec*>::iterator iter = message_codecs_.find(message_id);
  if (iter != message_codecs_.end())
  {
    return iter->second;
  }
  return NULL;
}

