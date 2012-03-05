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
  m->SetString("DestDir",dest_dir_);
  m->SetInteger("ProductId",product_id_);
  m->SetInteger("TimeSpan",time_span_);
  m->SetInteger("StartTime",start_time_);
  m->SetInteger("EndTime",end_time_);
  
  return true;
}

bool wmdf::ProductTarget::LoadState( Memento* m )
{
  dest_dir_=m->GetString("DestDir");
  product_id_=m->GetInteger("ProductId");
  time_span_=m->GetInteger("TimeSpan");
  start_time_=m->GetInteger("StartTime");
  end_time_=m->GetInteger("EndTime");
  
  return true;
}
