#include <atomic_ops.h>
#include <stdint.h>
#include <unistd.h>
#include <fstream>

#include <algorithm>
#include <ctime>

#include "BackoffLock.h"

using namespace std;

Backoff::Backoff(uint32_t minDelay, uint32_t maxDelay)
    : minDelay(minDelay), maxDelay(maxDelay), limit(maxDelay)
{
    ifstream randseed("/dev/urandom", ios::binary);
    uint32_t seed;
    randseed.read((char*)&seed, 4);
    gen.seed( seed );
}

void Backoff::backoff(void)
{
    distribution_type dist(1, limit);
    uint32_t delay = dist(gen);

    limit = min(maxDelay, 2 * limit);
    usleep(delay);
}



void BackoffLock::lock(void)
{
    Backoff backoff(minDelay, maxDelay);
    while(1)
    {
        do
        {
            if( AO_test_and_set(&state) != AO_TS_SET) {
                return;
            }
            else {
                backoff.backoff();
            }
        } while( state == AO_TS_SET);
    }
}

void BackoffLock::unlock(void)
{
    AO_CLEAR(&state);
}
