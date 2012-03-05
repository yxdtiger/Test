#include "../include/wmdf_api.h"
#include <stdio.h>
#include "api_factory.h"
#include "api_version.h"
#include "../monitor/logger.h"
#include "../environment/utils.h"
#include "../session/feed_connection.h"
#include "../product/product_source.h"

using namespace wmdf;

WMDF_API eReturnValue wmdf_initialize()
{
  //to do initilaize api  
  FeedConnectionPool::instance();
  FeedConnectionPool::instance()->Startup();
  Logger::instance();
  return SUCCESS;
}

WMDF_API eReturnValue wmdf_finalize()
{
  FeedConnectionPool::instance()->Shutdown();
  return SUCCESS;
}

WMDF_API eReturnValue wmdf_open_connection(OpenArgs* args)
{
  if ((args->connection_id < 0) 
    || (args->connection_id >= FeedConnectionPool::max_num_of_connections) )
  {
    return CONNECTION_ID_INVALID;
  }
  //最多32个连接,检查是否都已经使用
  if (FeedConnectionPool::instance()->IsAllInUsing())
  {
    return CONNECTION_OVERLOAD;
  }
  
  FeedConnectionPool::instance()->BorrowConnection(args->connection_id);
  FeedConnection* feed_connection = FeedConnectionPool::instance()->feed_connections(args->connection_id);
  if (feed_connection->is_connected())
  {
    return CONNECTION_ID_INUSE;
  }
  //ps will be released by feed connection
  ProductSource* ps = ApiFactory::CreateProductSource(args);
  if (!feed_connection->Connect(ps))
  {
    FeedConnectionPool::instance()->ReturnConnection(args->connection_id);
    FeedConnectionPool::instance()->CommitReturn(args->connection_id);
    return CONNECTION_FAILED;
  }

  if (!feed_connection->Login())
  {
    FeedConnectionPool::instance()->ReturnConnection(args->connection_id);
    feed_connection->DisConnect();
    FeedConnectionPool::instance()->CommitReturn(args->connection_id);
    return AUTHORITY_INVALID;
  }
  else
  {
    //确认借阅，并设置需要监控的时间段
    feed_connection->set_market_time(args->marktet_open_time,args->markt_close_time);
    FeedConnectionPool::instance()->CommitBorrow(args->connection_id); 
  }
  return SUCCESS;
}


WMDF_API eReturnValue wmdf_open_connection_ex(OpenExArgs* args)
{
  if ((args->connection_id < 0) 
    || (args->connection_id >= FeedConnectionPool::max_num_of_connections) )
  {
    return CONNECTION_ID_INVALID;
  }
  //最多32个连接,检查是否都已经使用
  if (FeedConnectionPool::instance()->IsAllInUsing())
  {
    return CONNECTION_OVERLOAD;
  }

  FeedConnectionPool::instance()->BorrowConnection(args->connection_id);
  FeedConnection* feed_connection = FeedConnectionPool::instance()->feed_connections(args->connection_id);
  if (feed_connection->is_connected())
  {
    return CONNECTION_ID_INUSE;
  }
  //ps will be released by feed connection
  ProductSource* ps = ApiFactory::CreateProductSource(args);
  if (!feed_connection->Connect(ps))
  {
    FeedConnectionPool::instance()->ReturnConnection(args->connection_id);
    FeedConnectionPool::instance()->CommitReturn(args->connection_id);
    return CONNECTION_FAILED;
  }

  if (!feed_connection->Login())
  {
    FeedConnectionPool::instance()->ReturnConnection(args->connection_id);
    feed_connection->DisConnect();
    FeedConnectionPool::instance()->CommitReturn(args->connection_id);
    return AUTHORITY_INVALID;
  }
  else
  {
    //确认借阅，并设置需要监控的时间段
    feed_connection->set_market_time(args->marktet_open_time,args->markt_close_time);
    FeedConnectionPool::instance()->CommitBorrow(args->connection_id); 
  }
  return SUCCESS;
}

WMDF_API eReturnValue wmdf_close_connection(CloseArgs* args)
{
  if ((args->connection_id < 0) 
    || (args->connection_id >= FeedConnectionPool::max_num_of_connections) )
  {
    return CONNECTION_ID_INVALID;
  }
  FeedConnection* feed_connection = FeedConnectionPool::instance()->feed_connections(args->connection_id);
  FeedConnectionPool::instance()->ReturnConnection(args->connection_id);
  feed_connection->DisConnect();
  FeedConnectionPool::instance()->CommitReturn(args->connection_id);
  Utils::SleepTime(100);
  return SUCCESS;
}

WMDF_API eReturnValue wmdf_get_api_version(APIVersionArgs* args)
{
  args->major = API_VERSION_MAJOR;
  args->minor = API_VERSION_MINOR;
  args->patch = API_VERSION_PATCH;
  return SUCCESS;
}

WMDF_API eReturnValue wmdf_get_server_info(ServerInfoArgs* args)
{
  if ((args->connection_id < 0) 
    || (args->connection_id >= FeedConnectionPool::max_num_of_connections) )
  {
    return CONNECTION_ID_INVALID;
  }
  FeedConnection* feed_connection = FeedConnectionPool::instance()->feed_connections(args->connection_id);   
  if (!feed_connection->is_connected())  {return INVALID_VALUE;}
  strcpy((char*)args->server_addr,feed_connection->connected_server_address().c_str());
  args->server_port = feed_connection->connected_server_port();
  strcpy((char*)args->user_name,feed_connection->connected_user_name().c_str());
  strcpy((char*)args->password,feed_connection->connected_pass_word().c_str());
  return SUCCESS;
}

WMDF_API eReturnValue wmdf_set_filter_mask(FilterArgs* args)
{
  if ((args->connection_id < 0) 
    || (args->connection_id >= FeedConnectionPool::max_num_of_connections) )
  {
    return CONNECTION_ID_INVALID;
  }
  FeedConnection* feed_connection = FeedConnectionPool::instance()->feed_connections(args->connection_id);
  feed_connection->set_filter_masks((char*)args->filter_masks);
  return SUCCESS;
}

WMDF_API eReturnValue wmdf_set_callback(CallbackArgs* args)
{
  if ((args->connection_id < 0) 
    || (args->connection_id >= FeedConnectionPool::max_num_of_connections) )
  {
    return CONNECTION_ID_INVALID;
  }
  FeedConnection* feed_connection = FeedConnectionPool::instance()->feed_connections(args->connection_id);
  feed_connection->set_record_callback(args->record_callback);
  feed_connection->set_event_callback(args->event_callback);
  if (NULL != args->state)
  {
    feed_connection->set_state(args->state);
  }
  return SUCCESS;
}

WMDF_API eReturnValue  wmdf_get_record(RecordArgs* args)
{
  if ((args->connection_id < 0) 
    || (args->connection_id >= FeedConnectionPool::max_num_of_connections) )
  {
    return CONNECTION_ID_INVALID;
  }
  FeedConnection* feed_connection = FeedConnectionPool::instance()->feed_connections(args->connection_id);
  args->record = feed_connection->RecvRecord();
  args->state = NULL;
  return SUCCESS;
}

WMDF_API eReturnValue wmdf_release_record(RecordArgs* args)
{
  Utils::DestroyMDFRecord(args->record);
  return SUCCESS;
}
