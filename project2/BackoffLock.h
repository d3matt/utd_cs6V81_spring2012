#ifndef __BACKOFFLOCK_H__
#define __BACKOFFLOCK_H__

#include <stdint.h>

#include <boost/version.hpp>

#if BOOST_VERSION == 103301

#include <boost/random/uniform_int.hpp>
#include <boost/random/mersenne_twister.hpp>

typedef boost::uniform_int<> distribution_type;
typedef boost::mt19937 mt_gen;

#else

#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>

typedef boost::random::uniform_int_distribution<> distribution_type;
typedef boost::random::mt19937 mt_gen;

#endif


#include "common.h"

class Backoff
{
    uint32_t minDelay;
    uint32_t maxDelay;
    uint32_t limit;
    mt_gen   gen;


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
    BackoffLock(uint32_t minDelay, uint32_t maxDelay);
    ~BackoffLock() {}
    void lock(void);
    void unlock(void);
};

#endif //__BACKOFFLOCK_H__
