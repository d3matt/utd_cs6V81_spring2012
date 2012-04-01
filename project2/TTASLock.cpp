#include <atomic_ops.h>
#include <pthread.h>
#include <sys/syscall.h>
#include <unistd.h>

#include <cstdio>

#include "TTASLock.h"

//#define TTASLOCK_DEBUG
#ifdef  TTASLOCK_DEBUG
#define DBGDISP(format, args...) \
    printf("%s:%d " format "\n",__FILE__,__LINE__,  \
    ## args         \
    );
#else
    #define DBGDISP(format, args...)
#endif


TTASLock::TTASLock()
{
    AO_CLEAR(&state);
}

void TTASLock::lock()
{
    while(1)
    {
        while( state == AO_TS_SET)
        {
            DBGDISP("yielding %lu", syscall(__NR_gettid) );
            pthread_yield();
        }
        if( AO_test_and_set(&state) != AO_TS_SET) {
            return;
        }
    }
}

void TTASLock::unlock()
{
    AO_CLEAR(&state);
}
