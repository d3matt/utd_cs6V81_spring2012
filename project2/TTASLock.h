#ifndef __TTASLOCK_H__
#define __TTASLOCK_H__

#include "common.h"

class TTASLock : public LOCK
{
    //state has two possible values:
    //  AO_TS_SET
    //  AO_TS_CLEAR
    volatile unsigned char state;

public:
    TTASLock();
    ~TTASLock() {};
    void lock(void);
    void unlock(void);
};

#endif //__TTASLOCK_H__
