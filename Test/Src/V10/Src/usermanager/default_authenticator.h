#ifndef DEFAULT_AUTHENTICATOR_H
#define DEFAULT_AUTHENTICATOR_H
#include "authenticator.h"
#include <string>

namespace wmdf
{
  class DefaultAuthenticator:public Authenticator
  {
  public:
    DefaultAuthenticator(void);
    virtual ~DefaultAuthenticator(void);
    virtual eReturnValue Check(SharedMdfMessage message,const std::string& ipaddress);
    virtual eReturnValue Check(int32_t user_id,int16_t product_id,int16_t message);
  };
}

#endif
