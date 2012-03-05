#ifndef MDFMESSAGE_H
#define MDFMESSAGE_H

#include <boost/shared_ptr.hpp>
#include "../include/wmdf_api.h"

namespace wmdf
{
  class MdfMessage
  {
  public:
    MdfMessage();
    MdfMessage(MdfRecord* mdf_record);
    ~MdfMessage();
    int32_t socket_id(){return socket_id_;}  // product record count;
    void set_socket_id(int32_t socket_id){socket_id_ = socket_id;}
		uint32_t packet_num(){return mdf_record_->packet_num;}  // get packet_num
		void set_packet_num(uint32_t packet_num){mdf_record_->packet_num = packet_num;}
    time_stamp_t local_time() {return mdf_record_->local_time;}  // local time;
    void set_local_time(time_stamp_t local_time){mdf_record_->local_time= local_time;}
    int16_t product_id() {return mdf_record_->product_id;}       // product id;
    void set_product_id(int16_t product_id){mdf_record_->product_id = product_id;}
    int16_t message_id(){return mdf_record_->message_id;}      // message id;
    void set_message_id(int16_t message_id){mdf_record_->message_id = message_id;}
    int8_t major_version(){return mdf_record_->major_version;}  // major version
    void set_major_version(int8_t major_version){mdf_record_->major_version = major_version;}
    int8_t minor_version(){return mdf_record_->minor_version;}  // minor version
    void set_minor_version(int8_t minor_version){mdf_record_->minor_version = minor_version;}
    void* header(){return mdf_record_->header;}                // product header;
    void set_header(void* header){mdf_record_->header = header;}
    int32_t record_count(){return mdf_record_->record_count;}  // product record count;
    void set_record_count(int32_t record_count){mdf_record_->record_count = record_count;}
    void* body(){return mdf_record_->body;}                     // product record;
    void set_body(void* body){mdf_record_->body = body;}
  private:
    int32_t socket_id_;      // socket id,if the message need to be broadcast it is 0;
    MdfRecord* mdf_record_;

  };

  typedef boost::shared_ptr<MdfMessage> SharedMdfMessage;


  class RelayMdfMessage
  {
  public:
    RelayMdfMessage();
    ~RelayMdfMessage();
  public:
    time_stamp_t local_time;// local time;
		uint32_t packet_num;
    int32_t socket_id;      // socket id,if the message need to be broadcast it is 0;
    int16_t product_id;     // product id;
    int16_t message_id;     // message id;
    int8_t major_version;   // major version
    int8_t minor_version;   // minor version
    int32_t length;
    int8_t* data;
  };

  typedef boost::shared_ptr<RelayMdfMessage> SharedRelayMdfMessage;
}

#endif
