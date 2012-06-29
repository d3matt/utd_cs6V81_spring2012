/********************************************************
* This work is licensed under the Creative Commons      *
* Attribution-ShareAlike 3.0 Unported License.          *
* To view a copy of this license, visit                 *
* http://creativecommons.org/licenses/by-sa/3.0/ or     *
* send a letter to Creative Commons, 444 Castro Street, *
* Suite 900, Mountain View, California, 94041, USA.     *
*********************************************************/

// Written by James Hall and Matthew Stoltenberg

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
