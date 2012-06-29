/********************************************************
* This work is licensed under the Creative Commons      *
* Attribution-ShareAlike 3.0 Unported License.          *
* To view a copy of this license, visit                 *
* http://creativecommons.org/licenses/by-sa/3.0/ or     *
* send a letter to Creative Commons, 444 Castro Street, *
* Suite 900, Mountain View, California, 94041, USA.     *
*********************************************************/

// Written by James Hall and Matthew Stoltenberg

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
