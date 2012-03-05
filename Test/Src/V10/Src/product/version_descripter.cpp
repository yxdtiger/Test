#include "version_descripter.h"
#include "../environment/strutils.h"

using namespace wmdf;
using namespace std;

VersionDescripter::VersionDescripter()
{

}
VersionDescripter::~VersionDescripter()
{
  for(uint32_t i=0;i<support_messages_.size();i++)
    delete support_messages_[i];
  support_messages_.clear();
}



bool VersionDescripter::SaveState(Memento* m)
{
  m->SetName("VersionDescription");
  m->SetInteger("major",major_version_);
  m->SetInteger("minor",minor_version_);
  for(uint32_t i=0;i<support_codecs_.size();i++)
  {
    string str = StrUtils::formatstring(30,"SupportCodec%d",i);
    m->SetInteger(str,support_codecs_[i]);
  }

  for(uint32_t i=0;i<support_messages_.size();i++)
  {
    Memento* mds_m=m->CreateChild("support_messages");
    support_messages_[i]->SaveState(mds_m);
  }
  return true;
}


bool VersionDescripter::LoadState(Memento* m)
{
  major_version_=m->GetInteger("major");
  minor_version_=m->GetInteger("minor");
  int32_t attribute_num=m->GetAttributeCount();
  for(int32_t i=0;i<attribute_num-2;i++)
  {
    string str = StrUtils::formatstring(30,"SupportCodec%d",i);
    CodecType ct=static_cast<CodecType>(m->GetInteger(str));
    support_codecs_.push_back(ct);
  }

  int32_t childcount=m->GetChildCount();
  for (int32_t i=0;i<childcount;i++)
  {
    Memento* child_m=m->GetChild(i);
    MessageDescripter* md=new MessageDescripter();
    md->LoadState(child_m);
    support_messages_.push_back(md);
  }

  return true;
}

VersionDescripter& VersionDescripter::operator= (const VersionDescripter& rhs)
{
  major_version_ = rhs.major_version();
  minor_version_ = rhs.minor_version();

  int32_t i = 0;
  for(i = 0; i < rhs.support_codecs_count(); ++i)
  {
    support_codecs_.push_back(rhs.support_codecs(i));
  }

  for(i = 0; i < rhs.support_messages_count(); ++i)
  {
    MessageDescripter* ptr_message_descripter = new MessageDescripter;
    *ptr_message_descripter = *(rhs.support_message(i));
    support_messages_.push_back(ptr_message_descripter);
  }

  return *this;
}

MessageDescripter* VersionDescripter::FindMessageDescripter(int16_t message_id)
{
  for(int32_t i = 0; i < support_messages_count(); ++i)
  {
    if(message_id == support_message(i)->message_id())
      return support_message(i);
  }

  return NULL;
}

MessageDescripter* VersionDescripter::FindMessageDescripter(std::string message_description)
{
  for(int32_t i = 0; i < support_messages_count(); ++i)
  {
    if(message_description == support_message(i)->message_description())
      return support_message(i);
  }

  return NULL;
}

void VersionDescripter::clear_codecs()
{
  support_codecs_.clear();
}

void VersionDescripter::delete_message(int16_t message_id)
{
  vector<MessageDescripter*>::iterator it;
  for(it = support_messages_.begin(); it != support_messages_.end(); ++it)
  {
    if((*it)->message_id() == message_id)
    {
      delete (*it);
      support_messages_.erase(it);
      break;
    }
  }
}

void VersionDescripter::delete_message(MessageDescripter* ptr_message)
{
  vector<MessageDescripter*>::iterator it;
  for(it = support_messages_.begin(); it != support_messages_.end(); ++it)
  {
    if( *it == ptr_message )
    {
      delete (*it);
      support_messages_.erase(it);
      break;
    }
  }
}
