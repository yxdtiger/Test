#ifndef DATADECODER_H_
#define DATADECODER_H_

#include <vector>
#include "../../include/wmdf_api.h"
#include "../../include/wmdf_structs.h"


namespace wmdf
{
  class PacketDecoder;
  class IndexBase;
  class SecurityContainerManager;

  class DataDecoder
  {
  public:
    DataDecoder();
    ~DataDecoder(); 
    bool DecodeOnePacket( const unsigned long& buflen, uint8_t* buf);
    SecurityContainerManager* get_SecurityContainerManager(){return scm_;}

  private:
    bool DecodeOneWindCode(PacketDecoder* pd);
    IndexBase* ReadTime(PacketDecoder* pd);
    IndexBase* DecodeL2_OrderQueue(PacketDecoder* pd,bool is_bond);
    IndexBase* DecodeL2_Transactions(PacketDecoder* pd,bool is_bond);
    IndexBase* DecodeL2_Orders(PacketDecoder* pd,bool is_bond);
    IndexBase* DecodeStreamPacket(PacketDecoder* pd);
    IndexBase* Decode_Int64(uint16_t index, PacketDecoder* pd);
    //传进来的wind_code的结构必定为 char[16]
    inline bool IsBond(const char* wind_code)
    {
      //;债券 上海
      // Code=009*.SH|3=1000|8=10|59=1|253=10
      // Code=010*.SH|3=1000|8=10|59=1|253=10
      // Code=019*.SH|3=1000|8=10|59=1|253=10
      // Code=020*.SH|3=1000|8=10|59=1|253=10
      // Code=090*.SH|3=1000|8=10|59=1|253=10
      // Code=099*.SH|3=1000|8=10|59=1|253=10
      // Code=1*.SH|3=1000|8=10|59=1|253=10
      // Code=2*.SH|3=1000|8=10|59=1|253=10

      //;债券 深圳
      //Code=10*.SZ|3=1000|8=1|59=1|253=10
      //Code=11*.SZ|3=1000|8=1|59=1|253=10
      //Code=12*.SZ|3=1000|8=1|59=1|253=10
      //Code=13*.SZ|3=1000|8=1|59=1|253=10

      //上海
      if (wind_code[8]=='H') 
      {
        if ((wind_code[0] == '1') || (wind_code[0]=='2'))
        {
          return true;
        }
        else if (wind_code[0] == '0')
        {
          if ((wind_code[1]=='0') && (wind_code[2]=='9'))  
          {
            return true;
          }
          else if ((wind_code[1]=='1') && (wind_code[2]=='0'))
          {
            return true;
          }
          else if ((wind_code[1]=='1') && (wind_code[2]=='9'))
          {
            return true;
          }
          else if ((wind_code[1]=='2') && (wind_code[2]=='0'))
          {
            return true;
          }
          else if ((wind_code[1]=='9') && (wind_code[2]=='0'))
          {
            return true;
          }
          else if ((wind_code[1]=='9') && (wind_code[2]=='9'))
          {
            return true;
          }
        }
        return false;
      }
      //深交所
      else if (wind_code[8]=='Z') 
      {
        if ((wind_code[0]=='1') && (wind_code[1]=='0'))  
        {
          return true;
        }
        else if ((wind_code[0]=='1') && (wind_code[1]=='1'))
        {
          return true;
        }
        else if ((wind_code[0]=='1') && (wind_code[1]=='2'))
        {
          return true;
        }
        else if ((wind_code[0]=='1') && (wind_code[1]=='3'))
        {
          return true;
        }
        return false;
      }
      return false;
    }

  private:
    SecurityContainerManager* scm_;
  };
}

#endif
