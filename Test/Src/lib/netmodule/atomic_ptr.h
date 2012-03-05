
#ifndef WIND_NET_ATOMIC_PTR_
#define WIND_NET_ATOMIC_PTR_


#if defined ZMQ_FORCE_MUTEXES
#define ZMQ_ATOMIC_PTR_MUTEX
#elif (defined __i386__ || defined __x86_64__) && defined __GNUC__
#define ZMQ_ATOMIC_PTR_X86
#elif defined WIN32
#define ZMQ_ATOMIC_PTR_WINDOWS
#elif (defined ZMQ_HAVE_SOLARIS || defined ZMQ_HAVE_NETBSD)
#define ZMQ_ATOMIC_PTR_ATOMIC_H
#else
#define ZMQ_ATOMIC_PTR_MUTEX
#endif

#if defined ZMQ_ATOMIC_PTR_MUTEX
#include "mutex.hpp"
#elif defined ZMQ_ATOMIC_PTR_WINDOWS
#include <windows.h>
#elif defined ZMQ_ATOMIC_PTR_ATOMIC_H
#include <atomic.h>
#endif

namespace wmdf
{

    //  This class encapsulates several atomic operations on pointers.

    template <typename T> class atomic_ptr_t
    {
    public:

        //  Initialise atomic pointer
        inline atomic_ptr_t ()
        {
            ptr = NULL;
        }

        //  Destroy atomic pointer
        inline ~atomic_ptr_t ()
        {
        }

        //  Set value of atomic pointer in a non-threadsafe way
        //  Use this function only when you are sure that at most one
        //  thread is accessing the pointer at the moment.
        inline void set (T *ptr_)
        {
            this->ptr = ptr_;
        }

        //  Perform atomic 'exchange pointers' operation. Pointer is set
        //  to the 'val' value. Old value is returned.
        inline T *xchg (T *val_)
        {
#if defined ZMQ_ATOMIC_PTR_WINDOWS
            return (T*) InterlockedExchangePointer ((PVOID*) &ptr, val_);
#elif defined ZMQ_ATOMIC_PTR_ATOMIC_H
            return (T*) atomic_swap_ptr (&ptr, val_);
#elif defined ZMQ_ATOMIC_PTR_X86
            T *old;
            __asm__ volatile (
                "lock; xchg %0, %2"
                : "=r" (old), "=m" (ptr)
                : "m" (ptr), "0" (val_));
            return old;
#elif defined ZMQ_ATOMIC_PTR_MUTEX
            sync.lock ();
            T *old = (T*) ptr;
            ptr = val_;
            sync.unlock ();
            return old;
#else
#error atomic_ptr is not implemented for this platform
#endif
        }

        //  Perform atomic 'compare and swap' operation on the pointer.
        //  The pointer is compared to 'cmp' argument and if they are
        //  equal, its value is set to 'val'. Old value of the pointer
        //  is returned.
        inline T *cas (T *cmp_, T *val_)
        {
#if defined ZMQ_ATOMIC_PTR_WINDOWS
            return (T*) InterlockedCompareExchangePointer (
                (volatile PVOID*) &ptr, val_, cmp_);
#elif defined ZMQ_ATOMIC_PTR_ATOMIC_H
            return (T*) atomic_cas_ptr (&ptr, cmp_, val_);
#elif defined ZMQ_ATOMIC_PTR_X86
            T *old;
            __asm__ volatile (
                "lock; cmpxchg %2, %3"
                : "=a" (old), "=m" (ptr)
                : "r" (val_), "m" (ptr), "0" (cmp_)
                : "cc");
            return old;
#elif defined ZMQ_ATOMIC_PTR_MUTEX
            sync.lock ();
            T *old = (T*) ptr;
            if (ptr == cmp_)
                ptr = val_;
            sync.unlock ();
            return old;
#else
#error atomic_ptr is not implemented for this platform
#endif
        }

    private:

        volatile T *ptr;
#if defined ZMQ_ATOMIC_PTR_MUTEX
        mutex_t sync;
#endif

        atomic_ptr_t (const atomic_ptr_t&);
        const atomic_ptr_t &operator = (const atomic_ptr_t&);
    };

}

//  Remove macros local to this file.
#if defined ZMQ_ATOMIC_PTR_WINDOWS
#undef ZMQ_ATOMIC_PTR_WINDOWS
#endif
#if defined ZMQ_ATOMIC_PTR_ATOMIC_H
#undef ZMQ_ATOMIC_PTR_ATOMIC_H
#endif
#if defined ZMQ_ATOMIC_PTR_X86
#undef ZMQ_ATOMIC_PTR_X86
#endif
#if defined ZMQ_ATOMIC_PTR_MUTEX
#undef ZMQ_ATOMIC_PTR_MUTEX
#endif

#endif

