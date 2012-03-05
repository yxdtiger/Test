#include "message_factory.h"

#ifdef WIN32
#include <Windows.h>
#else
#include <time.h>
#include <sys/timeb.h>
#include <stdlib.h>		//add for linux
#include <string.h>		//add for linux
#endif

NET_API  Message* wmdf::MessageFactory::CreateMessage( size_t size )
{
  size=size+sizeof(Message)-sizeof(char);
  Message* msg = (Message*)malloc(size);
  msg->STX=MSG_START_TAG;
  msg->msg_len=size;
  msg->local_time= GetTimestamp();
  return msg;
}

NET_API  Message* wmdf::MessageFactory::CreateMessage( size_t size,uint8_t* data )
{
  Message* msg = CreateMessage(size);
  memcpy(msg->data,data,size);
  return msg;
}

Message* wmdf::MessageFactory::CreateRawMessage( size_t size )
{
  Message* msg = (Message*)malloc(size);
  return msg;
}

NET_API  void wmdf::MessageFactory::DestoryMessage( Message* msg )
{
  free(msg);
}

uint8_t wmdf::MessageFactory::CheckSum( uint8_t* data,int size )
{
  uint32_t check_sum =0;
  for (int i=0;i<size-1;i++)
  {
    check_sum +=data[i];
  }
  return check_sum%256;
}

uint32_t wmdf::MessageFactory::GetTimestamp()
{
#ifdef WIN32
  SYSTEMTIME tm;
  GetLocalTime(&tm);
  return LTIME(tm.wHour, tm.wMinute, tm.wSecond, tm.wMilliseconds);
#else
  struct timeb tp;
  struct tm *tm;
  ftime(&tp);
  tm = localtime (&(tp.time));
  return LTIME(tm->tm_hour, tm->tm_min, tm->tm_sec, tp.millitm);
#endif
}
