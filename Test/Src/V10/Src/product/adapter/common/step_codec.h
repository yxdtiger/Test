#ifndef STEP_CODEC_H
#define STEP_CODEC_H

#include <cassert>
#include <cstring>

namespace wmdf
{
  class StepDecoder
  {
  private:
    char* header_;
    char* tailer_;
    char* end_;
  public:
    inline void ReSet(char*buffer,char* tail)
    {
      header_=buffer;
      tailer_=buffer;
      end_=tail;
    }

    inline int CheckTag(const char* tag)
    {
      bool ret= EqualStr(tailer_,tag);
      if(!ret) return 0;
      return strlen(tag)+1;//结束符和‘=’
    }

    inline bool Next(int len)
    {
      tailer_+=len;
      return true;
    }

    inline int32_t DecodeInt32()
    {
//       char* end_ptr;
//       int32_t ret=strtol(tailer_,&end_ptr,10);
//       tailer_=end_ptr+1;
//       return ret;
      int32_t ret = (int32_t)ParseInt64(&tailer_);
      tailer_+=1;
      return ret;
    }

    inline char DecodeChar()
    {
      char ret = *tailer_;
      tailer_+=2;
      return ret;
    }

    inline int64_t DecodeInt64()
    {
//       char* end_ptr;
//       int64_t ret=strtol(tailer_,&end_ptr,10);
//       tailer_=end_ptr+1;
//       return ret;
      int64_t ret = ParseInt64(&tailer_);
      tailer_+=1;
      return ret;
    }

    inline double ParseDouble(char **str)
    {
      char *cc = *str;

      bool neg = (*cc == '-');
      if(neg) ++cc;

      double value = 0, e = 1;

      for(; *cc != '.'; ++cc)
      {
        if(*cc < '0' || *cc > '9')
        {
          *str = cc;
          return neg ? -value : value;
        }

        value *= 10;
        value += *cc - '0';
      }

      for(++cc;; ++cc)
      {
        if(*cc < '0' || *cc > '9')
        {
          *str = cc;
          return neg ? -value : value;
        }

        e /= 10;
        value += (*cc - '0') * e;
      }
    }



    inline int64_t ParseInt64(char **str)
    {
      char *cc = *str;

      bool neg = (*cc == '-');
      if(neg) ++cc;

      int32_t value = 0, e = 1;
      while(true)
      {
        if(*cc < '0' || *cc > '9')
        {
          *str = cc;
          return neg ? -value : value;
        }

        value *= 10;
        value += *cc - '0';
        ++cc;
      }
      return -1;
    }

    inline double DecodeDouble()
    {
      //char* end_ptr;
      //double ret=strtod(tailer_,&end_ptr);
      //tailer_=end_ptr+1;
      double ret = ParseDouble(&tailer_);
      tailer_+=1;
      return ret;
    }

    inline char* DecodeString(char* str,int str_len)
    {
      char* tmp =tailer_;
      int count=0;
      while(*tmp>1){count++;tmp++;}
      assert(str_len>=count);
      memcpy(str,tailer_,count);
      str[count]='\0';
      tailer_+=count+1;
      return str;
    }

    inline int32_t DecodeSZSETimeStamp()
    {
      tailer_+=9;//跳过年份和‘-’
      int64_t hour=ParseInt64(&tailer_);
      tailer_+=1;
      int64_t minute=ParseInt64(&tailer_);
      tailer_+=1;
      int64_t second=ParseInt64(&tailer_);
      tailer_+=1;
      int64_t ms =ParseInt64(&tailer_);
      tailer_+=1;
      return (int32_t)(hour*10000000+minute*100000+second*1000+ms);
    }

		inline int DecodeSSETimeStamp()
		{

			int64_t hour = ParseInt64(&tailer_);
			tailer_+=1;
			int64_t minute = ParseInt64(&tailer_);
			tailer_+=1;
			int64_t second = ParseInt64(&tailer_);
			tailer_+=1;
			return (int32_t)(hour*10000+minute*100+second);
		}

    inline bool Empty()
    {
      return end_==tailer_;
    }

  private:
    inline bool EqualStr(const char* src,const char *dest)
    {
      assert(src!=NULL&&dest!=NULL);

      while(*src>1&&*dest&&*src==*dest){
        src++;dest++;
      }
      return (*src=='='||*src<2)&&*dest==0;
    }
  };
}

#endif
