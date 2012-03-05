#include "feed_connection.h"
#include "../environment/splitter.h"
#include "../environment/utils.h"
#include "../monitor/logger.h"
#include "../include/wmdf_structs.h"
#include "../include/date_time.h"
#include "../protocol/mdf_message.h"
#include "../protocol/protocol_processor_controller.h"
#include "../protocol/protocol_description.h"
#include "../protocol/protocol_processor_factory.h"
#include "login_proxy.h"
#include "../environment/strutils.h"
#include "config_request_proxy.h"
#include "config_modify_proxy.h"

using namespace std;

wmdf::FeedConnectionPool* wmdf::FeedConnectionPool::instance_ = new wmdf::FeedConnectionPool(wmdf::FeedConnectionPool::max_num_of_connections);

wmdf::FeedConnection::FeedConnection(int32_t connection_id)
{
  connection_id_ = connection_id;
  protocol_processor_controller_ = new ProtocolProcessorController();
  state_ = NULL;
  record_callback_ = NULL;
  event_callback_ = NULL;
  feed_client_ = new FeedClient(MDF_CLIENT);
  feed_client_->SetClientCallback(this);
  market_begin_time_ = 0;
  market_end_time_ = 0;
  login_proxy_ = new LoginProxy();
  config_request_proxy_ = new ConfigRequestProxy();
  config_modify_proxy_ = new ConfigModifyProxy();
  config_info_ = NULL;
  update_config_response_ = NULL;
  product_source_ = NULL;
  owner_ = NULL;
  last_heart_beat_time_ = 0;
  cur_size_ = 0;
  pre_size_ = 0;
}

wmdf::FeedConnection::~FeedConnection(void)
{
  if (NULL != login_proxy_)
  {
    delete login_proxy_;
    login_proxy_ = NULL;
  }
  if (NULL != config_request_proxy_)
  {
    delete config_request_proxy_;
    config_request_proxy_ = NULL;
  }
  if (NULL != config_modify_proxy_)
  {
    delete config_modify_proxy_;
    config_modify_proxy_ = NULL;
  }
  if (NULL != config_info_)
  {
    delete config_info_;
    config_info_ = NULL;
  }
  if (NULL != update_config_response_)
  {
    delete update_config_response_;
    update_config_response_ = NULL;
  }
  if (NULL != feed_client_)
  {
    delete feed_client_;
    feed_client_ = NULL;
  }
  if (NULL != protocol_processor_controller_)
  {
    delete protocol_processor_controller_;
    protocol_processor_controller_ = NULL;
  }
  state_ = NULL;
  record_callback_ = NULL;

  if (NULL != product_source_)
  {
    delete product_source_;
    product_source_ = NULL;
  }
  owner_ = NULL;
}

bool wmdf::FeedConnection::Connect(ProductSource* ps)
{
  if (NULL != product_source_)
  {
    delete product_source_;
    product_source_ = NULL;
  }
  product_source_ = ps;
  return Connect();
  
}

bool wmdf::FeedConnection::Connect()
{
  product_source_->set_which_is_in_use(0);
  last_heart_beat_time_ = 0;
  if (feed_client_->Connect(product_source_->ip_address(),product_source_->port(),
    product_source_->retry_times(),product_source_->timeout()))
  {
    return true;
  }
  else
  {
    //切换到备源
    for (int32_t index =0; index < product_source_->backup_source_count();index++)
    {
      product_source_->set_which_is_in_use(index+1);
      if (feed_client_->Connect(product_source_->backup_sources(index)->ip_address(),
        product_source_->backup_sources(index)->port(),
        product_source_->retry_times(),product_source_->timeout()))
      {
        return true;
      }
    }
  }  
  return false;
}

void wmdf::FeedConnection::DisConnect()
{
  feed_client_->DisConnect();
  if(!is_connected())
  {
    secid_filter_masks_.clear();
  }
  product_source_->set_which_is_in_use(0);
}

