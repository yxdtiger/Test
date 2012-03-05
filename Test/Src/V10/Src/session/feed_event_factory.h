#ifndef FEED_EVENT_FACTORY_H
#define FEED_EVENT_FACTORY_H
#include "../include/wmdf_api.h"
#include <string>
#include "../include/date_time.h"
#include <cstring>

namespace wmdf
{
  class FeedEventFactory
  {
  public:
    static EventArgs* CreateEventMsg( int32_t connection_id,int32_t event_no,std::string message,void* state)
    {
      EventArgs* temp=new EventArgs();
      temp->connection_id = connection_id;
      temp->EventId=event_no;
      temp->Time=DateTime::time2hmsms(DateTime::get_current_time());
      int32_t char_len = message.length()>=255?254:message.length();
      memcpy(temp->Context,message.c_str(),char_len);
      temp->Context[char_len]='\0';
      return temp;
    }
  };
}

#endif
