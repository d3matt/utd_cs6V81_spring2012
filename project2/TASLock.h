#ifndef __TASLOCK_H__
#define __TASLOCK_H__

#include "common.h"

class TASLock : public LOCK
{
    //state has two possible values:
    //  AO_TS_SET
    //  AO_TS_CLEAR
    volatile unsigned char state;

public:
    TASLock();
    ~TASLock() {};
    void lock(void);
    void unlock(void);
};

#endif //__TASLOCK_H__