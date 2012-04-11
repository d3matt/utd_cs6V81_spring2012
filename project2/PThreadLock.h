#ifndef __PTHREADLOCK_H__
#define __PTHREADLOCK_H__

#include "common.h"

class PThreadLock : public LOCK
{
    //state has two possible values:
    //  AO_TS_SET
    //  AO_TS_CLEAR
    pthread_mutex_t mutex;

public:
    PThreadLock();
    ~PThreadLock() {};
    void lock(void);
    void unlock(void);
};

#endif //__PThreadLOCK_H__
