#include <cstdio>

#include <atomic_ops.h>
#include <pthread.h>

#include "TASLock.h"

//#define TASLOCK_DEBUG
#ifdef  TASLOCK_DEBUG
#define DBGDISP(format, args...) \
    printf("%s:%d " format "\n",__FILE__,__LINE__,  \
    ## args         \
    );
#else
    #define DBGDISP(format, args...)
#endif

TASLock::TASLock()
{
    AO_CLEAR(&state);
}

void TASLock::lock()
{
    while( AO_test_and_set(&state) == AO_TS_SET)
    {
        pthread_yield();
    }
}

void TASLock::unlock()
{
    AO_CLEAR(&state);
}
