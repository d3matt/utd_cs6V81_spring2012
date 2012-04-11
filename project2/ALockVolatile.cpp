#include <pthread.h>
#include <unistd.h>

#include <cstdio>

#include "ALockVolatile.h"

//#define ALOCK_DEBUG
#ifdef  ALOCK_DEBUG
#define DBGDISP(format, args...) \
    printf("%s:%d " format "\n",__FILE__,__LINE__,  \
    ## args         \
    );
#else
    #define DBGDISP(format, args...)
#endif


ALockVolatile::ALockVolatile(uint32_t capacity) : tail(0), size(capacity)
{
    flag = new volatile bool[capacity];
    for( uint32_t i = 0 ; i < capacity; i++)
        flag[i] = false;
    flag[0] = true;

    DBGDISP("capacity: %u", capacity);
}

ALockVolatile::~ALockVolatile()
{
    volatile bool * tmp = flag;

    flag = 0;
    delete tmp;

}

void ALockVolatile::lock(void)
{
    //The below call is a gcc built-in
    //libatomic_ops caused a rollover in tail at 65535 (uint16_t)
    //this is a problem for any number of threads not a power of 2
    //This gcc builtin doesn't have that problem
    uint64_t tmp = __sync_fetch_and_add(&tail, 1);
    uint32_t slot = tmp % size;
    DBGDISP("lock() tmp: %lu slot: %u size: %u", tmp, slot, size);

    while( ! flag[slot])
    {
        //have to yield otherwise it locks up
        //pthread_yield();
    }

    flag[slot] = false;

    // At this point we are in the CS. We don't need thread local storage...

    curslot = slot;
}

void ALockVolatile::unlock(void)
{
    flag[ ( curslot + 1 ) % size ] = true;
}

