#ifndef SERVER_MANAGER_H
#define SERVER_MANAGER_H

#include <map>
#include "net_info_collection.h"
#include "../common/studio_utility.h"

namespace wmdf
{
  class ServerManager
  {
  public:
    static ServerManager* Instance();
    void Initialize();
    void Finalize ();
    void add_server_info(ServerNetInfo* conn_info);
    bool delete_server_by_cid(int32_t conn_id);
    NetInfoCollection* net_info_collection(){return net_info_collection_;}

    int32_t get_next_id();    
    bool connection_id(int32_t id){return connection_ids_[id];}

    
  private:
    ServerManager(void);
    virtual ~ServerManager(void);

    void set_connection_id(int32_t id,bool flag){connection_ids_[id]=flag;}
  private:
    NetInfoCollection* net_info_collection_ ;

    std::vector<bool> connection_ids_; 
    static const int32_t max_num_of_connections = 50;
    static ServerManager* instance_;  
  };
}

#endif
