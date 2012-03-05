#pragma once
#include "afxcmn.h"
#include <string>
#include "../../include/wmdf_structs.h"
class UIUilties
{
public:
  UIUilties(void);
  ~UIUilties(void);
public:
  static std::string IpToString(DWORD ip_addr);
  static DWORD StringToIp(std::string ip_addr);
};
