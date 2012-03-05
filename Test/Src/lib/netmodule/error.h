#ifndef NET_ERROR_H_
#define NET_ERROR_H_

#if defined __GNUC__
#define likely(x) __builtin_expect ((x), 1)
#define unlikely(x) __builtin_expect ((x), 0)
#else
#define likely(x) (x)
#define unlikely(x) (x)
#endif



#define alloc_assert(x) \
  do {\
  if (unlikely (!x)) {\
  fprintf (stderr, "FATAL ERROR: OUT OF MEMORY (%s:%d)\n",\
  __FILE__, __LINE__);\
  abort ();\
  }\
  } while (false)


#endif
