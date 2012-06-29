/********************************************************
* This work is licensed under the Creative Commons      *
* Attribution-ShareAlike 3.0 Unported License.          *
* To view a copy of this license, visit                 *
* http://creativecommons.org/licenses/by-sa/3.0/ or     *
* send a letter to Creative Commons, 444 Castro Street, *
* Suite 900, Mountain View, California, 94041, USA.     *
*********************************************************/

// Written by James Hall and Matthew Stoltenberg

#include <iostream>
#include <cstdio>

#include <stdint.h>
#include <pthread.h>
#include <errno.h>

#include <time.h>

#include "common.h"


//#define SIMPLE_DEBUG
#ifdef  SIMPLE_DEBUG
#define DBGDISP(format, args...) \
    printf("%s:%d " format "\n",__FILE__,__LINE__,  \
    ## args         \
    );
#else
    #define DBGDISP(format, args...)
#endif

using namespace std;

bool ts_lt(timespec *a, timespec *b)
{
    if(a->tv_sec > b->tv_sec)
        return false;
    if(a->tv_sec < b->tv_sec)
        return true;
    if(a->tv_nsec < b->tv_nsec)
        return true;
    return false;
}

#define NSEC_SEC 1000000000

// returns the number of nanoseconds in the difference (x - y)
uint64_t ts_sub(const timespec *x, const timespec *y)
{
    int64_t retval;
    int64_t second;
    if(x->tv_nsec < y->tv_nsec)
    {
        retval = x->tv_nsec - y->tv_nsec + NSEC_SEC;
        second = x->tv_sec - y->tv_sec - 1;
    }
    else {
        retval = x->tv_nsec - y->tv_nsec;
        second = x->tv_sec - y->tv_sec;
    }

    retval += second * NSEC_SEC;
    
    if(retval < 0)
    {
        cout << "NEGATIVE VALUE!" << endl
             << "x: " << x->tv_sec << "." << x->tv_nsec << endl
             << "y: " << y->tv_sec << "." << y->tv_nsec << endl;
        return 0;
    }

    return (uint64_t) retval;
}

static uint64_t counter;
static uint64_t *tcounters;
static uint64_t *tnsecs;

static void *worker_thread(void *Arg)
{
    worker_thread_arg * arg = (worker_thread_arg *) Arg;
    uint32_t tnum = arg->tnum;
    
    timespec stop;
    timespec current;
    timespec lock1;
    timespec lock2;
    clock_gettime( CLOCK_REALTIME, &current );

    stop = arg->carg->start;
    stop.tv_sec += arg->carg->num_seconds;
    
    while( ts_lt(&current, &(arg->carg->start) ) )
    {
        //pthread_yield();
        clock_gettime( CLOCK_REALTIME, &current );
    }
    DBGDISP("start thread %2d : %ld.%ld", tnum, current.tv_sec, current.tv_nsec);
    lock2 = current;

    while( ts_lt(&lock2, &stop) ) {
        DBGDISP("thread %u lock()", tnum);
        clock_gettime( CLOCK_REALTIME, &lock1 );
        arg->lock->lock();
        clock_gettime( CLOCK_REALTIME, &lock2 );
        DBGDISP("thread %u back", tnum);
        counter++;

        DBGDISP("thread %u unlock()", tnum);
        arg->lock->unlock();
        DBGDISP("thread %u back", tnum);

        tcounters[tnum]++;
        tnsecs[tnum] += ts_sub(&lock2, &lock1);

        //clock_gettime( CLOCK_REALTIME, &current );
    }
    DBGDISP(" stop thread %2d : %ld.%ld %ld", tnum, lock2.tv_sec, lock2.tv_nsec, tcounters[tnum] );
    delete arg;
    return NULL;
}

int main(int argc, char ** argv)
{
    common_args carg;
    int         retval=0;

    parse_args(argc, argv, &carg);

    timespec start;
    timespec current;
    clock_gettime( CLOCK_REALTIME, &start );
    start.tv_sec += 1;

    tcounters = new uint64_t[carg.num_threads];
    tnsecs = new uint64_t[carg.num_threads];

    for(uint32_t i = 0; i < carg.num_threads; i++)
    {
        tcounters[i] = tnsecs[i] = 0;
    }

    carg.start=start;

    test_common(&carg, worker_thread);

    clock_gettime( CLOCK_REALTIME, &current );

    cout << "Test Start : " << start.tv_sec << "." << start.tv_nsec << endl;
    cout << " Test Stop : " << current.tv_sec << "." << current.tv_nsec << endl;


    cout << "counter is: " << counter << endl;
    uint64_t test=0;
    for(uint32_t i=0; i < carg.num_threads; i++)
    {
        test += tcounters[i];
    }
    if(test != counter)
        retval=1;
    test=0;
    for(uint32_t i=0; i < carg.num_threads; i++)
    {
        test += tnsecs[i];
    }
    cout << "total_nsec: " << test << endl;

    delete tcounters;
    return retval;
}


