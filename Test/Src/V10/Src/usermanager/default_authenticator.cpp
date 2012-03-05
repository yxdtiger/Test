#include "default_authenticator.h"
#include "user.h"
#include "authorized_product.h"
#include "user_manager.h"
#include "../include/date_time.h"
#include "../include/wmdf_structs.h"
#include "../environment/server_environment.h"

wmdf::DefaultAuthenticator::DefaultAuthenticator(void)
{
}

wmdf::DefaultAuthenticator::~DefaultAuthenticator(void)
{
}

eReturnValue wmdf::DefaultAuthenticator::Check(SharedMdfMessage message,const std::string& ipaddress)
{
  LoginRequest* request = (LoginRequest*)message->body();
	std::string user_name((char*)request->user_name);
  User* user = ServerEnvironment::user_manager()->FindUser(user_name);
	if(NULL != user)
	{
		if(strcmp((char*)request->password,user->passwd().c_str())==0)
		{
      if(DateTime::date2ymd(DateTime::today())>user->authorization_deadline())
      {
        return IDEXPIRED;
      }
      //默认admin用户不用检查Ip地址
      if (!user->is_ip_limited())
      {
        return AUTHORITY_VALID;
      }
      else if(strcmp(ipaddress.c_str(),user->ip_address().c_str())==0)
			{
				return AUTHORITY_VALID;
			}
			else return IP_UNAUTHORIZED;
		}
		else return PASSWORD_INCORRECT;
	}
	else return USER_NOT_EXIST;
}

eReturnValue wmdf::DefaultAuthenticator::Check( int32_t user_id,int16_t product_id,int16_t message_id )
{
  User* user = ServerEnvironment::user_manager()->FindUser(user_id);
  AuthorizedProduct* product = user->FindAuthorizedProduct(product_id);
  if(product!=NULL&&product->contains(message_id))
    return SUCCESS;
  else
    return AUTHORITY_INVALID;
}

