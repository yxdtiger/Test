#ifndef	FAST_CODEC_H
#define FAST_CODEC_H

#include "../../../include/wmdf_api.h"
#include <cassert>
#include <cstring>

namespace wmdf
{

static uint32_t bit_mask [8] = { 64, 32, 16, 8, 4, 2, 1, 0 };//to do pmap增加时需要修改

enum OperatorType{NONE,DEFAULT,COPY,CONSTANT,INCREASE,DELTA};
enum PresenceType{PT_NONE,PT_MANDATORY,PT_OPTION};


typedef struct tap_TValue {
  union{
    int32_t int32_value;
    int64_t int64_value;
    uint32_t uint32_value;
    uint64_t uint64_value;
    double double_value;
    uint8_t* str_value;
  };
} TValue ;
class FastDecoder
{

private:
  static const int  SIGN_MASK_I32 = 0x40;
  static const int SIGN_MASK_U32 = 0x00;
  static const int TYPE_MASK_I32 = 5;
  static const int TYPE_MASK_I64 = 10;
  static const int MAX_PMAP_BYTES = 8;
  static const int MAX_PMAP_BITS = 7 * MAX_PMAP_BYTES;
private:
  uint8_t* header_;
  uint8_t* tailer_;
  uint8_t* end_;
  uint8_t pmap_[MAX_PMAP_BITS];
  uint8_t* current_pmap_;
  uint8_t* parent_pmap_;
  int32_t parent_index_;
  int32_t index_;
  TValue* default_value_;
public:
  FastDecoder(int32_t items_count)
    :default_value_(new TValue[items_count]),current_pmap_(pmap_)
  {
    memset(default_value_,0,sizeof(TValue)*items_count);
  }

  ~FastDecoder()
  {
    delete[] default_value_;
  }

  void ReSet(uint8_t*buffer,size_t length)
  {
    header_=buffer;
    tailer_=buffer;
    end_=buffer+length;
    index_=0;
  }

  void SetDefaultValue(int32_t index)
  {
    memset(&default_value_[index],0,sizeof(TValue));
  }



  //当前假定Pmap的长度小于64位
  uint8_t* BeginNewMessage()
  {
    index_=0;
    uint8_t  data [MAX_PMAP_BYTES];
    int bytes = ParseBytes (data, MAX_PMAP_BYTES);
    memset(pmap_,0,MAX_PMAP_BITS);
    if (bytes < 0)
      assert(0);
    for (int p1 = 0,  offset = 0 ; p1 < bytes ; p1 ++, offset += 7)
    {
      uint32_t temp = data [p1];
      for (int p2 = 0 ; p2 < 7 ; p2 ++)
      {
        pmap_ [offset + p2] = (temp & bit_mask [p2]) != 0;
      }
    }
    return pmap_;
  }



  void BeginSqueuce(uint8_t* pmap,int32_t size)
  {
    uint8_t  data [MAX_PMAP_BYTES];
    int bytes = ParseBytes (data, MAX_PMAP_BYTES);
    parent_pmap_=current_pmap_;
    current_pmap_=pmap;
    parent_index_ = index_;
    index_=0;
    //current_pmap_=sqpmap_;
    memset(current_pmap_,0,size);
    if (bytes < 0)
      assert(0);
    for (int p1 = 0,  offset = 0 ; p1 < bytes ; p1 ++, offset += 7)
    {
      uint32_t temp = data [p1];
      for (int p2 = 0 ; p2 < 7 ; p2 ++)
      {
        assert(offset + p2<size);
        current_pmap_ [offset + p2] = (temp & bit_mask [p2]) != 0;
      }
    }
  }

  void EndSqueuce(uint8_t* pmap=NULL,int32_t index=0)
  {
    current_pmap_=pmap==NULL?pmap_:pmap;
    index_=index==0?parent_index_:index;
  }

  inline uint8_t ReadPmap()
  {
    return current_pmap_[index_++];
  }

  inline bool Empty()
  {
    return end_==tailer_;
  }