ConfigInfo* wmdf::FeedConnection::RequestConfigInfo(ConfigFileType type)
{
  if(is_connected())
  {
    if(config_request_proxy_->RequestConfigQequest(product_source_,feed_client_,type))
    {
      ConfigInfo* tmp_config_info = config_info_;
      config_info_ = NULL;
      return tmp_config_info;
    }
  }
  return NULL;
}


eReturnValue wmdf::FeedConnection::RequestModifyConfig(ConfigInfo* config_info)
{
  if(is_connected())
  {
    if(config_modify_proxy_->ConfigModifyQequest(product_source_,feed_client_,config_info))
    {
      UpdateConfigResponse* response = update_config_response_;
      if(update_config_response_ != NULL)
        return response->return_value;
    }
  }
  return INVALID_VALUE;
}

bool wmdf::FeedConnection::Login()
{
  if (login_proxy_->RequestLogin(product_source_,feed_client_,product_source_->in_use_user_name(),product_source_->in_use_pass_word()))
  {
    if (NULL != event_callback_)
    {
      EventArgs event_args = {0};
      event_args.state = state_;
      event_args.connection_id = this->connection_id_;
      event_args.level = LL_INFO;
      event_args.event_id = MEI_SOURCE_CONNECTED;
      event_args.date_time = DateTime::now();

      std::string at_where = "feed connection";
      uint32_t at_where_len = at_where.length() < 60 ? at_where.length() :59;
      memcpy(event_args.at_where,at_where.c_str(),at_where_len);
      std::string what_happened = StrUtils::formatstring(150,"succeed to login to server. %s, port: %d, username: %s",
        feed_client_->server_address().c_str(),
        feed_client_->server_port(),
        product_source_->in_use_user_name().c_str());
      uint32_t what_happened_len = what_happened.length()  < 150 ? what_happened.length() :149;
      memcpy(event_args.what_happened,what_happened.c_str(),what_happened_len);
      event_callback_(&event_args);
    }
    Logger::instance()->WriteFormatEventLog(LL_INFO,MEI_SOURCE_CONNECTED,"feed connection","succeed to login to server. %s, port: %d, username: %s",
      feed_client_->server_address().c_str(),
      feed_client_->server_port(),
      product_source_->in_use_user_name().c_str());
    return true;
  }
  else
  {
    if (NULL != event_callback_)
    {
      EventArgs event_args = {0};
      event_args.state = state_;
      event_args.connection_id = this->connection_id_;
      event_args.level = LL_INFO;
      event_args.event_id = MEI_SOURCE_DISCONNECTED;
      event_args.date_time = DateTime::now();

      std::string at_where = "feed connection";
      uint32_t at_where_len = at_where.length() < 60 ? at_where.length() :59;
      memcpy(event_args.at_where,at_where.c_str(),at_where_len);
      std::string what_happened = StrUtils::formatstring(150,"failed to login to server(unauthorized). server: %s, port: %d, username: %s, password: %s",
        feed_client_->server_address().c_str(),
        feed_client_->server_port(),
        product_source_->in_use_user_name().c_str(),
        product_source_->in_use_pass_word().c_str());
      uint32_t what_happened_len = what_happened.length()  < 150 ? what_happened.length() :149;
      memcpy(event_args.what_happened,what_happened.c_str(),what_happened_len);
      event_callback_(&event_args);
    }

    Logger::instance()->WriteFormatEventLog(LL_ERROR,MEI_SOURCE_DISCONNECTED,"feed connection","failed to login to server(unauthorized). server: %s, port: %d, username: %s, password: %s",
      feed_client_->server_address().c_str(),
      feed_client_->server_port(),
      product_source_->in_use_user_name().c_str(),
      product_source_->in_use_pass_word().c_str());
    return false;
  }
}


