/********************************************************
* This work is licensed under the Creative Commons      *
* Attribution-ShareAlike 3.0 Unported License.          *
* To view a copy of this license, visit                 *
* http://creativecommons.org/licenses/by-sa/3.0/ or     *
* send a letter to Creative Commons, 444 Castro Street, *
* Suite 900, Mountain View, California, 94041, USA.     *
*********************************************************/

// Written by James Hall and Matthew Stoltenberg

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
