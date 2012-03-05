#ifndef NET_STDINT_
#define NET_STDINT_


#if defined ZMQ_HAVE_SOLARIS || defined ZMQ_HAVE_OPENVMS

#include <inttypes.h>

#elif (defined _MSC_VER&&_MSC_VER<= 1500) 

#ifndef int8_t
typedef __int8 int8_t;
#endif
#ifndef int16_t
typedef __int16 int16_t;
#endif
#ifndef int32_t
typedef __int32 int32_t;
#endif
#ifndef int64_t
typedef __int64 int64_t;
#endif
#ifndef uint8_t
typedef unsigned __int8 uint8_t;
#endif
#ifndef uint16_t
typedef unsigned __int16 uint16_t;
#endif
#ifndef uint32_t
typedef unsigned __int32 uint32_t;
#endif
#ifndef uint64_t
typedef unsigned __int64 uint64_t;
#endif

#else

#include <stdint.h>

#endif

#endif

