#include <iostream>
#include <cstdio>

#include <stdint.h>
#include <pthread.h>
#include <errno.h>

#include <time.h>

#include "common.h"


//#define SIMPLE_DEBUG
#ifdef  PROJ_DEBUG
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

static uint64_t counter;
static uint64_t *tcounters;

static void *worker_thread(void *Arg)
{
    worker_thread_arg * arg = (worker_thread_arg *) Arg;
    uint32_t tnum = arg->tnum;
    
    //not sure if each thread should run for 10 seconds or overall we should run for 10 seconds...
    timespec stop;
    timespec current;
    clock_gettime( CLOCK_REALTIME, &current );

    stop = arg->carg->start;
    stop.tv_sec += arg->carg->num_seconds;
    
    while( ts_lt(&current, &(arg->carg->start) ) )
    {
        pthread_yield();
        clock_gettime( CLOCK_REALTIME, &current );
    }
    DBGDISP("start thread %2d : %ld.%ld", tnum, current.tv_sec, current.tv_nsec);

    while( ts_lt(&current, &stop) ) {
        DBGDISP("thread %u lock()", tnum);
        arg->lock->lock();
        DBGDISP("thread %u back", tnum);
        counter++;
        tcounters[tnum]++;

        //yield each time through loop to make things interesting...
        //pthread_yield();

        DBGDISP("thread %u unlock()", tnum);
        arg->lock->unlock();
        DBGDISP("thread %u back", tnum);

        clock_gettime( CLOCK_REALTIME, &current );
    }
    DBGDISP(" stop thread %2d : %ld.%ld %ld\n", tnum, current.tv_sec, current.tv_nsec, tcounters[tnum] );
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

    delete tcounters;
    return retval;
}


