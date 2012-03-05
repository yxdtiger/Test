#ifndef DATE_TIME_H
#define DATE_TIME_H

#include "wmdf_api.h"
#include <iostream>
#include "time.h"
#include "math.h"


#if defined(_WIN32) || defined(_WIN32_WCE) || defined(__WIN32__)
#  ifndef WIN32
#    define WIN32
#  endif
#endif

#ifdef WIN32
#include "Windows.h"
#elif __linux__
#include <time.h>
#include <sys/timeb.h>
#include <cstdio>
#endif

namespace wmdf
{
  class DateTime
  {
  public:
    DateTime(void);
    virtual ~DateTime(void);
    static const time_stamp_t  TotalMsSecond= 86400000; //一天总的毫秒数

    /************************************************************************/
    /*               Decode Date and time                                   */
    /************************************************************************/
    static void minute2hm (time_stamp_t time,int32_t& hour,int32_t& minute) {
      hour = time / 60;
      minute = time % 60;
      //hour, minute is wanted
    }

    static void second2hms (time_stamp_t time,int32_t& hour,int32_t& minute,int32_t& second) {
      hour = time / 3600;
      minute = time % 3600 / 60;
      second = time % 60;
      //hour, minute, second is wanted
    }

    static void time2hmsms (time_stamp_t time,int32_t& hour,int32_t& minute,int32_t& second,int32_t& msecond) {
      hour = time / 3600000;
      minute = time % 3600000 / 60000;
      second = time % 3600000 % 60000 / 1000;
      msecond = time % 1000;
      //hour, minute, second, msecond is wanted
    }

    static void month2ym (date_t date, int32_t& year, int32_t& month) {
      year = (date + 2000 * 12)/12;
      month = (date + 2000 * 12)%12;
      if (month == 0) {year--; month = 12;}
      //year, month is wanted
    }

    static void date2ymd (date_t date, int32_t& year,int32_t& month,int32_t& day) {
      int32_t y = (date / 365 + 300) % 602;
      year = date <= DATEMAP[y] ? (date > DATEMAP[y-1] ? y + 1699 : y + 1698 ) : y + 1700;
      int32_t iyd = (date + BASEDAYS - DURA(year)) % 367;
      int32_t md = IYD2MD[LEAP(year) ? iyd-1 : (iyd<60) ? iyd-1 : iyd];
      month = md / 100;
      day = md % 100;
      //year, month, day is wanted
    }

    static void datetime2ymdhmsms (date_time_t datetime, int32_t& year, int32_t& month, int32_t& day,int32_t& hour,int32_t& minute,int32_t& second,int32_t& msecond ) {
      int32_t i = (int32_t)floor(datetime);
      int32_t y = (i / 365 + 300) % 602;
      year = i <= DATEMAP[y] ? (i > DATEMAP[y-1] ? y + 1699 : y + 1698 ) : y + 1700;
      int32_t iyd = (i + BASEDAYS - DURA(year)) % 367;
      int32_t md = IYD2MD[LEAP(year) ? iyd-1 : (iyd<60) ? iyd-1 : iyd];
      month = md / 100;
      day = md % 100;

      int32_t t = (int32_t)((datetime - i) * SECONDSPERDAY + 0.01); /* 0.01 is a choosen number, may cause error!! */
      hour = t / 3600000;
      minute = t % 3600000 / 60000;
      second = t % 3600000 % 60000 / 1000;
      msecond = t % 1000;
      //year, month, day, hour, minute, second, msecond is wanted
    }

    static time_stamp_t get_current_time()
    {
      #ifdef WIN32
        SYSTEMTIME tm;
        GetLocalTime(&tm);
        return GTIME(tm.wHour, tm.wMinute, tm.wSecond, tm.wMilliseconds);
      #elif __linux__
        struct timeb tp;
        struct tm *tm;
        ftime(&tp);
        tm = localtime (&(tp.time));
        return GTIME (tm->tm_hour, tm->tm_min, tm->tm_sec, tp.millitm);
      #endif
    }

    static date_t today()
    {
      #ifdef WIN32
        SYSTEMTIME tm;
        GetLocalTime(&tm);
        return GDATE(tm.wYear, tm.wMonth, tm.wDay);
      #elif __linux__
        struct timeb tp;
        struct tm *tm;
        ftime(&tp);
        tm = localtime (&(tp.time));
        return GDATE (tm->tm_year,tm->tm_mon,tm->tm_mday);
      #endif
    }

    static date_time_t now()
    {
      #ifdef WIN32
        SYSTEMTIME tm;
        GetLocalTime(&tm);
        return GDATETIME(tm.wYear, tm.wMonth, tm.wDay ,tm.wHour, tm.wMinute, tm.wSecond, tm.wMilliseconds);
      #elif __linux__
        struct timeb tp;
        struct tm *tm;
        ftime(&tp);
        tm = localtime (&(tp.time));
        return GDATETIME (tm->tm_year,tm->tm_mon,tm->tm_mday, tm->tm_hour, tm->tm_min, tm->tm_sec, tp.millitm);
      #endif
    }

