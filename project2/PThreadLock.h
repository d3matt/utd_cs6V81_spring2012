#ifndef __PTHREADLOCK_H__
#define __PTHREADLOCK_H__

#include "common.h"

class PThreadLock : public LOCK
{
    pthread_mutex_t mutex;

public:
    PThreadLock();
    ~PThreadLock() {};
    void lock(void);
    void unlock(void);
};

#endif //__PThreadLOCK_H__
