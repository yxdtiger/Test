#ifndef FEED_CONNECTION_H
#define FEED_CONNECTION_H

#include <vector>
#include <map>
#include <string>
#include <boost/thread/mutex.hpp>
#include <boost/thread.hpp>
#include "pipe.h"
#include "net_module.h"
#include "feed_client.h"
#include "../include/wmdf_api.h"
#include "../include/wmdf_internal_api.h"
#include "../product/product_source.h"

namespace wmdf
{
  class ProtocolProcessorController;
  class LoginProxy;
  class FeedConnectionPool;
  class ConfigRequestProxy;
  class ConfigModifyProxy;

  class FeedConnection:public IClientCallback
  {
  public:
    FeedConnection(int32_t connection_id);
    virtual ~FeedConnection(void);
    bool Connect(ProductSource* ps);
    bool Connect();  //正常连接断开后，进行重连
    void DisConnect();
    bool Login();
    ConfigInfo* RequestConfigInfo(ConfigFileType type);
    eReturnValue RequestModifyConfig(ConfigInfo* config_info);

    // 设置了回调函数则不需要调用此方法
    MdfRecord* RecvRecord();    
    bool is_connected();
    int32_t connection_id() {return connection_id_;}
    //当前连接的服务器信息
    std::string connected_server_address(){return feed_client_->server_address();}
    int32_t connected_server_port(){return feed_client_->server_port();}
    std::string connected_user_name(){return product_source_->user_name();}
    std::string connected_pass_word(){return product_source_->pass_word();}


    time_stamp_t last_heart_beat_time() {return last_heart_beat_time_;}
    void set_last_heart_beat_time(time_stamp_t time){last_heart_beat_time_ = time;}
    void set_record_callback(RecordCallback record_callback);
    void set_event_callback(FeedEventCallback event_callback);
    void set_filter_masks(const std::string& filter_masks);
    void set_state(void* state);
    void set_market_time(int32_t begin_time,int32_t end_time);
    bool is_market_time(int32_t curr_time);
    int64_t pre_size(){return pre_size_;}
    void set_pre_size(int64_t size){pre_size_=size;}
    int64_t cur_size(){return cur_size_;}
    
    void set_owner(FeedConnectionPool* owner);
    virtual void OnRecvMsg(Message* msg);

    virtual void OnCloseCallback(EventMsg* args);
    virtual void OnEventCallback(EventMsg* args);
    virtual void ResetContext();
  private:
    void ProcessLoginResponse(Message* msg);
    void ProcessConfigResponse(Message* msg);
    void ProcessUpdateConfigResponse(Message* msg);
    void DoFilter( MdfRecord*& record );
    void DoCallback( MdfRecord* record );
    template< typename H,typename B >
    MdfRecord* Filter(MdfRecord* src)
    {
      MdfRecord* dst = new MdfRecord();
      memcpy(dst,src,sizeof(MdfRecord));
      dst->header = new H();
      memcpy(dst,src,sizeof(H));
      std::vector<int32_t> indexes;
      B* body_src = (B*)src->body;
      for (int32_t index=0;index < src->record_count;index++)
      {
        if (IsSecIdInFilter((body_src+index)->wind_code))
        {
          indexes.push_back(index);
        }
      }
      dst->record_count = (int32_t)indexes.size();
      if (dst->record_count > 0)
      {
        dst->body = new B[indexes.size()];
        for (uint32_t index=0;index<indexes.size();index++)
        {
          B* body_dst = (B*)dst->body;
          memcpy(body_dst+index,body_src+indexes[index],sizeof(B));
        }
      }
      else
      {
        dst->body = NULL;
      }
      return dst;
    }
    MdfRecord* FilterOrderQueue(MdfRecord* src);
    bool IsSecIdInFilter(const std::string& sec_id);
    MdfRecord* FilterRecord(MdfRecord* src);

  private:
    int32_t connection_id_;    //连接ID
    int32_t market_begin_time_; //090000
    int32_t market_end_time_;   //160000;
    FeedClient* feed_client_;  // 
    LoginProxy* login_proxy_;
    ConfigRequestProxy* config_request_proxy_;
    ConfigModifyProxy* config_modify_proxy_;
    ConfigInfo* config_info_;
    UpdateConfigResponse* update_config_response_;

    RecordCallback record_callback_;  //数据回调函数
    FeedEventCallback event_callback_; //事件回调函数
    void* state_;                     //回调时传回句柄       
    ypipe_t<MdfRecord*,256> record_queue_;  //record缓存区，当没有设置回调函数时有用
    ProtocolProcessorController* protocol_processor_controller_;   //协议解码控制器
    //字符串，索引
    std::map<std::string,int32_t> secid_filter_masks_;    //过滤条件，只有符合过滤条件的才被返回。
    ProductSource* product_source_;
    FeedConnectionPool* owner_;
    time_stamp_t last_heart_beat_time_;
    

    volatile int64_t pre_size_;
    volatile int64_t cur_size_;

  };

  class FeedConnectionPool
  {
  public:
    static FeedConnectionPool* instance();
    void Startup();
    void Shutdown();
    bool BorrowConnection(int32_t connection_id);
    void CommitBorrow(int32_t connection_id);
    void ReturnConnection(int32_t connection_id); 
    void CommitReturn(int32_t connection_id);
    bool IsAllInUsing();
    int32_t feed_connections_count(){return (int32_t)feed_connection_pool_.size();}
    FeedConnection* feed_connections(int32_t index){return feed_connection_pool_[index];}
    static const int32_t max_num_of_connections = 32;
  private:
    void Guard();
    void GuardConnections();
  private:
    FeedConnectionPool(int32_t max_size);
    ~FeedConnectionPool();
    std::vector<FeedConnection*> feed_connection_pool_;
    std::vector<bool> connection_states_;
    std::vector<FeedConnection*> active_connections_;
    static FeedConnectionPool* instance_;
    boost::thread* work_thread_;
    volatile bool isworking_;
    boost::mutex mutex_;
  };
}

#endif
