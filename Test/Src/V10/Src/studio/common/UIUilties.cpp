#include "../main_frame/StdAfx.h"
#include "UIUilties.h"
#include "../../environment/utils.h"
#include <sstream>
#include <assert.h>
#include "../../environment/splitter.h"


using namespace std;
using namespace wmdf;

UIUilties::UIUilties(void)
{
}

UIUilties::~UIUilties(void)
{
}

string UIUilties::IpToString(DWORD ip_addr)
{
   unsigned long sAddr[4];
   DWORD ulAddr = ip_addr;
   sAddr[0] = ulAddr;
   sAddr[0] = ulAddr/(256*256*256);
   sAddr[1] = ulAddr%(256*256*256)/(256*256);
   sAddr[2] = ulAddr%(256*256)/256;
   sAddr[3] = ulAddr%256;

   stringstream ss;
   ss << sAddr[0] << "."  << sAddr[1] << "."  << sAddr[2] << "."  << sAddr[3];
   return ss.str();
}

DWORD UIUilties::StringToIp(string ip_addr)
{
  DWORD ip_address = 0;
  
  Splitter split(ip_addr, string("."));
  assert(split.size() == 4);
  
  int field1 = atoi(split[0].c_str());
  int field2 = atoi(split[1].c_str());
  int field3 = atoi(split[2].c_str());
  int field4 = atoi(split[3].c_str());

  field1 = field1 << 24;
  field2 = field2 << 16;
  field3 = field3 << 8;
  ip_address = field1|field2|field3|field4;

  return ip_address;
}
