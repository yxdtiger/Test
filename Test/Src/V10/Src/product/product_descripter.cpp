#include "product_descripter.h"

using namespace wmdf;
using namespace std;

ProductDescripter::ProductDescripter()
{

}


ProductDescripter::~ProductDescripter()
{
  for(uint32_t i=0;i<support_versions_.size();i++)
    delete support_versions_[i];
  support_versions_.clear();
}


bool ProductDescripter::SaveState(Memento* m)
{

  m->SetName("ProductDescripter");
  m->SetInteger("ProductID",product_id_);
  m->SetString("ProductName",product_name_);
  m->SetString("Supplier",supplier_);
  m->SetString("Description",description_);

  for(uint32_t i=0;i<support_versions_.size();i++)
  {
    Memento* sv_m=m->CreateChild("SupportVersions");
    support_versions_[i]->SaveState(sv_m);
  }
  return true;
}



bool ProductDescripter::LoadState(Memento* m)
{
  product_id_=m->GetInteger("ProductID");
  product_name_=m->GetString("ProductName");
  supplier_=m->GetString("Supplier");
  description_=m->GetString("Description");

  int32_t num_of_versions=m->GetChildCount();
  for(int32_t i=0;i<num_of_versions;i++)
  {
    Memento* sub_m=m->GetChild(i);
    VersionDescripter* vd=new VersionDescripter();
    vd->LoadState(sub_m);
    support_versions_.push_back(vd);
  }

  return true;
}

// deep copy
ProductDescripter& ProductDescripter::operator= (const ProductDescripter& rhs)
{
  product_id_ = rhs.product_id();
  product_name_ = rhs.product_name();
  supplier_ = rhs.supplier();
  description_ = rhs.description();

  for(int32_t i = 0; i < rhs.support_versions_count(); ++i)
  {
    VersionDescripter* ptr_version_descripter = new VersionDescripter;
    *ptr_version_descripter = *rhs.support_versions(i);
  }

  return *this;
}

VersionDescripter* ProductDescripter::FindVersionDescripter(int8_t major_version, int8_t minor_version)
{
  for(int32_t i = 0; i < support_versions_count(); ++i)
  {
    if( major_version == support_versions(i)->major_version()
      && minor_version == support_versions(i)->minor_version() )
    {
      return support_versions(i);
    }
  }

  return NULL;
}

void ProductDescripter::delete_version(int8_t major_version, int8_t minor_version)
{
  vector<VersionDescripter*>::iterator it;
  for(it = support_versions_.begin(); it != support_versions_.end(); ++it)
  {
    if((*it)->major_version() == major_version && (*it)->minor_version() == minor_version)
    {
      delete (*it);
      support_versions_.erase(it);
      break;
    }
  }
}

void ProductDescripter::delete_version(VersionDescripter* ptr_version)
{
  vector<VersionDescripter*>::iterator it;
  for(it = support_versions_.begin(); it != support_versions_.end(); ++it)
  {
    if( *it == ptr_version )
    {
      delete (*it);
      support_versions_.erase(it);
      break;
    }
  }
}
