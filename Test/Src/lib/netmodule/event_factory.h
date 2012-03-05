#ifndef WIND_NET_LOGGER_
#define WIND_NET_LOGGER_

namespace wmdf{

class EventFactory
{
public:
  static EventMsg* CreateEventMsg( int event_no,std::string message,EventState state )
  {
    EventMsg* temp=new EventMsg();
    temp->State=state;
    temp->EventId=event_no;
    int32_t char_len = message.length()>=79?78:message.length();
    memcpy(temp->Context,message.c_str(),char_len);
    temp->Context[char_len]='\0';
    temp->Time=time(0);
    return temp;
  }
};
}//end namespace
#endif
