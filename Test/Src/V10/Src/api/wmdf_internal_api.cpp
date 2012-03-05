#include "../include/wmdf_internal_api.h"
#include <stdio.h>
#include "../session/feed_connection.h"

using namespace wmdf;

WMDF_API ConfigInfo* wmdf_request_configinfo(ConfigInfoRequset* args)
{
  if ((args->connection_id < 0) 
    || (args->connection_id >= FeedConnectionPool::max_num_of_connections) )
  {
    return NULL;
  }
  FeedConnection* feed_connection = FeedConnectionPool::instance()->feed_connections(args->connection_id);
  ConfigInfo* config_info = feed_connection->RequestConfigInfo(args->config_type);
  if(config_info != NULL)
    config_info->connection_id = args->connection_id;
  return config_info;
}

WMDF_API eReturnValue wmdf_set_configinfo( ConfigInfo* args )
{
  FeedConnection* feed_connection = FeedConnectionPool::instance()->feed_connections(args->connection_id);
  return feed_connection->RequestModifyConfig(args);
}
