#include "product_source_manager.h"

wmdf::ProductSourceManager::ProductSourceManager(void)
{
}

wmdf::ProductSourceManager::~ProductSourceManager(void)
{
  for(uint32_t i=0;i<product_sources_.size();i++)
    delete product_sources_[i];
  product_sources_.clear();
}

bool wmdf::ProductSourceManager::LoadState( Memento* m )
{
  int child_num=m->GetChildCount();
  for(int i=0;i<child_num;i++)
  {
    Memento* sub_m=m->GetChild(i);
    ProductSource* ps=new ProductSource();
    ps->LoadState(sub_m);
    product_sources_.push_back(ps);
  }
  return true;
}

bool wmdf::ProductSourceManager::SaveState( Memento* m )
{
  m->SetName("ProductSourceManager");
  for(uint32_t i=0;i<product_sources_.size();i++)
  {
    Memento* sub_m=m->CreateChild("ProductSource");
    product_sources_[i]->SaveState(sub_m);
  }
  return true;
}

void wmdf::ProductSourceManager::delete_product_source(int32_t product_source_id)
{
  std::vector<ProductSource*>::iterator it;
  for(it = product_sources_.begin(); it != product_sources_.end(); it++)
  {
    if(product_source_id == (*it)->product_id())
    {
      delete (*it);
      product_sources_.erase(it);
      break;
    }
  }
}

void wmdf::ProductSourceManager::delete_product_source(wmdf::ProductSource* ptr_product_source)
{
  std::vector<ProductSource*>::iterator it;
  for(it = product_sources_.begin(); it != product_sources_.end(); it++)
  {
    if( *it == ptr_product_source )
    {
      delete (*it);
      product_sources_.erase(it);
      break;
    }
  }
}

wmdf::ProductSource* wmdf::ProductSourceManager::FindProductSource(int32_t product_source_id)
{
  wmdf::ProductSource* ptr_product_source = NULL;
  for(int i = 0; i < product_source_count(); ++i)
  {
    if(product_source_id == product_sources(i)->product_id())
    {
      ptr_product_source = product_sources(i);
      break;
    }
  }

  return ptr_product_source;
}