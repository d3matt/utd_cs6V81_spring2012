/********************************************************
* This work is licensed under the Creative Commons      *
* Attribution-ShareAlike 3.0 Unported License.          *
* To view a copy of this license, visit                 *
* http://creativecommons.org/licenses/by-sa/3.0/ or     *
* send a letter to Creative Commons, 444 Castro Street, *
* Suite 900, Mountain View, California, 94041, USA.     *
*********************************************************/

// Written by James Hall and Matthew Stoltenberg

#ifndef __ALOCK_H__
#define __ALOCK_H__

#include <stdint.h>
#include <pthread.h>

#include "common.h"

class ALock : public LOCK
{
    volatile bool  *flag;
    uint64_t        tail;
    uint64_t        size;
    uint32_t        curslot;

public:
    ALock(uint32_t capacity);
    ~ALock();

    void lock(void);
    void unlock(void);
};

#endif //__ALOCK_H__