void wmdf::FeedConnection::ProcessConfigResponse( Message* msg )
{
  MdfMessage* mdf_message = config_request_proxy_->ProcessResponse(msg);
  if(mdf_message!=NULL)
  {
    if(config_info_!=NULL)
    {
      free(config_info_);
      config_info_ = NULL;
    }
    ConfigInfo* info = (ConfigInfo*)mdf_message->body();
    config_info_ = (ConfigInfo*)malloc(sizeof(ConfigInfo)+info->length);
    memcpy(config_info_,info,sizeof(ConfigInfo)+info->length);
    delete mdf_message;
  }
}

void wmdf::FeedConnection::ProcessUpdateConfigResponse( Message* msg )
{
  MdfMessage* mdf_message = config_modify_proxy_->ProcessResponse(msg);
  if(mdf_message!=NULL)
  {
    if(update_config_response_!=NULL)
    {
      delete update_config_response_;
      update_config_response_ = NULL;
    }
    UpdateConfigResponse* info = (UpdateConfigResponse*)mdf_message->body();
    update_config_response_ = new UpdateConfigResponse;
    memcpy(update_config_response_,info,sizeof(UpdateConfigResponse));
    delete mdf_message;
  }
}


void wmdf::FeedConnection::ProcessLoginResponse( Message* msg )
{
  MdfMessage* mdf_message = login_proxy_->ResponseLogin(msg);
  if (mdf_message != NULL)
  {
    LoginResponse* login_resp = (LoginResponse*)mdf_message->body();
    if ( login_resp->ack_type==AUTHORITY_VALID ) 
    {
      for (int32_t index=0; index< login_resp->product_count;index++)
      {
        if (login_resp->product_status[index].product_status == AUTHORITY_VALID)
        {
          ProtocolDescription* pd = new ProtocolDescription(
            login_resp->product_status[index].product_id,
            login_resp->product_status[index].major_version,
            login_resp->product_status[index].minor_version,
            (CodecType)login_resp->product_status[index].codec);
          ProtocolProcessor* protocol_processor = ProtocolProcessorFactory::CreateProtocolCodec(pd);
          protocol_processor_controller_->AddProtocolProcessor(protocol_processor);
        }
      }
      ProtocolDescription* pd = new ProtocolDescription(PRODEUCT_SYSTEM_ID, MAJOR_VERSION_SYSTEM_ID,MINOR_VERSION_SYSTEM_ID,CT_BINARY);
      ProtocolProcessor* protocol_processor = ProtocolProcessorFactory::CreateProtocolCodec(pd);
      protocol_processor_controller_->AddProtocolProcessor(protocol_processor);
    }
    delete mdf_message;
  }
}

void wmdf::FeedConnection::OnRecvMsg( Message* msg )
{
  cur_size_ += msg->msg_len;

  //the first message is login response, process login response
  if (msg->product_id == PRODEUCT_SYSTEM_ID) 
  {
    //登录响应消息、心跳消息不暴露给用户
    if (msg->message_id==SYSTEM_LOGIN_RESPONSE_ID)
    {
      ProcessLoginResponse(msg);
      return;
    }
    else if(msg->message_id==SYSTEM_CONFIG_INFO_ID)
    {
      ProcessConfigResponse(msg);
      return;
    }
    else if(msg->message_id==SYSTEM_UPDATE_CONFIG_RESPONSE_ID)
    {
      ProcessUpdateConfigResponse(msg);
      return;
    }
    else if (msg->message_id == SYSTEM_HEARTBEAT_ID)
    {
      last_heart_beat_time_ = DateTime::get_current_time();
      return;
    }
  }
  
  last_heart_beat_time_ = DateTime::get_current_time();
  MdfRecord* record = protocol_processor_controller_->DecodeToRecord(msg);
  DoFilter(record);
  DoCallback(record);
}

void wmdf::FeedConnection::DoFilter( MdfRecord*& record )
{
  if ((NULL != record) && (record->product_id != PRODEUCT_SYSTEM_ID) && (secid_filter_masks_.size()>0))
  {
    MdfRecord* filtered_record = FilterRecord(record);
    Utils::DestroyMDFRecord(record);
    if (filtered_record!=NULL)
    {
      if (filtered_record->record_count > 0)
      {
        record = filtered_record;
      }
      else
      {
        Utils::DestroyMDFRecord(filtered_record);
        record = NULL;
      }
    }
    else
    {
      record = NULL;
    }
  }
}

