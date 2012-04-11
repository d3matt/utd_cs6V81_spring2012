#ifndef __ALOCK_VOLATILE_H__
#define __ALOCK_VOLATILE_H__

#include <stdint.h>
#include <pthread.h>

#include "common.h"

class ALockVolatile : public LOCK
{
    volatile bool           *flag;
    uint64_t        tail;
    uint64_t        size;
    uint32_t        curslot;

public:
    ALockVolatile(uint32_t capacity);
    ~ALockVolatile();

    void lock(void);
    void unlock(void);
};

#endif //__ALOCK_VOLATILE_H__
