#ifndef STR_UTILS_H
#define STR_UTILS_H

#include <string>
#include <cstdarg>
#include <algorithm>
#include <cctype>

#include "../include/wmdf_api.h"
#ifdef __linux__
#include <cstdio>
#endif
using namespace std;

namespace wmdf
{
  class StrUtils
  {
  public:
    StrUtils(void);
    virtual ~StrUtils(void);
    static string LTrim(const string& str)
    {
      if (str != "")
        return str.substr(str.find_first_not_of(" \n\r\t"));
      else
        return "";
    }

    static string RTrim(const string& str)
    {
      if (str != "")
        return str.substr(0,str.find_last_not_of(" \n\r\t")+1);
      else
        return "";
    }

    static string Trim(const string& str)
    {
      if (str != "")
        return LTrim(RTrim(str));
      else
        return "";
    }

    static string ToUpper(const string& str)
    {
      string out(str.length(),'X');
      //transform(str.begin(),str.end(),out.begin(),toupper);
      for (uint32_t index = 0; index< str.length();index++)
      {
        out[index] = toupper(str[index]);
      }
      return out;
    }
    static string ToLower(const string& str)
    {
      string out(str.length(),'X');
      for (uint32_t index = 0; index< str.length();index++)
      {
        out[index] = tolower(str[index]);
      }
      //transform(str.begin(),str.end(),out.begin(),tolower);
      return out;
    }

    static string formatstring(int32_t length,string format,...)
    {
      char* buff= new char[length];
      va_list pArgList;
      va_start (pArgList,format);
      #pragma warning(disable : 4996)
      vsprintf(buff,format.c_str(),pArgList);
      #pragma warning(default : 4996)
      va_end(pArgList);
      string str = std::string(buff);
      delete buff;
      return str;
      return NULL;
    }

    static inline uint8_t* Str3Find(uint8_t* src,uint8_t* dest,uint8_t* end)
    {
      //查找长度为3的字符串
      if (end-src<3)
      {
        return NULL;
      }
      for(int32_t index =0; index<end-src-2; index++)
      {
        if ((src[index] == dest[0])
          && (src[index+1] == dest[1])
          && (src[index+2] == dest[2]))
        {
          return src+index;
        }
      }
      return NULL;
    }
    static inline uint8_t* Str2Find(uint8_t* src,uint8_t* dest,uint8_t* end)
    {
      //查找长度为3的字符串
      if (end-src<2)
      {
        return NULL;
      }
      for(int32_t index =0; index<end-src-1; index++)
      {
        if ((src[index] == dest[0])
          && (src[index+1] == dest[1]))
        {
          return src+index;
        }
      }
      return NULL;
    }
    static inline uint8_t* StrFind(uint8_t* src, uint8_t* dest, uint8_t* end)
    {
      uint8_t *p = src;
      uint8_t *q = dest;
      int32_t index=-1;
      uint8_t *tmp;
      if(src==NULL)
        return NULL;
      while (p != end)
      {
        tmp = p;
        ++index;
        while ( *tmp == *q&&q != end && tmp != end)
        {
          ++tmp;
          ++q;
        }
        if (*q == '\0') //匹配成功，返回结果
          return src+index;
        else //主串和模式串回溯
        {
          ++p;
          q = dest;
        }
      }
      return NULL; //匹配失败
    }
  };
}

#endif