void wmdf::FeedConnection::DoCallback( MdfRecord* record )
{
  if (NULL != record)
  {    
    if (NULL != record_callback_)//if callback is not null then call it;
    {
      RecordArgs args;
      args.connection_id=connection_id_;
      args.record = record;
      args.state = state_;
      record_callback_(&args);
    }
    else                        //else push data to queue
    {
      record_queue_.write(record,false);
      record_queue_.flush();
    }
  }
}

MdfRecord* wmdf::FeedConnection::RecvRecord()
{
  MdfRecord* record=NULL;
  while(feed_client_->is_connected())
  {
    if(record_queue_.read(&record))
      return record;
    else
      Utils::SleepTime(1);
  }
  return record;
}

void wmdf::FeedConnection::set_record_callback( RecordCallback record_callback )
{
  record_callback_ = record_callback;
}

void wmdf::FeedConnection::set_event_callback( FeedEventCallback event_callback )
{
  event_callback_ = event_callback;
}

void wmdf::FeedConnection::set_state( void* state )
{
  state_ = state;
}

void wmdf::FeedConnection::set_filter_masks( const std::string& filter_masks )
{
  if (!feed_client_->is_connected())
  {
    secid_filter_masks_.clear();
    Splitter spliter(filter_masks,",");
    for (uint32_t index=0;index<spliter.size();index++)
    {
      secid_filter_masks_.insert(make_pair(spliter[index],index));
    }
  }
}

void wmdf::FeedConnection::OnCloseCallback( EventMsg* args )
{
  cur_size_ = 0;
  pre_size_ = 0;
  last_heart_beat_time_ = 0;
  if (NULL != event_callback_)
  {
    EventArgs event_args = {0};
    event_args.state = state_;
    event_args.connection_id = this->connection_id_;
    event_args.level = LL_WARN;
    event_args.event_id = MEI_SOURCE_DISCONNECTED;
    event_args.date_time = DateTime::now();

    std::string at_where = "feed connection";
    uint32_t at_where_len = at_where.length() < 60 ? at_where.length() :59;
    memcpy(event_args.at_where,at_where.c_str(),at_where_len);
    std::string what_happened = StrUtils::formatstring(300,
      "Connection disconnected, ip address:%s, port:%d,product id:%d",
      feed_client_->server_address().c_str(),feed_client_->server_port(),product_source_->product_id());
    uint32_t what_happened_len = what_happened.length()  < 150 ? what_happened.length() :149;
    memcpy(event_args.what_happened,what_happened.c_str(),what_happened_len);
    event_callback_(&event_args);
//     if (NULL != owner_)
//     {
//       owner_->ReturnConnection(this->connection_id_);
//       owner_->CommitReturn(this->connection_id_);
//     }
  }
}

void wmdf::FeedConnection::OnEventCallback( EventMsg* args )
{
  if (NULL != event_callback_)
  {
    EventArgs event_args = {0};
    event_args.state = state_;
    event_args.connection_id = this->connection_id_;
    event_args.level = LL_INFO;
    event_args.event_id = MEI_NOMAL_EVENT;
    event_args.date_time = DateTime::now();

    std::string at_where = "feed connection";
    uint32_t at_where_len = at_where.length() < 60 ? at_where.length() :59;
    memcpy(event_args.at_where,at_where.c_str(),at_where_len);
    std::string what_happened = args->Context;
    uint32_t what_happened_len = what_happened.length()  < 150 ? what_happened.length() :149;
    memcpy(event_args.what_happened,what_happened.c_str(),what_happened_len);
    event_callback_(&event_args);
  }
}

bool wmdf::FeedConnection::is_connected()
{
  if ( NULL != feed_client_ )
  {
    return feed_client_->is_connected();
  }
  return false;
}

