#ifndef __ALOCK_YIELD_H__
#define __ALOCK_YIELD_H__

#include <stdint.h>
#include <pthread.h>

#include "common.h"

class ALockYield : public LOCK
{
    bool           *flag;
    uint64_t        tail;
    uint64_t        size;
    uint32_t        curslot;

public:
    ALockYield(uint32_t capacity);
    ~ALockYield();

    void lock(void);
    void unlock(void);
};

#endif //__ALOCK_YIELD_H__
