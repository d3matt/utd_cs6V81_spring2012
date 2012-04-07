#ifndef __ALOCK_H__
#define __ALOCK_H__

#include <stdint.h>
#include <pthread.h>

#include "common.h"

class ALock : public LOCK
{
    bool           *flag;
    uint32_t        tail;
    uint32_t        size;
    uint32_t        curslot;

public:
    ALock(uint32_t capacity);
    ~ALock();

    void lock(void);
    void unlock(void);
};

#endif //__ALOCK_H__