bool wmdf::FeedConnection::is_market_time( int32_t curr_time )
{
  if ((curr_time >= market_begin_time_) &&
    (curr_time <= market_end_time_))
  {
    return true;
  }
  else
  {
    return false;
  }
}
void wmdf::FeedConnection::set_market_time( int32_t begin_time,int32_t end_time )
{
  market_begin_time_ = begin_time;
  market_end_time_ = end_time;
}

bool wmdf::FeedConnection::IsSecIdInFilter( const std::string& sec_id )
{
  map<std::string,int32_t>::iterator iter = secid_filter_masks_.find(sec_id);
  return (iter != secid_filter_masks_.end());
}

MdfRecord* wmdf::FeedConnection::FilterRecord( MdfRecord* src )
{
  for (int32_t index=0;index< src->record_count;index++)
  {
    switch (src->product_id)
    {
    case PRODUCT_SSE_LEVEL1_ID:
      {
        if (src->message_id == SSEL1_MARKETDATA_ID)
        {
          return Filter<MarketMessageHeader,L1MarketSnapshot>(src);
        } 
        break;
      }      
      
    case PRODUCT_SZSE_LEVEL1_ID:
      {
        if (src->message_id == SZSEL1_MARKETDATA_ID)
        {
          return Filter<MarketMessageHeader,L1MarketSnapshot>(src);
        } 
        break;
      }
    case PRODUCT_SSE_LEVEL2_ID:
      {
        if (src->message_id == SSEL2_MARKETDATA_ID)
        {
          return Filter<MarketMessageHeader,L2MarketSnapshot>(src);
        } 
        else if (src->message_id == SSEL2_TRANSACTION_ID)
        {
          return Filter<MarketMessageHeader,L2Transaction>(src);
        }
        else if (src->message_id == SSEL2_ORDERQUEUE_ID)
        {
          return FilterOrderQueue(src);
        }
        else if (src->message_id == SSEL2_INDEXDATA_ID)
        {
          return Filter<MarketMessageHeader,IndexSnapshot>(src);
        }
        break;
      }
    case PRODUCT_SZSE_LEVEL2_ID:
      {
        if (src->message_id == SZSEL2_MARKETDATA_ID)
        {
          return Filter<MarketMessageHeader,L2MarketSnapshot>(src);
        } 
        else if (src->message_id == SZSEL2_TRANSACTION_ID)
        {
          return Filter<MarketMessageHeader,L2Transaction>(src);
        }
        else if (src->message_id == SZSEL2_ORDERQUEUE_ID)
        {
          return FilterOrderQueue(src);
        }
        else if (src->message_id == SZSEL2_SINGLEORDER_ID)
        {
          return Filter<MarketMessageHeader,SZSEL2Order>(src);
        }
        else if (src->message_id == SSEL2_INDEXDATA_ID)
        {
          return Filter<MarketMessageHeader,IndexSnapshot>(src);
        }
        break;
      }
    case PRODUCT_CFFE_LEVEL1_ID:
      {
        if (src->message_id == CFFEL1_MARKETDATA_ID)
        {
          return Filter<MarketMessageHeader,FuturesMarketSnapshot>(src);
        } 
        break;
      }
    case PRODUCT_CFFE_LEVEL2_ID:
      {
        if (src->message_id == CFFEL2_MARKETDATA_ID)
        {
          return Filter<MarketMessageHeader,CFEL2MarketSnapshot>(src);
        } 
        break;
      }
    case PRODUCT_SHFE_LEVEL1_ID:
      {
        if (src->message_id == SHFEL1_MARKETDATA_ID)
        {
          return Filter<MarketMessageHeader,FuturesMarketSnapshot>(src);
        } 
        break;
      }
    case PRODUCT_DCE_LEVEL1_ID:
      {
        if (src->message_id == DCEL1_MARKETDATA_ID)
        {
          return Filter<MarketMessageHeader,FuturesMarketSnapshot>(src);
        } 
        break;
      }
    case PRODUCT_CZCE_LEVEL1_ID:
      {
        if (src->message_id == CZCEL1_MARKETDATA_ID)
        {
          return Filter<MarketMessageHeader,FuturesMarketSnapshot>(src);
        } 
        break;
      }
    case PRODUCT_FTSE_LEVEL1_ID:
      {
        if (src->message_id == FTSE_INDEXDATA_ID)
        {
          return Filter<MarketMessageHeader,IndexSnapshot>(src);
        } 
        break;
      }
		case PRODUCT_SWINDEX_LEVEL1_ID:
			{
				if(src->message_id == SW_INDEXDATA_ID )
				{
					return Filter<MarketMessageHeader,IndexSnapshot>(src);
				}
				break;
			}
    default:
      break;
    }    
  }
  return NULL;
}

