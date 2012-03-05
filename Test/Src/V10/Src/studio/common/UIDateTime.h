#pragma once

#include <string>
#include "../../include/date_time.h"

class UIDateTime
{
public:
  UIDateTime(void);
  ~UIDateTime(void);
public:
  // date_t -> "2011-4-8"
  static std::string Date2String(const date_t& date, const std::string separator = "-");

  // "2011-4-8" -> date_t
  static date_t String2Date(const std::string& str_date, const std::string separator = "-");
};