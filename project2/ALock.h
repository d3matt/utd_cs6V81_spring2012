#ifndef __ALOCK_H__
#define __ALOCK_H__

#include <stdint.h>
#include <pthread.h>

#include "common.h"

class ALockLocal;


class ALock : public LOCK
{
    bool           *flag;
    uint32_t        tail;
    uint32_t        size;
    pthread_key_t   ALockKey;

public:
    ALock(uint32_t capacity);
    ~ALock();

    void lock(void);
    void unlock(void);
    friend class ALockLocal;
};

class ALockLocal
{
public:
    ALock   *myLock;
    uint32_t index;

    void     clearKey(void);
};

void dataDestructor(void *data);

#endif //__ALOCK_H__