MdfRecord* wmdf::FeedConnection::FilterOrderQueue( MdfRecord* src )
{
  MdfRecord* dst = new MdfRecord();
  memcpy(dst,src,sizeof(MdfRecord));
  dst->header = new MarketMessageHeader();
  memcpy(dst,src,sizeof(MarketMessageHeader));
  std::vector<int32_t> indexes;
  L2OrderQueue* body_src = (L2OrderQueue*)src->body;
  for (int32_t index=0;index < src->record_count;index++)
  {
    if (IsSecIdInFilter(body_src->wind_code))
    {
      indexes.push_back(index);
    }
  }
  dst->record_count = indexes.size();
  if (dst->record_count > 0)
  {
    dst->body = new L2OrderQueue[indexes.size()];
    L2OrderQueue* body_dst = (L2OrderQueue*)dst->body;
    for (uint32_t index=0;index<indexes.size();index++)
    {
      L2OrderQueue* order_src = body_src+indexes[index];
      L2OrderQueue* order_dst = body_dst+index;
      memcpy(order_dst,order_src,sizeof(L2OrderQueue));
      if (order_dst->price_level_count > 0)
      {
        order_dst->price_levels = new L2OrderQueuePriceLevel[order_src->price_level_count];
        for (int32_t subindex=0;subindex < order_src->price_level_count;subindex++)
        {
          L2OrderQueuePriceLevel* pl_src = order_src->price_levels+subindex;
          L2OrderQueuePriceLevel* pl_dst = order_dst->price_levels+subindex;
          memcpy(pl_dst,pl_src,sizeof(L2OrderQueuePriceLevel));
          if (pl_dst->orders_number > 0)
          {
            pl_dst->order_items = new L2OrderQueueItem[pl_src->orders_number];
            for (int32_t subsubIndex=0;subsubIndex < pl_src->orders_number;subsubIndex++)
            {
              L2OrderQueueItem* oi_src = pl_src->order_items+subsubIndex;
              L2OrderQueueItem* oi_dst = pl_dst->order_items+subsubIndex;
              memcpy(oi_dst,oi_src,sizeof(L2OrderQueueItem));
            }
          }
          else
          {
            pl_dst->order_items = NULL;
          }
        }
      }
      else
      {
        order_dst->price_levels = NULL;
      }
    }
  }
  else
  {
    dst->body = NULL;
  }
  return dst;
}


void wmdf::FeedConnection::set_owner( FeedConnectionPool* owner )
{
  owner_ = owner;
}

void wmdf::FeedConnection::ResetContext()
{
  last_heart_beat_time_ = 0;
}

//*********************************************************//
//*****************FeedClientPool*************************//
//*********************************************************//

wmdf::FeedConnectionPool::FeedConnectionPool( int32_t max_size )
{
  isworking_ = false;
  for (int32_t index=0;index< max_size;index++)
  {
    FeedConnection* feed_connection = new FeedConnection(index);
    feed_connection_pool_.push_back(feed_connection);
    feed_connection->set_owner(this);
    connection_states_.push_back(false);
    active_connections_.push_back(NULL);
  }
}

wmdf::FeedConnectionPool::~FeedConnectionPool()
{
  Shutdown();
  for(uint32_t index=0;index<feed_connection_pool_.size();index++)
  {
    delete feed_connection_pool_[index];
  }
  feed_connection_pool_.clear();
}

