/********************************************************
* This work is licensed under the Creative Commons      *
* Attribution-ShareAlike 3.0 Unported License.          *
* To view a copy of this license, visit                 *
* http://creativecommons.org/licenses/by-sa/3.0/ or     *
* send a letter to Creative Commons, 444 Castro Street, *
* Suite 900, Mountain View, California, 94041, USA.     *
*********************************************************/

// Written by James Hall and Matthew Stoltenberg

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
            //DBGDISP("yielding %lu", syscall(__NR_gettid) );
#ifdef YIELD
            pthread_yield();
#endif /*YIELD*/
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
