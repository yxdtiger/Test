#include "message_publisher.h"
#include "../environment/utils.h"
#include "../environment/server_environment.h"

wmdf::MessagePublisher::MessagePublisher()
{
  
}

wmdf::MessagePublisher::~MessagePublisher(void)
{
  
}

void wmdf::MessagePublisher::PublishOneMessage( SharedMdfMessage message )
{
  ServerEnvironment::feed_server()->subscriber_manager()->PublishMessage(message);
}

void wmdf::MessagePublisher::PublishOneMessage( SharedRelayMdfMessage message )
{
  ServerEnvironment::feed_server()->subscriber_manager()->PublishMessage(message);
}

void wmdf::MessagePublisher::PublishMonitorMessage( SharedMdfMessage message )
{
  ServerEnvironment::monitor_server()->subscriber_manager()->PublishMessage(message);
}