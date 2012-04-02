#ifndef __BACKOFFLOCK_H__
#define __BACKOFFLOCK_H__

/* PROBLEM:
 * following errors on cs1.utdallas.edu
BackoffLock.h:7:53: warning: boost/random/uniform_int_distribution.hpp: No such file or directory
BackoffLock.h:16: error: âmt19937â in namespace âboost::randomâ does not name a type
*/

#include <stdint.h>

#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>

#include "common.h"

class Backoff
{
    uint32_t minDelay;
    uint32_t maxDelay;
    uint32_t limit;
    boost::random::mt19937 gen;


public:
    Backoff(uint32_t minDelay, uint32_t maxDelay);
    void backoff();
};

class BackoffLock : public LOCK
{
    //state has two possible values:
    //  AO_TS_SET
    //  AO_TS_CLEAR
    volatile unsigned char state;
    uint32_t minDelay;
    uint32_t maxDelay;

public:
    BackoffLock(uint32_t minDelay, uint32_t maxDelay)
        : minDelay(minDelay), maxDelay(maxDelay) {}
    ~BackoffLock() {}
    void lock(void);
    void unlock(void);
};

#endif //__BACKOFFLOCK_H__
