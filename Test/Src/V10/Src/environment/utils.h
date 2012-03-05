#ifndef UTILS_H
#define UTILS_H

#include "serializer.h"
#include <vector>
#include <string>
#include "../include/wmdf_structs.h"
#include "../include/wmdf_api.h"
#ifdef __linux__
#include <cstdio>
#endif
using namespace std;

namespace wmdf
{
  #define WMDF_CONSTS_TEN                   10
  #define WMDF_CONSTS_ONE_HUNDRED           100
  #define WMDF_CONSTS_ONE_THOUSAND          1000
  #define WMDF_CONSTS_TEN_THOUSAND          10000
  #define WMDF_CONSTS_HUNDRED_THOUSAND      100000
  #define WMDF_CONSTS_ONE_MILLION           1000000

  //#define ReleaseMDFMessage(mdfrecord) (DestroyMDFMessage(&mdfrecord),mdfrecord=NULL)

  class MdfMessage;
  class Utils
  {
  public:
    Utils(void);
    virtual ~Utils(void);

    static Serializer* CreateSerializer(const string& file_name);
    static void Deserialize(const string& file_name,Persistable* persistable);
    static void Deserialize(const char*  xml,Persistable* persistable);
    static void Serialize(const string& file_name,Persistable* persistable);
    static std::string Serialize(Persistable* persistable);
    static void SleepTime(int millsecond);
    static void DestroyMDFRecord(MdfRecord*& mdfrecord); 
    static void DestroyLoginRequest(LoginRequest*& request);
    static void DestroyLoginResponse(LoginResponse*& response);
    static void DestroyClientStatus(ClientStatus*& client_status);

    static int8_t GetMajorVersion(int16_t product_id);
    static int8_t GetMinorVersion(int16_t product_id);

    static inline int32_t ConvertToInt32(int64_t x1);
    static inline int32_t Multiply(int32_t x1, int32_t x2);
    static inline int64_t Multiply(int64_t x1, int64_t x2);
    static inline double Multiply(double x1, double x2);

    static inline int32_t DivideEx(uint32_t x1, uint32_t x2);
    static inline int32_t DivideEx(int32_t x1, int32_t x2);
    static inline int64_t DivideEx(uint64_t x1, uint32_t x2);
    static inline int64_t DivideEx(int64_t x1, int32_t x2);
    static inline int32_t DivideEx2(int64_t x1, int32_t x2);

    static inline double Divide(uint32_t x1, int32_t x2);
    static inline double Divide(int32_t x1, int32_t x2);
    static inline double Divide(uint64_t x1, int32_t x2);
    static inline double Divide(int64_t x1, int32_t x2);
    static inline double Divide(double x1, double x2);

  private:
    static void DestroySystemMessage(MdfRecord*& mdfrecord);
    static void DestroyMonitorMessage(MdfRecord*& mdfrecord);
    static void DestroyMarketMessage(MdfRecord*& mdfrecord);
    
  };


  double wmdf::Utils::Divide( uint32_t x1, int32_t x2 )
  {
    if(x2 == WMDF_CONSTS_32BIT_NIL_VALUE)
      return WMDF_CONSTS_DOUBLE_NIL_VALUE;
    else
      return (double)x1/x2;
  }

  double wmdf::Utils::Divide( int32_t x1, int32_t x2 )
  {
    if(x1 == WMDF_CONSTS_32BIT_NIL_VALUE||x2 == WMDF_CONSTS_32BIT_NIL_VALUE)
      return WMDF_CONSTS_DOUBLE_NIL_VALUE;
    else
      return (double)x1/x2;
  }

  double wmdf::Utils::Divide( uint64_t x1, int32_t x2 )
  {
    if(x2 == WMDF_CONSTS_32BIT_NIL_VALUE)
      return WMDF_CONSTS_DOUBLE_NIL_VALUE;
    else
      return (double)x1/x2;
  }

  double wmdf::Utils::Divide( int64_t x1, int32_t x2 )
  {
    if(x1 == WMDF_CONSTS_64BIT_NIL_VALUE||x2 == WMDF_CONSTS_32BIT_NIL_VALUE)
      return WMDF_CONSTS_DOUBLE_NIL_VALUE;
    else
      return (double)x1/x2;
  }



  int32_t wmdf::Utils::DivideEx( uint32_t x1, uint32_t x2 )
  {
    if(x2 == WMDF_CONSTS_32BIT_NIL_VALUE)
      return WMDF_CONSTS_32BIT_NIL_VALUE;
    else
      return x1/x2;
  }

  int32_t wmdf::Utils::DivideEx( int32_t x1, int32_t x2 )
  {
    if(x1 == WMDF_CONSTS_32BIT_NIL_VALUE||x2 == WMDF_CONSTS_32BIT_NIL_VALUE)
      return WMDF_CONSTS_32BIT_NIL_VALUE;
    else
      return x1/x2;
  }

  int64_t wmdf::Utils::DivideEx( uint64_t x1, uint32_t x2 )
  {
    if(x2 == WMDF_CONSTS_32BIT_NIL_VALUE)
      return WMDF_CONSTS_64BIT_NIL_VALUE;
    else
      return x1/x2;
  }

  int64_t wmdf::Utils::DivideEx( int64_t x1, int32_t x2 )
  {
    if(x1 == WMDF_CONSTS_64BIT_NIL_VALUE||x2 == WMDF_CONSTS_32BIT_NIL_VALUE)
      return WMDF_CONSTS_64BIT_NIL_VALUE;
    else
      return x1/x2;
  }

  int32_t wmdf::Utils::DivideEx2( int64_t x1, int32_t x2 )
  {
    if(x1 == WMDF_CONSTS_64BIT_NIL_VALUE||x2 == WMDF_CONSTS_32BIT_NIL_VALUE)
      return WMDF_CONSTS_32BIT_NIL_VALUE;
    else
      return (int32_t)(x1/x2);
  }


  double wmdf::Utils::Divide( double x1, double x2 )
  {
    if(x1 != x1||x2 != x2)
      return WMDF_CONSTS_DOUBLE_NIL_VALUE;
    else
      return x1/x2;
  }

  int32_t wmdf::Utils::Multiply( int32_t x1, int32_t x2 )
  {
    if(x1 == WMDF_CONSTS_32BIT_NIL_VALUE||x2 == WMDF_CONSTS_32BIT_NIL_VALUE)
      return WMDF_CONSTS_32BIT_NIL_VALUE;
    else
      return x1*x2;
  }

  int64_t wmdf::Utils::Multiply( int64_t x1, int64_t x2 )
  {
    if(x1 == WMDF_CONSTS_64BIT_NIL_VALUE||x2 == WMDF_CONSTS_64BIT_NIL_VALUE)
      return WMDF_CONSTS_64BIT_NIL_VALUE;
    else
      return x1*x2;
  }

  double wmdf::Utils::Multiply( double x1, double x2 )
  {
    if(x1 == WMDF_CONSTS_32BIT_NIL_VALUE||x2 == WMDF_CONSTS_32BIT_NIL_VALUE)
      return WMDF_CONSTS_32BIT_NIL_VALUE;
    else
      return x1*x2;
  }


  int32_t wmdf::Utils::ConvertToInt32( int64_t x1 )
  {
    if(x1==WMDF_CONSTS_64BIT_NIL_VALUE)
      return WMDF_CONSTS_32BIT_NIL_VALUE;
    else return (int32_t)x1;
  }

}

#endif
