/********************************************************
* This work is licensed under the Creative Commons      *
* Attribution-ShareAlike 3.0 Unported License.          *
* To view a copy of this license, visit                 *
* http://creativecommons.org/licenses/by-sa/3.0/ or     *
* send a letter to Creative Commons, 444 Castro Street, *
* Suite 900, Mountain View, California, 94041, USA.     *
*********************************************************/

// Written by James Hall and Matthew Stoltenberg

#include <atomic_ops.h>
#include <stdint.h>
#include <unistd.h>
#include <fstream>

#include <algorithm>
#include <ctime>

#include "BackoffLock.h"

using namespace std;

Backoff::Backoff(mt_gen &gen, uint32_t minDelay, uint32_t maxDelay)
    : gen_(gen), minDelay(minDelay), maxDelay(maxDelay)
{
    limit = min(minDelay, maxDelay);
}

void Backoff::backoff(void)
{
    distribution_type dist(minDelay, limit);
    uint32_t delay = dist(gen_);

    limit = min(maxDelay, 2 * limit);
    
    //struct timespec req;
    //req.tv_sec = 0;
    //req.tv_nsec = delay;
    //nanosleep(&req, &req);
    //cout << delay << endl;
    usleep(delay);
}

BackoffLock::BackoffLock(uint32_t minDelay, uint32_t maxDelay)
        : minDelay(minDelay), maxDelay(maxDelay) 
{
    uint32_t seed = 0x55AAFF00;
    gen.seed( seed );
}

void BackoffLock::lock(void)
{
    Backoff backoff(gen, minDelay, maxDelay);
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
