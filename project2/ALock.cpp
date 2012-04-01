#include <cstdio>

#include <atomic_ops.h>

#include "ALock.h"

#define ALOCK_DEBUG
#ifdef  ALOCK_DEBUG
#define DBGDISP(format, args...) \
    printf("%s:%d " format "\n",__FILE__,__LINE__,  \
    ## args         \
    );
#else
    #define DBGDISP(format, args...)
#endif

ALock::ALock(uint32_t capacity) :
tail(0),
size(capacity)
{
    flag = new bool[capacity];
    flag[0] = true;
    DBGDISP("capacity: %u", capacity);
}

ALock::~ALock()
{
    bool * tmp = flag;

    //FIXME: this will probably still break :)
    flag = 0;
    delete tmp;
}

void ALock::lock(void)
{

    //went with atomic ops...
    uint32_t tmp = AO_int_fetch_and_add1(&tail);
    uint32_t slot = tmp % size;
    DBGDISP("lock() tmp: %u slot: %u size: %u", tmp, slot, size);


    while( ! flag[slot]) {}

    //at this point, we have the lock so no need for the overhead of Thread Local Storage
    current_slot = slot;
}

void ALock::unlock(void)
{
    DBGDISP("unlock() slot: %u", current_slot);
    flag[current_slot] = false;
    flag[ ( current_slot + 1 ) % size ] = true;
}
