#include <atomic_ops.h>
#include <stdint.h>
#include <unistd.h>

#include <algorithm>
#include <ctime>

#include "BackoffLock.h"

using namespace std;

Backoff::Backoff(uint32_t minDelay, uint32_t maxDelay)
    : minDelay(minDelay), maxDelay(maxDelay), limit(maxDelay)
{
    gen.seed( std::time(0) );
}

void Backoff::backoff(void)
{
    boost::random::uniform_int_distribution<> dist(1, limit);
    uint32_t delay = dist(gen);

    limit = min(maxDelay, 2 * limit);
    usleep(delay * 1000);
}



void BackoffLock::lock(void)
{
    Backoff backoff(minDelay, maxDelay);
    while(1)
    {
        while( state == AO_TS_SET)
        {
            if( AO_test_and_set(&state) != AO_TS_SET) {
                return;
            }
            else {
                backoff.backoff();
            }
        }
    }
}

void BackoffLock::unlock(void)
{
    AO_CLEAR(&state);
}
