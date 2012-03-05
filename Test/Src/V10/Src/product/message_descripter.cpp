#include "message_descripter.h"

using namespace std;
using namespace wmdf;


bool MessageDescripter::SaveState(Memento* m)
{
  //m->SetName("MessageDescription");
  m->SetInteger("message_id",message_id_);
  m->SetString("message_description",message_description_);
  return 0;
}


bool MessageDescripter::LoadState(Memento* m)
{
  message_id_=m->GetInteger("message_id");
  message_description_=m->GetString("message_description");
  return 0;
}

MessageDescripter& MessageDescripter::operator= (const MessageDescripter& rhs)
{
  message_id_ = rhs.message_id();
  message_description_ = rhs.message_description();
  return *this;
}
