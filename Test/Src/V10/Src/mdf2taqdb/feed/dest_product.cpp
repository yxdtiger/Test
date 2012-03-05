#include "dest_product.h"

wmdf::DestProduct::DestProduct(void)
{
}

wmdf::DestProduct::~DestProduct(void)
{
}

bool wmdf::DestProduct::SaveState( Memento* m )
{
  m->SetName("DestProduct");
  m->SetInteger("ProductId",product_id_);
  return true;
}

bool wmdf::DestProduct::LoadState( Memento* m )
{
  product_id_=m->GetInteger("ProductId");
  return true;
}
