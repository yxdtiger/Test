#ifndef SERVER_INFO_H
#define SERVER_INFO_H

#include <string>
#include <vector>
#include <map>
#include "../common/studio_utility.h"
#include "../../include/wmdf_api.h"
#include "../../include/wmdf_structs.h"
#include "persistable.h"
#include "memento.h"
#include <boost/thread/mutex.hpp>

namespace wmdf
{
  class ServerNetInfo:public Persistable
  {
  public:
    ServerNetInfo(void);
    virtual ~ServerNetInfo(void);

    virtual bool LoadState(Memento* m);
    virtual bool SaveState(Memento* m);   

/*---------------------------------------------------------------------------*/
    int32_t server_id() const {return server_id_;}
    void set_server_id(int32_t server_id){server_id_=server_id;}

    std::string ip() const {return ip_;}
    void set_ip(std::string p_ip){ip_=p_ip;}

    int32_t monitor_port() const {return monitor_port_;}
    void set_monitor_port(int32_t port){monitor_port_=port;}     

    int32_t server_type() const {return server_type_;}
    void set_server_type(int32_t type){server_type_=type;}

    int32_t quote_port() const {return quote_port_;}
    void set_quote_port(int32_t port){quote_port_=port;}

    std::string monitor_user_name()const{return monitor_user_name_;}
    void set_monitor_user_name(std::string name){monitor_user_name_=name;}

    std::string monitor_keyword()const{return monitor_keyword_;}
    void set_monitor_keyword(std::string key){monitor_keyword_=key;}

    std::string quote_user_name()const{return quote_user_name_;}
    void set_quote_user_name(std::string name){quote_user_name_=name;}

    std::string quote_keyword()const{return quote_keyword_;}
    void set_quote_keyword(std::string key){quote_keyword_=key;}

/*---------------------------------------------------------------------------*/
    int32_t status() const {return status_;}
    void set_status(int32_t status){status_=status;} 

    int32_t index_of_event() const {return index_of_event_;}
    void set_index_of_event(int32_t index){index_of_event_=index;} 

    int32_t is_drawed() const {return is_drawed_;}
    void set_is_drawed(int32_t drawed_flag){is_drawed_=drawed_flag;} 

/*---------------------------------------------------------------------------*/
    ClientStatus* client_status(int index){return client_statuses_[index];}    
    int client_status_count(){return client_statuses_.size();}  
    ClientStatus* FindClientStatus(int32_t socket_id);
    void AddClientStatus(ClientStatus* status);    
    void ClearClientsStatus();   
    void LockClientsStatus();
    void UnlockClientsStatus();

    TransferDataStat* transfer_status(int index){return transfer_statuses_[index];}    
    int transfer_status_count(){return transfer_statuses_.size();}   
    TransferDataStat* FindTransferStatus(int16_t product_id);
    void AddTransferStatus(TransferDataStat* status);    
    void ClearTransfersStatus();
    void LockTransfersStatus();
    void UnlockTransfersStatus();
    
    MonitorEvent* monitor_events(int index){return monitor_events_[index];}    
    int monitor_events_count(){return monitor_events_.size();}    
    void AddMonitorEvent(MonitorEvent* monitor_event);   
    void ClearMonitorEvent();
    void LockMonitorEvent();
    void UnlockMonitorEvent();

    void ClearModels();
    bool NeedAlarm();
    void RestAllModels();
    void ClearWarningStatus();

    SymbolModel* GetSymbolModel(std::string key,StudioSymbolType type);
    

    ServerNetInfo* Copy();
  private://用户配置参数 
    std::string ip_;
    int32_t monitor_port_;  
    int32_t server_type_;  
    int32_t quote_port_; 
    std::string monitor_user_name_;
    std::string monitor_keyword_;
    std::string quote_user_name_;
    std::string quote_keyword_;

  private://程序衍生参数
    int32_t server_id_;  
    int32_t status_; 
    int32_t is_drawed_;

    std::vector<ClientStatus*> client_statuses_;
    std::vector<TransferDataStat*> transfer_statuses_;
    std::vector<MonitorEvent*> monitor_events_;

    std::map<std::string,SymbolModel*> symbol_models_;

    int index_of_event_;
    boost::mutex client_mutex_;
    boost::mutex transfer_mutex_;
    boost::mutex event_mutex_;
  };
}

#endif
