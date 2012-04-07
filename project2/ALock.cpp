#include <atomic_ops.h>
#include <pthread.h>
#include <unistd.h>

#include <cstdio>

#include "ALock.h"

//#define ALOCK_DEBUG
#ifdef  ALOCK_DEBUG
#define DBGDISP(format, args...) \
    printf("%s:%d " format "\n",__FILE__,__LINE__,  \
    ## args         \
    );
#else
    #define DBGDISP(format, args...)
#endif


ALock::ALock(uint32_t capacity) : tail(0), size(capacity)
{
    flag = new bool[capacity];
    for( uint32_t i = 0 ; i < capacity; i++)
        flag[i] = false;
    flag[0] = true;

    DBGDISP("capacity: %u", capacity);
}

ALock::~ALock()
{
    bool * tmp = flag;

    flag = 0;
    delete tmp;

}

void ALock::lock(void)
{
    //went with atomic ops...
    uint32_t tmp = AO_int_fetch_and_add1(&tail);
    uint32_t slot = tmp % size;
    DBGDISP("lock() tmp: %u slot: %u size: %u", tmp, slot, size);

    while( ! flag[slot])
    {
        //have to yield otherwise it locks up
        pthread_yield();
    }

    // At this point we are in the CS. We don't need thread local storage...

    curslot = slot;
}

void ALock::unlock(void)
{
    flag[curslot] = false;
    flag[ ( curslot + 1 ) % size ] = true;
}