    static int32_t hour()
    {
       return (get_current_time() / 3600000);
    }
    static time_stamp_t calctimespan(time_stamp_t time1,time_stamp_t time2)
    {
      time_stamp_t time_span = time1-time2;
      if (time_span<0)
      {
        time_span += DateTime::TotalMsSecond;
      }
      return time_span;
    }

    /************************************************************************/
    /*               Encode Date and time                                   */
    /************************************************************************/
    static time_stamp_t hm2minute(int32_t hour,int32_t minute)
    {
      return GMINUTE(hour,minute);
    }
    static time_stamp_t hms2second(int32_t hour,int32_t minute,int32_t second )
    {
      return GSECOND(hour,minute,second);
    }
    static time_stamp_t hmsms2time(int32_t hour,int32_t minute,int32_t second,int32_t msecond )
    {
      return GTIME(hour,minute,second,msecond);
    }
    static date_t ym2month( int32_t year, int32_t month )
    {
      return GMONTH(year,month);
    }
    static date_t ymd2date( int32_t year, int32_t month, int32_t day )
    {
      return GDATE(year,month,day);
    }
    static date_time_t ymdhmsms2datetime( int32_t year, int32_t month, int32_t day,int32_t hour,int32_t minute,int32_t second,int32_t msecond )
    {
      return GDATETIME(year,month,day,hour,minute,second,msecond);
    }
    static int32_t week(int32_t year, int32_t month, int32_t day)
    {
        return GWEEK(year,month,day);
    }

    //add 20110811 to date
    static date_t ymd2date(int32_t ymd)
    {
      int32_t year=ymd/10000;
      int32_t month=(ymd-year*10000)/100;
      int32_t day = ymd-year*10000-month*100;
      return ymd2date(year,month,day);
    }

    static time_stamp_t hms2time(int32_t hms)
    {
      int32_t hour = hms/10000;
      int32_t minute = (hms-hour*10000)/100;
      int32_t second = hms-hour*10000-minute*100;
      return hms2second(hour,minute,second);
    }
    static time_stamp_t hmsms2time(int32_t hmsms)
    {
      int32_t hour = hmsms/1000000;
      int32_t minute = (hmsms-hour*1000000)/10000;
      int32_t second = (hmsms-hour*1000000-minute*10000)/100;
      int32_t millisecond = (hmsms-hour*1000000-minute*10000-second*100)*10;
      return hmsms2time(hour,minute,second,millisecond);
    }

    static int32_t date2ymd(date_t date)
    {
      int32_t y = (date / 365 + 300) % 602;
      int32_t year = date <= DATEMAP[y] ? (date > DATEMAP[y-1] ? y + 1699 : y + 1698 ) : y + 1700;
      int32_t iyd = (date + BASEDAYS - DURA(year)) % 367;
      int32_t md = IYD2MD[LEAP(year) ? iyd-1 : (iyd<60) ? iyd-1 : iyd];
      int32_t month = md / 100;
      int32_t day = md % 100;
      return year*10000+month*100+day;
    }
    static std::string date2ymd(date_t date,const std::string& delimiter)
    {
      int32_t y = (date / 365 + 300) % 602;
      int32_t year = date <= DATEMAP[y] ? (date > DATEMAP[y-1] ? y + 1699 : y + 1698 ) : y + 1700;
      int32_t iyd = (date + BASEDAYS - DURA(year)) % 367;
      int32_t md = IYD2MD[LEAP(year) ? iyd-1 : (iyd<60) ? iyd-1 : iyd];
      int32_t month = md / 100;
      int32_t day = md % 100;
      char str[11];
      sprintf(str,"%4d%s%02d%s%02d",year,delimiter.c_str(),month,delimiter.c_str(),day);
      return std::string(str);
    }

    static int32_t time2hms(time_stamp_t time)
    {
      int32_t hour = time / 3600000;
      int32_t minute = time % 3600000 / 60000;
      int32_t second = time % 3600000 % 60000 / 1000;
      return hour*10000+minute*100+second;
    }
    static std::string time2hms(time_stamp_t time,const std::string& delimiter)
    {
      int32_t hour = time / 3600000;
      int32_t minute = time % 3600000 / 60000;
      int32_t second = time % 3600000 % 60000 / 1000;
      char str[10];
      sprintf(str,"%02d%s%02d%s%02d",hour,delimiter.c_str(),minute,delimiter.c_str(),second);
      return std::string(str);
    }


    static int32_t time2hmsms(time_stamp_t time)
    {
      int32_t hour = time / 3600000;
      int32_t minute = time % 3600000 / 60000;
      int32_t second = time % 3600000 % 60000 / 1000;
      int32_t msecond = time % 1000;
      return hour*10000000+minute*100000+second*1000+msecond;
    }

    static std::string time2hmsms(time_stamp_t time,const std::string& delimiter)
    {
      int32_t hour = time / 3600000;
      int32_t minute = time % 3600000 / 60000;
      int32_t second = time % 3600000 % 60000 / 1000;
      int32_t msecond = time % 1000;
      char str[13];
      sprintf(str,"%02d%s%02d%s%02d.%03d",hour,delimiter.c_str(),minute,delimiter.c_str(),second,msecond);
      return std::string(str);
    }
  };
}

#endif
