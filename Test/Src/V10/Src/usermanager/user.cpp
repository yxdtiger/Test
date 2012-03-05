#include "user.h"
#include "memento.h"
#include "authorized_product.h"
#include "../include/date_time.h"

using namespace std;
using namespace wmdf;


User::User()
{
  Init();
}
User::~User()
{
  Init();
}

void User::Init()
{
  user_id_ = 0;
  user_name_.clear();
  passwd_.clear();
  ip_address_.clear();
  company_.clear();
  contacts_.clear();
  phone_number_.clear();
  email_address_.clear();
  is_privileged_ = false;
  is_ip_limited_ = true;
  authorization_deadline_ = 0;

  for(int32_t i = 0; i < static_cast<int32_t>(authorized_products_.size()); ++i)
  {
    delete authorized_products_[i];
    authorized_products_[i] = NULL;
  }
  authorized_products_.clear();
}

int32_t User::authorized_products_count() const
{
  return authorized_products_.size();
}

void User::add_authorized_products(AuthorizedProduct* authorized_product)
{
  authorized_products_.push_back(authorized_product);
} 

bool User::SaveState(Memento* m)
{
	m->SetName("User");
	m->SetInteger("user_id", user_id_);
	m->SetString("user_name", user_name_);
	m->SetString("passwd", passwd_);
	m->SetString("ip_address", ip_address_);
	m->SetString("company", company_);
	m->SetString("contacts", contacts_);
	m->SetString("phone_number", phone_number_);
	m->SetString("email_address", email_address_);
	m->SetBoolean("is_privileged",is_privileged_);
  m->SetBoolean("is_ip_limited",is_ip_limited_);
  m->SetInteger("authorization_deadline", authorization_deadline_);

	for(uint32_t i=0;i<authorized_products_.size();i++)
	{
		Memento* sub_m=m->CreateChild("AuthorizedProduct");
    authorized_products_[i]->SaveState(sub_m);
	}
	return true;
}
  
bool User::LoadState(Memento* m)
{
  user_id_=m->GetInteger("user_id");
  user_name_=m->GetString("user_name");
  passwd_=m->GetString("passwd");
  ip_address_=m->GetString("ip_address");
  company_=m->GetString("company");
  contacts_=m->GetString("contacts");
  phone_number_=m->GetString("phone_number");
  email_address_=m->GetString("email_address");
  is_privileged_=m->GetBoolean("is_privileged");
  is_ip_limited_=m->GetBoolean("is_ip_limited",true);
  authorization_deadline_ = m->GetInteger("authorization_deadline");

  int32_t child_count=m->GetChildCount();
  for(int32_t i=0;i<child_count;i++)
  {
    Memento* sub_m=m->GetChild(i);
    AuthorizedProduct* pAuthorizedProduct=new AuthorizedProduct();
    pAuthorizedProduct->LoadState(sub_m);
    add_authorized_products(pAuthorizedProduct);
  }
  return true;
}

AuthorizedProduct*  User::FindAuthorizedProduct( int32_t product_id)
{
   for(uint32_t i=0;i<authorized_products_.size();i++)
   {
      if(authorized_products_[i]->product_id()==product_id)
      {
        return authorized_products_[i];
      }
   }
   return NULL;
}

User& User::operator= (const User& rhs)
{
  user_id_ = rhs.user_id();
  user_name_ = rhs.user_name();
  passwd_ = rhs.passwd();
  ip_address_ = rhs.ip_address();
  company_ = rhs.company();
  contacts_ = rhs.contacts();
  phone_number_ = rhs.phone_number();
  email_address_ = rhs.email_address();
  is_privileged_ = rhs.is_privileged();
  is_ip_limited_ = rhs.is_ip_limited();
  authorization_deadline_ = rhs.authorization_deadline();
  
  for(int32_t i = 0; i < rhs.authorized_products_count(); ++i)
  {
    AuthorizedProduct* ptr_authorized_product = new AuthorizedProduct;
    *ptr_authorized_product = *rhs.authorized_products(static_cast<int32_t>(i));  // Éî¿½±´
    add_authorized_products(ptr_authorized_product);
  }

  return *this;
}