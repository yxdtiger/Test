#ifndef NET_INFO_COLLECTION_H
#define NET_INFO_COLLECTION_H
//#include "../main_frame/StdAfx.h"
//#include "../main_frame/resource.h"
#include "server_info.h"
#include <vector>
//#include "NetworkMapEditorView.h"

namespace wmdf
{
  class NetInfoCollection:public Persistable
  {
  public:
    NetInfoCollection(void);
    virtual ~NetInfoCollection(void);

    virtual bool LoadState(Memento* m);
    virtual bool SaveState(Memento* m);

    void add_server_info(ServerNetInfo* conn_info);
    bool delete_server_by_cid(int32_t conn_id);
    int32_t server_info_count(){return (int32_t)servers_.size();}
    ServerNetInfo* server_infos(int32_t index){return servers_[index];}   
    ServerNetInfo* find_server_by_cid(int32_t conn_id);

    void RestAllModels();
    void ClearWarningStatus();
    bool NeedAlarm();
   /* CNetworkMapEditorView* GetView(int conn_id){return views_[conn_id];}
    void AddView(CNetworkMapEditorView* view)
    {
      views_[view->conn_id()]=view;
    }*/

    NetInfoCollection* Copy();
  private:
    /*std::map<int,CNetworkMapEditorView*> views_;*/
    std::vector<ServerNetInfo*> servers_;
  };
}

#endif
