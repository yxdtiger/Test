#ifndef AUTHENTICATOR_H
#define AUTHENTICATOR_H
#include "../include/wmdf_api.h"
#include "../protocol/mdf_message.h"
#include <string>

namespace wmdf
{
  class Authenticator
  {
  public:
    Authenticator(void);
    virtual ~Authenticator(void);
    virtual eReturnValue Check(SharedMdfMessage message, const std::string& ipaddress) = 0;
    virtual eReturnValue Check(int32_t user_id,int16_t product_id,int16_t message) = 0;
  };
}

#endif
