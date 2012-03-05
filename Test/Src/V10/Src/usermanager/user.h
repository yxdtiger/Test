#ifndef USER_H_
#define USER_H_

#include <string>
#include <vector>
#include "persistable.h"


#include "../include/wmdf_api.h"


namespace wmdf{

class AuthorizedProduct;
class Memento;

class User : public Persistable
{
public:
	User();
	virtual ~User();
  void Init();
	virtual bool LoadState(Memento* m);
	virtual bool SaveState(Memento* m);
	int32_t user_id() const {return user_id_;}
	void set_user_id(int32_t user_id) {user_id_ = user_id;}
	std::string user_name() const {return user_name_;}
	void set_user_name(std::string user_name){user_name_=user_name;}
	std::string passwd() const {return passwd_;}
	void set_passwd(std::string passwd){passwd_=passwd;}
	std::string ip_address() const {return ip_address_;}
	void set_ip_address(std::string ip_address){ip_address_=ip_address;}
  bool is_ip_limited() const {return is_ip_limited_;}
  void set_is_ip_limited(bool is_ip_limited){is_ip_limited_=is_ip_limited;}
	std::string company() const {return company_;}
	void set_company(std::string company){company_=company;}
	std::string contacts() const {return contacts_;}
	void set_contacts(std::string contacts){contacts_=contacts;}
	std::string phone_number() const {return phone_number_;}
	void set_phone_number(std::string phone_number){phone_number_=phone_number;}
	std::string email_address() const {return email_address_;}
	void set_email_address(std::string email_address){email_address_=email_address;}
	bool is_privileged() const {return is_privileged_;}
	void set_is_privileged(bool is_privileged){is_privileged_=is_privileged;}
  void set_authorization_deadline(date_t date){authorization_deadline_ = date;}
  date_t authorization_deadline() const {return authorization_deadline_;}
	AuthorizedProduct* authorized_products(int32_t index) const {return authorized_products_[index];}
  AuthorizedProduct* FindAuthorizedProduct( int32_t product_id);
  int32_t authorized_products_count() const;
	void add_authorized_products(AuthorizedProduct* );

public:
  User& operator= (const User& rhs);

private:
    int32_t user_id_;
    std::string user_name_;
    std::string passwd_;
    bool is_ip_limited_;
    std::string ip_address_;
    std::string company_;
    std::string contacts_;
    std::string phone_number_;
    std::string email_address_;
    bool is_privileged_;
    date_t authorization_deadline_;
    std::vector<AuthorizedProduct*> authorized_products_;
};

};

#endif
