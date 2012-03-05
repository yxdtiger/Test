#ifndef MESSAGE_DESCRIPTER_H
#define MESSAGE_DESCRIPTER_H

#include <string>
#include "persistable.h"
#include "memento.h"
#include "../include/wmdf_api.h"

namespace wmdf
{

class MessageDescripter : public Persistable
{
public:
  MessageDescripter(){}
  virtual ~MessageDescripter(){}
  virtual bool SaveState(Memento* m);
  virtual bool LoadState(Memento* m);
  int16_t message_id() const {return message_id_;};
  void set_message_id(int16_t message_id){message_id_=message_id;}
  string message_description() const {return message_description_;}
  void set_message_description(std::string message_description){message_description_=message_description;}

public:
  MessageDescripter& operator= (const MessageDescripter& rhs);

private:
  int16_t message_id_;
  std::string message_description_;

};


}

#endif
