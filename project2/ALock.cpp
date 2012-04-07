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
    flag[0] = true;
    int rc = pthread_key_create(&ALockKey, dataDestructor);
    if(rc) {
        printf("Error creating key\n");
        _exit(1);
    }

    DBGDISP("capacity: %u", capacity);
}

ALock::~ALock()
{
    bool * tmp = flag;

    flag = 0;
    delete tmp;

    pthread_key_delete(ALockKey);
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

/*
The pthread_getspecific() function shall return the thread-specific data value associated with the given key. If no thread-specific data value is associated with key, then the value NULL shall be returned.

If successful, the pthread_setspecific() function shall return zero; otherwise, an error number shall be returned to indicate the error.
 */
    ALockLocal * ptr = (ALockLocal *)pthread_getspecific(ALockKey);
    DBGDISP("prthread_getspecific: %p", ptr);
    if( ptr == NULL)
    {
        ptr = new ALockLocal();
        ptr->myLock = this;
        pthread_setspecific(ALockKey, ptr);
    }
    ptr->index = slot;
    DBGDISP("ptr->index: %u", ptr->index);
}

void ALock::unlock(void)
{
    ALockLocal * ptr = (ALockLocal *) pthread_getspecific(ALockKey);

    DBGDISP("unlock() index: %u", ptr->index);
    flag[ptr->index] = false;
    flag[ ( ptr->index + 1 ) % size ] = true;
}

void ALockLocal::clearKey(void)
{
    pthread_setspecific(myLock->ALockKey, NULL);
}

void dataDestructor(void *data) {
    if(data) {
        ALockLocal * ptr = (ALockLocal *)data;
        ptr->clearKey();
        delete ptr;
    }
}
