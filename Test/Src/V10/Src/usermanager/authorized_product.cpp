#include "authorized_product.h"

using namespace std;
using namespace wmdf;

AuthorizedProduct::AuthorizedProduct():product_id_(0)
{

}

AuthorizedProduct::~AuthorizedProduct()
{
  messages_.clear();

}



bool AuthorizedProduct::SaveState(Memento* m)
{
	m->SetName("AuthorizedProduct");
	m->SetInteger("product_id",product_id_);

	for(uint32_t i=0;i<messages_.size();i++)
	{
		Memento* sub_m=m->CreateChild("Message");
		sub_m->SetInteger("message_id",messages_[i]);
	}
	return true;
}

bool AuthorizedProduct::LoadState(Memento* m)
{
	product_id_=m->GetInteger("product_id");
	int32_t n=m->GetChildCount();
	for(int32_t i=0;i<n;i++)
	{
		Memento* sub_m=m->GetChild(i);
		messages_.push_back(sub_m->GetInteger("message_id"));
	}

	return true;
}

AuthorizedProduct& AuthorizedProduct::operator= (const AuthorizedProduct& rhs)
{
  product_id_ = rhs.product_id();

  for(int32_t i = 0; i < rhs.message_count(); ++i)
  {
    add_message_id(rhs.message_id(static_cast<int16_t>(i)));
  }

  return *this;
}

bool wmdf::AuthorizedProduct::contains( int16_t id )
{
  for(int32_t i = 0; i < messages_.size(); ++i)
  {
    if(messages_[i]==id)
    {
      return true;
    }
  }
  return false;
}
