#ifndef WARNING_FACTORY_H
#define WARNING_FACTORY_H

#include <string>
#include "../../include/wmdf_api.h"
#include "../common/studio_utility.h"

namespace wmdf
{
  class ModelIDFactory
  {
  public:
    ModelIDFactory(void);
    virtual ~ModelIDFactory(void);

    static std::string GetServer2SourcePort(int32_t conn_id ,const char* source_ip,int32_t source_port);

    static std::string GetClientTitle(int32_t conn_id ,const char* client_ip,int32_t socket_id);
    //ServerID-ClientSocketID
    static std::string GetServer2ClientSocket(int32_t conn_id ,int32_t socket_id );

  };
}

#endif
