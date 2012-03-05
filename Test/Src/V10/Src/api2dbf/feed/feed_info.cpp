#include "feed_info.h"

wmdf::FeedInfo::FeedInfo(void)
{
  codec_type_= 0;
}

wmdf::FeedInfo::~FeedInfo(void)
{
}

bool wmdf::FeedInfo::LoadState(Memento* m )
{
  source_ip_=m->GetString("SrcIP");
  source_port_=m->GetInteger("SrcPort");
  codec_type_ = m->GetInteger("Codec");
  user_name_=m->GetString("UserName");
  password_=m->GetString("Password");
  int child_num=m->GetChildCount();
  for(int i=0;i<child_num;i++)
  {
    Memento* sub_m=m->GetChild(i);
    ProductTarget* pt=new ProductTarget();
    pt->LoadState(sub_m);
    targets_.push_back(pt);
  }
  return true;
}

bool wmdf::FeedInfo::SaveState(Memento* m )
{
  m->SetName("FeedInfo");
  m->SetString("SrcIP",source_ip_);
  m->SetInteger("SrcPort",source_port_);
  m->SetInteger("Codec",codec_type_);
  m->SetString("UserName",user_name_);
  m->SetString("Password",password_);
  for(uint32_t i=0;i<targets_.size();i++)
  {
    Memento* sub_m=m->CreateChild("ProductTarget");
    targets_[i]->SaveState(sub_m);
  }
  return true;
}