  int32_t DecodeInt32(int index,OperatorType operator_type,PresenceType presence_type=PT_OPTION)
  {
    int32_t temp=0;
    switch(operator_type)
    {
    case DEFAULT:
      {
        if(ReadPmap())
        {
          temp=ParseInt32(presence_type);
          return temp;
        }
        return WMDF_CONSTS_32BIT_NIL_VALUE;
      }
    case NONE:
      {
        temp=ParseInt32(presence_type);
        return temp;
      }
    case DELTA:
      temp=ParseInt32(presence_type);
      default_value_[index].int32_value += temp;
      return default_value_[index].int32_value;
    case COPY:
      if(ReadPmap()!=0)
        default_value_[index].int32_value = ParseInt32(presence_type);
      return default_value_[index].int32_value;
    case INCREASE:
      if(ReadPmap()!=0)
      {
        temp=ParseInt32(presence_type);
        default_value_[index].int32_value = temp;
      }else
      {
        default_value_[index].int32_value++;
      }
      return default_value_[index].int32_value;
    }
    assert(0);
    return WMDF_CONSTS_32BIT_NIL_VALUE;
  }

  uint32_t DecodeUInt32(int index,OperatorType operator_type )
  {
    switch(operator_type)
    {
    case DEFAULT:
      return ReadPmap()?ParseUInt32():0;
    case NONE:
      return ParseUInt32();
    case COPY:
      if(ReadPmap()!=0)
        default_value_[index].uint32_value = ParseUInt32();
      return default_value_[index].uint32_value;
    case INCREASE:
      if(ReadPmap()!=0)
      {
        default_value_[index].uint32_value = ParseUInt32();
      }else
      {
        default_value_[index].uint32_value++;
      }
      return default_value_[index].uint32_value;
    case DELTA:
      {
        uint32_t temp=ParseUInt32();
        default_value_[index].uint32_value += temp;
        return default_value_[index].uint32_value;
      }
    }
    assert(0);
    return 0;
  }

  int64_t DecodeInt64(int index,OperatorType operator_type,PresenceType presence_type=PT_OPTION )
  {
    switch(operator_type)
    {
    case DEFAULT:
      return ReadPmap()?ParseInt64(presence_type):WMDF_CONSTS_64BIT_NIL_VALUE;
    case NONE:
      return ParseInt64(presence_type);
    case COPY:
      if(ReadPmap()!=0)
        default_value_[index].int64_value = ParseInt64(presence_type);
      return default_value_[index].int64_value;
    case INCREASE:
      if(ReadPmap()!=0)
      {
        default_value_[index].int64_value = ParseInt64(presence_type);
      }else
      {
        default_value_[index].int64_value++;
      }
      return default_value_[index].int64_value;
    }
    assert(0);
    return WMDF_CONSTS_64BIT_NIL_VALUE;
  }

  uint64_t DecodeUInt64(int index,OperatorType operator_type )
  {
    switch(operator_type)
    {
    case DEFAULT:
      return ReadPmap()?ParseUInt64():0;
    case NONE:
      return ParseUInt64();
    case COPY:
      if(ReadPmap()!=0)
        default_value_[index].uint64_value = ParseUInt64();
      return default_value_[index].uint64_value;
    case INCREASE:
      if(ReadPmap()!=0)
      {
        default_value_[index].uint64_value = ParseUInt64();
      }else
      {
        default_value_[index].uint64_value++;
      }
      return default_value_[index].uint64_value;
    }
    assert(0);
    return 0;
  }

  inline uint8_t* GetDefaultStr(int index,size_t size)
  {
    if(default_value_[index].str_value==NULL){
      default_value_[index].str_value = new uint8_t[size];
      memset(default_value_[index].str_value,0,size);
    }
    return default_value_[index].str_value;
  }

  void DecodeString(int index,OperatorType operator_type,uint8_t*data,size_t size, char* default_value="" )
  {
    switch(operator_type)
    {
    case DEFAULT:
      if(ReadPmap()!=0)
        ParseASCIIString(data,size);
      else
        #pragma warning(disable : 4996)
        strcpy((char*)data,default_value);
        #pragma warning(default : 4996)

      break;
    case COPY:
      if(ReadPmap()==0){
        memcpy(data,GetDefaultStr(index,size),size);
      }else
      {
        ParseASCIIString(data,size);
        memcpy(GetDefaultStr(index,size),data,size);
      }
      break;
    case INCREASE:
      assert(0&&"string insert error");
      break;
    case NONE:
      ParseASCIIString(data,size);
      break;
    }
  }

