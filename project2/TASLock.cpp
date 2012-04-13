#include <cstdio>

#include <atomic_ops.h>
#include <atomic_ops.h>
#include <pthread.h>

#include <cstdio>

#include "TASLock.h"

TASLock::TASLock()
{
    AO_CLEAR(&state);
}

void TASLock::lock()
{
    while( AO_test_and_set(&state) == AO_TS_SET)
    {
#ifdef YIELD
        pthread_yield();
#endif /*YIELD*/
    }
}

void TASLock::unlock()
{
    AO_CLEAR(&state);
}
