#ifndef VERSION_DESCRIPTER_H
#define VERSION_DESCRIPTER_H

#include <string>
#include <vector>
#include "persistable.h"
#include "memento.h"
#include "message_descripter.h"
#include "../include/wmdf_api.h"

namespace wmdf
{
  class VersionDescripter : public Persistable
  {
  public:
    VersionDescripter();
    virtual ~VersionDescripter();
    virtual bool SaveState(Memento* m);
    virtual bool LoadState(Memento* m);
    int8_t major_version() const {return major_version_;}
    void set_major_version(int8_t major_version){major_version_=major_version;}
    int8_t minor_version() const {return minor_version_;}
    void set_minor_version(int8_t minor_version){minor_version_=minor_version;}
    CodecType support_codecs(int32_t index) const {return support_codecs_[index];}
    void add_support_codecs(CodecType suppport_codecs){support_codecs_.push_back(suppport_codecs);}
    int32_t support_codecs_count() const {return (int32_t)support_codecs_.size();}
    MessageDescripter* support_message(int32_t index) const{return support_messages_[index];}
    void add_support_message(MessageDescripter* support_message){support_messages_.push_back(support_message);}
    int32_t support_messages_count() const {return (int32_t)support_messages_.size();}
    MessageDescripter* FindMessageDescripter(int16_t message_id);
    MessageDescripter* FindMessageDescripter(std::string message_description);
    void clear_codecs();
    void delete_message(int16_t message_id);
    void delete_message(MessageDescripter* ptr_message);

  public:
    VersionDescripter& operator= (const VersionDescripter& rhs);

  private:
    int8_t major_version_;
    int8_t minor_version_;
    std::vector<CodecType> support_codecs_;
    std::vector<MessageDescripter*> support_messages_;
  };



}

#endif