  void DecodeUnicodeString(int index,OperatorType operator_type,uint8_t*data,size_t size )
  {
    switch(operator_type)
    {
    case DEFAULT:
      if(ReadPmap()!=0)
        ParserUnicodeString(data,size);
      break;
    case COPY:
      if(ReadPmap()==0){
        memcpy(data,GetDefaultStr(index,size),size);
      }else
      {
        ParserUnicodeString(data,size);
        memcpy(GetDefaultStr(index,size),data,size);
      }
      break;
    case INCREASE:
      assert(0);
      break;
    case NONE:
      ParserUnicodeString(data,size);
      break;
    }
  }
private:
  inline int ParseBytes(uint8_t* data,int size)
  {
    memset (data, 0, size);
    uint8_t temp;
    for (int i = 0 ; i < size ; i ++)
    {
      temp = *tailer_ ++;
      data [i] = temp;
      if (temp >= 0x80)
      {
        data [i] &= 0x7f;
        return i + 1;
      }
    }
    assert(0&&"Parse Bytes Error");
    return -1;
  }

  void ParseASCIIString(uint8_t* str,size_t str_len)
  {
    size_t temp=0;
    uint8_t* start = tailer_;
    while(*tailer_++ <0x80&&tailer_!=end_ )
      temp++;
    //null string
    if(start[0]==0)
    {
      if(str_len>0&&start[1]==0x80)
        memcpy(str,"",1);
    }
    if(start[0]==0x80)
    {
      memcpy(str,"",1);
    }
    if(str_len<=temp+1)  //while <= ?
      assert(0&&"return error");
    //empty string
    memcpy(str,start,temp+1);
    str[temp] =tailer_[-1]&0x7f;
    str[temp+1]='\0';
  }

  void ParserUnicodeString(uint8_t* str,size_t str_len)
  {
    uint32_t temp = ParseUInt32();
    if(temp>=str_len)
      assert(0&&"return error");
    memcpy(str,tailer_,temp);
    tailer_+=temp;
    str[temp+1]='\0';
  }

  int64_t ParseInt (int32_t sign_mask,int32_t type_mask )
  {
    uint8_t buffer[10];
    int32_t bytes;
    uint64_t temp;
    bytes = ParseBytes (buffer, type_mask);
    if (bytes < 0)
      return bytes;
    temp = 0 - ((buffer [0] & sign_mask) != 0);
    for (int32_t i = 0 ; i < bytes ; i++)
      temp = (temp << 7) | buffer [i];
    return temp;
  }

  inline uint32_t ParseUInt32 ()
  {
    return (uint32_t)ParseInt (SIGN_MASK_U32,TYPE_MASK_I32); }

  inline int32_t ParseInt32 (PresenceType presence_type=PT_OPTION)
  {
    int32_t temp = (int32_t)ParseInt ( SIGN_MASK_I32,TYPE_MASK_I32);

    if (presence_type == PT_OPTION)
    {
      if(temp>0)
        temp-=1;
      else if(temp == 0)
        temp = WMDF_CONSTS_32BIT_NIL_VALUE;
    }
    return temp;
  }

  inline uint64_t ParseUInt64 ()
  {
    return (uint64_t)ParseInt ( SIGN_MASK_U32,TYPE_MASK_I64); }

  inline int64_t ParseInt64 (PresenceType presence_type=PT_OPTION)
  {
    int64_t temp = ParseInt ( SIGN_MASK_I32,TYPE_MASK_I64);
    if(presence_type==PT_OPTION)
    {
      if(temp>0)
        temp-=1;
      else if(temp==0)
        temp = WMDF_CONSTS_64BIT_NIL_VALUE;
    }
    return temp;
  }
};
}

#endif
