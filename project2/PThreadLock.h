/********************************************************
* This work is licensed under the Creative Commons      *
* Attribution-ShareAlike 3.0 Unported License.          *
* To view a copy of this license, visit                 *
* http://creativecommons.org/licenses/by-sa/3.0/ or     *
* send a letter to Creative Commons, 444 Castro Street, *
* Suite 900, Mountain View, California, 94041, USA.     *
*********************************************************/

// Written by James Hall and Matthew Stoltenberg

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
