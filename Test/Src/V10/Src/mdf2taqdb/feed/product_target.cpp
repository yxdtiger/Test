#include "product_target.h"

wmdf::ProductTarget::ProductTarget(void)
{
}

wmdf::ProductTarget::~ProductTarget(void)
{
}

bool wmdf::ProductTarget::SaveState( Memento* m )
{
  m->SetName("ProductTarget");
  m->SetString("IPAddr",dest_ip_);
  m->SetInteger("Port",dest_port_);
  for(uint32_t i=0;i<products_.size();i++)
  {
    Memento* sub_m=m->CreateChild("DestProduct");
    products_[i]->SaveState(sub_m);
  }
  return true;
}

bool wmdf::ProductTarget::LoadState( Memento* m )
{
  dest_ip_=m->GetString("DestIP");
  dest_port_=m->GetInteger("DestPort");

  int child_num=m->GetChildCount();
  for(int i=0;i<child_num;i++)
  {
    Memento* sub_m=m->GetChild(i);
    DestProduct* pt=new DestProduct();
    pt->LoadState(sub_m);
    products_.push_back(pt);
  }

  return true;
}
