#include "product_manager.h"

using namespace wmdf;
using namespace std;

wmdf::ProductManager* wmdf::ProductManager::instance_ = new wmdf::ProductManager();

ProductManager::ProductManager()
{
}

ProductManager::~ProductManager()
{
  for(uint32_t i=0;i<products_.size();i++)
    delete products_[i];
  products_.clear();
}

bool ProductManager::SaveState(Memento* m)
{
  m->SetName("ProductManager");
  for(uint32_t i=0;i<products_.size();i++)
  {
    Memento* pd_m=m->CreateChild("ProductDescripter");
    products_[i]->SaveState(pd_m);
  }
  return true;
}


bool ProductManager::LoadState(Memento* m)
{
  products_.clear();
  int32_t child_num=m->GetChildCount();
  for(int32_t i=0;i<child_num;i++)
  {
    Memento* child_m=m->GetChild(i);
    ProductDescripter* pd=new ProductDescripter();
    pd->LoadState(child_m);
    products_.push_back(pd);
  }
  return true;
}

ProductDescripter* ProductManager::FindProductDescripter(int16_t product_id)
{
  for(int32_t i = 0; i < products_count(); ++i)
  {
    if(products(i)->product_id() == product_id)
      return products(i);
  }
  return NULL;
}

ProductDescripter* ProductManager::FindProductDescripter(std::string product_name)
{
  for(int32_t i = 0; i < products_count(); ++i)
  {
    if(products(i)->product_name() == product_name)
      return products(i);
  }
  return NULL;
}

void ProductManager::delete_products(int16_t product_id)
{
  std::vector<ProductDescripter*>::iterator it;
  for(it = products_.begin(); it != products_.end(); ++it)
  {
    if((*it)->product_id() == product_id)
    {
      delete (*it);
      it = products_.erase(it);
      return;
    }
  }
}

void ProductManager::delete_products(ProductDescripter* ptr_product)
{
  std::vector<ProductDescripter*>::iterator it;
  for(it = products_.begin(); it != products_.end(); ++it)
  {
    if( *it == ptr_product )
    {
      delete (*it);
      it = products_.erase(it);
      return;
    }
  }
}

ProductManager* wmdf::ProductManager::instance()
{
  return instance_;
}
