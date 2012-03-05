#ifndef PROTOCOL_DESCRIPTION_H
#define PROTOCOL_DESCRIPTION_H

#include "persistable.h"
#include "../include/wmdf_api.h"

namespace wmdf
{
  class Memento;
  class ProtocolDescription : public Persistable
  {
  public:
    ProtocolDescription(void);
    ProtocolDescription(ProtocolDescription* src); 
    ProtocolDescription(int16_t productId,int8_t majorVersion,int8_t minorVersion,CodecType codecType);

    virtual ~ProtocolDescription();
    virtual bool LoadState(Memento* m);
    virtual bool SaveState(Memento* m);
    int16_t product_id() {return product_id_;}
    void set_product_id(int16_t product_id) {product_id_ = product_id;}
    int8_t major_version() {return major_version_;}
    void set_major_version(int8_t major_version) {major_version_ = major_version;}
    int8_t minor_version() {return minor_version_;}
    void set_minor_version(int8_t minor_version) {minor_version_ = minor_version;}
    CodecType codec_type() {return codec_type_;}
    void set_codec_type(CodecType codec_type) {codec_type_ = codec_type;}
    int64_t unique_id()
    { 
      return MAKE_UNIQUE_ID(product_id_,major_version_,minor_version_,codec_type_);
    }
  private:
    int16_t product_id_;
    int8_t major_version_;
    int8_t minor_version_;
    CodecType codec_type_;
  };




}

#endif
