
#ifndef lclientapi_h
#define lclientapi_h

#ifdef __cplusplus
extern "C" {
#endif

#ifndef GAVA_API
#ifdef _WIN32
#   
#   if defined DLL_EXPORT
#       define GAVA_API __declspec(dllexport)
#   else
#       define GAVA_API __declspec(dllimport)
#   endif
#else
#   if defined __SUNPRO_C  || defined __SUNPRO_CC
#       define GAVA_API __global
#   elif (defined __GNUC__ && __GNUC__ >= 4) || defined __INTEL_COMPILER
#       define GAVA_API __attribute__ ((visibility("default")))
#   else
#       define GAVA_API
#   endif
#endif
#else 
#define GAVA_API
#endif


#if ((defined _MSC_VER &&_MSC_VER>1500)||(defined __GNUC__ && __GNUC__ >= 4))
#include <stdint.h>
#include <stddef.h>
#else
#ifndef int8_t
  typedef char int8_t;
#endif
#ifndef int16_t
  typedef short int16_t;
#endif
#ifndef int32_t
  typedef int int32_t;
#endif
#ifndef int64_t
  typedef long long int64_t;
#endif
#ifndef uint8_t
  typedef unsigned char uint8_t;
#endif
#ifndef uint16_t
  typedef unsigned short uint16_t;
#endif
#ifndef uint32_t
  typedef unsigned int uint32_t;
#endif
#ifndef uint64_t
  typedef unsigned long long uint64_t;
#endif
#endif

typedef struct Value0
{
  int32_t type;       //用于标记当前结构对应于Gava的数据类型
  void* owner;
  union{
    uint8_t u8;
    int32_t i32;
    int64_t i64;
    double  d;
    char* s;
    struct Value0* next;//用于检测回环
    struct
    {
      int32_t n;
      uint8_t data[1];
    };
  };
}*GValue;

#define gG(x)               ((x)->data)
#define gC(x)               gG(x)
#define gI(x)               ((int32_t*)gG(x))
#define gJ(x)               ((int64_t*)gG(x))
#define gE(x)               ((double*)gG(x))
#define gS(x)               ((char**)gG(x))
#define gL(x)               ((GValue*)gG(x))

#ifndef luai_NI
#define luai_NI              ((int)0x80000000)
#ifdef _WIN32
  #define luai_NL            ((long long)0x8000000000000000)
  #define luai_NN            (log(-1.0))
  extern double log(double);
#else 
  #define luai_NL            (0x8000000000000000LL)
  #define luai_NN            (0/0.0)
#endif

#define luai_NS             NULL
#define isNaI(n)            ((n) == luai_NI)
#define isNaL(n)            ((n) == luai_NL)
#define isNaN(n)            ((n) != (n))
#define isNaS(n)            ((n) == luai_NS)
#define isNaZ(n)            isNaN(n)
#define isNaD(n)            isNaI(n)
#define isNaT(n)            isNaI(n)
#define isNaU(n)            isNaI(n)
#define isNaV(n)            isNaI(n)
#define isNaM(n)            isNaI(n)

#endif //end of luai_NI

#ifndef LUA_TYPE
#define LUA_TYPE

#define LUA_TNONE		(-1)

#define LUA_TTYPEMASK	0xFFFF00FF

#define LUA_TNIL			0x0000
#define LUA_TLIGHTUSERDATA	0x0002

#define LUA_TBOOLEAN		0x0003
#define LUA_TBIT			  LUA_TBOOLEAN
#define LUA_TINT			  0x0004
#define LUA_TLONG			  0x0005
#define LUA_TNUMBER			0x0006
#define LUA_TSTRING			0x0007
#define LUA_TTABLE			0x0008
#define LUA_TVECTOR			0x0009
#define LUA_TGTABLE			0x000A
#define LUA_TBLOB       0x000B
#define LUA_TFUNCTION		0x000C
#define LUA_TUSERDATA		0x000D
#define LUA_TTHREAD			0x000E
#define LUA_NUMTAGS			0x000F
#define LUA_LIST        0x0010
#define LUA_TERROR      0x0011

#define LUA_TTYPE(subtype, basictype) ((subtype)|(basictype))
#define LUA_TDATETIME		LUA_TTYPE(0x0100, LUA_TNUMBER)
#define LUA_TDATE			  LUA_TTYPE(0x0100, LUA_TINT)
#define LUA_TMONTH			LUA_TTYPE(0x0200, LUA_TINT)
#define LUA_TMINUTE			LUA_TTYPE(0x0400, LUA_TINT)
#define LUA_TSECOND			LUA_TTYPE(0x0500, LUA_TINT)
#define LUA_TTIMESTAMP	LUA_TTYPE(0x0100, LUA_TLONG)
#define LUA_TTIME			  LUA_TTYPE(0x0300, LUA_TINT)
#define LUA_TTIMESPAN		LUA_TTYPE(0x0200, LUA_TLONG)

#endif //end of LUA_TYPE

/*
  * 说明： 连接Gava服务器。
  * 参数：
    * ipaddress 服务器IP地址。
    * port      服务器端口号。
  * 返回：
    * 如果建立连接成功，返回连接句柄；否则 返回-1。
*/
GAVA_API int32_t hopen(const char* ipaddress,int port);

/*
  * 说明： 关闭与Gava建立的连接。
  * 参数：
    * sock_id 连接句柄。该句柄必须是通过hopen建立连接时返回的句柄。
  * 返回：
    * 始终返回 1.
*/
GAVA_API int32_t hclose(int32_t sock_id);

/*
  * 说明： 以同步的方式，请求Gava服务器执行脚本命令。
  * 参数：
    * sock_id 通过hopen建立连接时返回的有效句柄。
    * script Gava脚本。
    * ... 附加参数，如果传递多个参数，x为第一个参数，附加参数之后必须再添加一个NULL参数。
    * 返回：
    * 如果脚本为函数调用，则返回函数调用返回的结果。
  * 备注：
    * 调用者将被阻塞。
*/
GAVA_API GValue hexec(int32_t sock_id, const char* script, ...);

/*
* 以异步的方式请求Gava服务器执行脚本命令时，注册的回调函数。
*/
typedef void callback (GValue value);

/*
  * 说明： 以异步的方式，请求Gava服务器执行脚本命令。
  * 参数：
    * sock_id 通过hopen建立连接时返回的有效句柄。
    * func 当请求执行完毕异步返回时，回调该函数。
    * script Gava脚本。
    * ... 附加参数，如果传递多个参数，x为第一个参数，附加参数之后必须再添加一个NULL参数。
  * 返回：
    * 如果调用失败，返回-1；否则返回0。
*/
GAVA_API int32_t hasynccall(int32_t sock_id, callback func, const char* script, ...);

/*
  * 说明：将int32_t类型转换为类型为int的GValue.
*/
GAVA_API GValue gint(int32_t);

/*
  * 说明：将int32_t类型转换为类型为time的GValue.
*/
GAVA_API GValue gttime(int32_t);

/*
  * 说明：将int32_t类型转换为类型为minute的GValue.
*/
GAVA_API GValue gtminute(int32_t);

/*
  * 说明：将int32_t类型转换为类型为second的GValue.
*/
GAVA_API GValue gtsecond(int32_t);

/*
  * 说明：将int32_t类型转换为类型为month的GValue.
*/
GAVA_API GValue gtmonth(int32_t);

/*
  * 说明：将int32_t类型转换为类型为date的GValue.
*/
GAVA_API GValue gtdate(int32_t);

/*
  * 说明：将double类型转换为类型为datetime的GValue.
*/
GAVA_API GValue gtdatetime(double);

/*
  * 说明：将blob类型装入GValue
*/
GAVA_API GValue gblob(const char*,size_t);

/*
  * 说明：将0结束符的字符串类型转换为类型为string的GValue.
*/
GAVA_API GValue gstring(const char*);

/*
  * 说明：将0结束符的字符串类型转换为内部的字符串.
*/
GAVA_API char* ss(char*);

/*
  * 说明：将int64_t类型转换为类型为long的GValue.
*/
GAVA_API GValue glong(int64_t);

/*
  * 说明：将double类型转换为类型为number的GValue.
*/
GAVA_API GValue gdouble(double);

/*
  * 说明：创建类型为type，元素个数为length的向量，即类型为vector的GValue.
  * 参数：
    * type 类型值。该类型值必须是有效的LUA_TYPE
    * length 向量中元素的个数
  * 返回：类型为vector的GValue.
*/
GAVA_API GValue gvector(int16_t type,int32_t length);

/*
  * 说明：将参数打包为list。
  * 参数：
    * count 需要打包为list的GValue的数目
    * ... GValue
  * 返回：
    * 类型为list的GValue
*/
GAVA_API GValue glist(int32_t count,...);

/*
  * 说明：释放任何不再使用的GValue对象。包括由gint,glong返回的，hexec返回的，以及异步回调函数传入的。
*/
GAVA_API void grelease(GValue value);

GAVA_API int gcount(GValue value);
GAVA_API int gencode(GValue value,char* data,int* len);

/*
  * 说明：获取内部表示小时分钟的整数值
  * 参数：
    * hour 小时部分，有效值区间为[-99, +99]
    * minute 分钟部分，有效值区间为[0, 59]
*/
GAVA_API int gminute(int hour,int minute);

/*
  * 说明：获取内部表示小时分钟秒的整数值
  * 参数：
    * hour 小时部分，有效值区间为[-99, +99]
    * minute 分钟部分，有效值区间为[0, 59]
    * second 秒部分，有效区间为[0, 59]
*/
GAVA_API int gsecond(int hour,int minute,int second);

/*
  * 说明：获取内部表示小时分钟秒毫秒的整数值
  * 参数：
    * hour 小时部分，有效值区间为[-99, +99]
    * minute 分钟部分，有效值区间为[0, 59]
    * second 秒部分，有效区间为[0, 59]
    * ms 秒部分，有效区间为[0, 999]
*/
GAVA_API int gtime(int hour,int minute,int second,int ms);

/*
  * 说明：获取内部表示年月的整数值
  * 参数：
    * year 年部分，有效值区间为[1000, 9999]
    * month 月部分，有效值区间为[1, 12]
*/
GAVA_API int gmonth(int year,int month);

/*
  * 说明：获取内部表示日期的整数值
  * 参数：
    * year 与lmonth同。
    * month 与lmonth同
    * day 日部分，有效值区间为[1, 31]，（注意，上界与所在年月有关）
*/
GAVA_API int gdate(int year,int month,int day);

/*
  * 说明：获取内部表示日期时间的浮点数值
  * 参数：
    * year 与ldate同。
    * month 与ldate同。
    * day 与ldate同。
    * hour 与ltime同。
    * minute 与ltime同。
    * second 与ltime同。
    * ms 与ltime同。
*/
GAVA_API double gdatetime(int year,int month,int day,int hour,int minute,int second,int ms);

/*
  * 说明：返回日期对应星期几。
  * 参数：
    * year 与ldate同。
    * month 与ldate同。
    * day 与ldate同。
*/
GAVA_API int gweek(int year,int month,int day);

/*
  * 说明：返回内部表示的当前月份。
*/
GAVA_API int gcurrentmonth();

/*
  * 说明：返回内部表示的当前日期。
*/
GAVA_API int gcurrentdate();

/*
  * 说明：返回内部表示的当前小时分钟。
*/
GAVA_API int gcurrentminute();

/*
  * 说明：返回内部表示的当前小时分钟秒。
*/
GAVA_API int gcurrentsecond();

/*
  * 说明：返回内部表示的当前时间(小时分钟秒毫秒)。
*/
GAVA_API int gcurrenttime();

/*
  * 说明：返回内部表示的当前日期时间。
*/
GAVA_API double gcurrentdatetime();

/*
  * 说明：返回内部表示的当前月份。
*/
GAVA_API long long gtimestamp();

/*
  * 说明: 将日期内部表示转换为年、月、日的整数
  * 参数：
    * d 以内部表示的日期
    * year 返回日期的年
    * month 返回日期的月
    * day 返回日期的日（月份的第几天）
  * 返回：如果失败返回0，否则返回1
*/
GAVA_API int gdate2ymd(int d, int *year, int *month, int *day);

/*
  * 说明: 将年月的内部表示转换为年、月的整数
  * 参数：
    * m 以内部表示的日期
    * year 返回年月的年份
    * month 返回年月的月份
  * 返回：如果转换失败返回0，否则返回1
*/
GAVA_API int gmonth2ym(int m, int *year, int *month);

/*
  * 说明: 将时分类型的内部表示转换为小时数和分钟数整数
  * 参数：
    * m 以内部表示的分钟
    * hour 返回时分中的小时数
    * minute 返回时分中的分钟数
  * 返回：如果转换失败返回0，否则返回1
*/
GAVA_API int gminute2hm(int m, int *hour, int *minute);

/*
  * 说明: 将时分秒类型的内部表示转换为时、分、秒的整数
  * 参数：
    * m 以内部表示的分钟
    * hour 返回时分秒中的小时数
    * minute 返回时分秒中的分钟数
    * second 返回时分秒中的秒数
  * 返回：如果转换失败返回0，否则返回1
*/
GAVA_API int gsecond2hms(int s, int *hour, int *minute, int *second);

/*
  * 说明: 将时间类型的内部表示转换为时、分、秒、毫秒的整数
  * 参数：
    * m 以内部表示的分钟
    * hour 返回时间中的小时数
    * minute 返回时间中的分钟数
    * second 返回时间中的秒数
    * msecond 返回时间中的毫秒数
  * 返回：如果转换失败返回0，否则返回1
*/
GAVA_API int gtime2hmsm(int t, int *hour, int *minute, int *second, int *msecond);

/*
  * 说明: 将日期时间类型的内部表示转换为年、月、日、时、分、秒、毫秒的整数
  * 参数：
    * m 以内部表示的分钟
    * year 返回日期时间中的年份
    * month 返回日期时间中的月份
    * day 返回日期时间中的日（月份的第几天）
    * hour 返回日期时间中的小时数
    * minute 返回日期时间中的分钟数
    * second 返回日期时间中的秒数
    * msecond 返回日期时间中的毫秒数
  * 返回：如果转换失败返回0，否则返回1
*/
GAVA_API int gdatetime2ymdhmsm(double dt,  int *year, int *month, int *day, 
  int *hour, int *minute, int *second, int *msecond);

#ifdef __cplusplus
}
#endif
#endif