wmdf::FeedConnectionPool* wmdf::FeedConnectionPool::instance()
{
  return instance_;
}

bool wmdf::FeedConnectionPool::BorrowConnection( int32_t client_id )
{
  boost::mutex::scoped_lock lock(mutex_);
  if ((client_id >= 0) 
    && (client_id < FeedConnectionPool::max_num_of_connections))
  {
    connection_states_[client_id] = true;
    return true;
  }
  return false;
}

void wmdf::FeedConnectionPool::CommitBorrow( int32_t connection_id )
{
  boost::mutex::scoped_lock lock(mutex_);
  if ((connection_id >= 0) 
    && (connection_id < FeedConnectionPool::max_num_of_connections)
    && (connection_states_[connection_id]))
  {
    active_connections_[connection_id] = feed_connection_pool_[connection_id];  
  }
}

void wmdf::FeedConnectionPool::ReturnConnection( int32_t connection_id )
{
  boost::mutex::scoped_lock lock(mutex_);
  if ((connection_id >= 0) 
    && (connection_id < FeedConnectionPool::max_num_of_connections))
  {
    connection_states_[connection_id] = false;
  }
}

void wmdf::FeedConnectionPool::CommitReturn( int32_t connection_id )
{
  boost::mutex::scoped_lock lock(mutex_);
  if ((connection_id >= 0) 
    && (connection_id < FeedConnectionPool::max_num_of_connections) 
    )
  {
    active_connections_[connection_id] = NULL;
  }
}

bool wmdf::FeedConnectionPool::IsAllInUsing()
{
  boost::mutex::scoped_lock lock(mutex_);
  bool isallinusing = true;
  for(uint32_t index=0;index < feed_connection_pool_.size();index++)
  {
    isallinusing = isallinusing && feed_connection_pool_[index]->is_connected();
  }
  return isallinusing;
}

void wmdf::FeedConnectionPool::Startup()
{
  if (!isworking_)
  {
    isworking_ = true;
    work_thread_ = new boost::thread(boost::bind(&FeedConnectionPool::Guard,this));
  }
}

void wmdf::FeedConnectionPool::Shutdown()
{
  if (isworking_)
  {
    isworking_ = false;
    work_thread_->join();
  }
  if (work_thread_ != NULL)
  {
    delete work_thread_;
    work_thread_ = NULL;
  }
}

void wmdf::FeedConnectionPool::Guard()
{
  while(isworking_)
  {
    //检查connection工作情况
    GuardConnections();
    Utils::SleepTime(500);
  }
}

void wmdf::FeedConnectionPool::GuardConnections()
{
  boost::mutex::scoped_lock lock(mutex_);

  for (uint32_t index=0;index<active_connections_.size();index++)
  {
    FeedConnection* connection = active_connections_[index];
    if ((NULL != connection) && (connection_states_[connection->connection_id()]))
    {
      
      if (connection->is_connected() && (connection->last_heart_beat_time() > 0))
      {
        time_stamp_t curr_time = DateTime::get_current_time();
        time_stamp_t time_span = curr_time - connection->last_heart_beat_time();
        //如果超过60S接收不到心跳包则断开连接
        if (time_span > 60000)
        {
          if(connection->pre_size()==connection->cur_size())
          {
            Logger::instance()->WriteFormatLog(LL_WARN,"heartbeat (%d, %d).",DateTime::time2hms(curr_time),DateTime::time2hms(connection->last_heart_beat_time()));
            connection->DisConnect();
            Utils::SleepTime(20);
          }
          else
          {
            connection->set_pre_size(connection->cur_size());
          }
        }
      }
      int32_t curr_time = DateTime::time2hms(DateTime::get_current_time()); 

      if ((!connection->is_connected()) && (connection->is_market_time(curr_time)))
      {
        if (connection->Connect())
        {
          if(!connection->Login())
          {
            connection->DisConnect();
          }
        }
      }
    }
  }
}

