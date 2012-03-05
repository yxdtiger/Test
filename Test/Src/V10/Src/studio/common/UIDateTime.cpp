#include "../main_frame/StdAfx.h"
#include "UIDateTime.h"
#include <sstream>
#include <vector>
#include <assert.h>
#include "../../environment/utils.h"
#include "../../environment/splitter.h"

using namespace std;
using namespace wmdf;

UIDateTime::UIDateTime(void)
{
}

UIDateTime::~UIDateTime(void)
{
}

string UIDateTime::Date2String(const date_t& date, const std::string separator /* = "-" */)
{
  int year, month, day;
  DateTime::date2ymd(date, year, month, day);
  stringstream ss;
  ss << year << separator << month << separator << day;
  return ss.str();
}

date_t UIDateTime::String2Date(const std::string& str_date, const std::string separator /* = "-" */)
{
  Splitter Split(str_date, separator);
  assert(3 == Split.size());
  return DateTime::ymd2date( atoi(Split[0].c_str()), atoi(Split[1].c_str()), atoi(Split[2].c_str()) );
}