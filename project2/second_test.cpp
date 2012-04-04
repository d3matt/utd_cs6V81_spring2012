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

static uint64_t counter;

static void *worker_thread(void *Arg)
{
    worker_thread_arg * arg = (worker_thread_arg *) Arg;
    printf("Start of thread %d\n", arg->tnum);
    for(uint32_t i=0 ; i < 10000; i++) {
        DBGDISP("thread %u lock()", arg->tnum);
        arg->lock->lock();
        DBGDISP("thread %u back", arg->tnum);
        counter++;

        //yield each time through loop to make things interesting...
        pthread_yield();

        DBGDISP("thread %u unlock()", arg->tnum);
        arg->lock->unlock();
        DBGDISP("thread %u back", arg->tnum);
    }
    printf("End of thread %d\n", arg->tnum);
    delete arg;
    return NULL;
}

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


int main(int argc, char ** argv)
{
    common_args         carg;

    parse_args(argc, argv, &carg);

    timespec start;
    timespec stop;
    timespec current;
    clock_gettime( CLOCK_REALTIME, &start );
    cout << "Start : " << start.tv_sec << "." << start.tv_nsec << endl;
    current = stop = start;
    stop.tv_sec+=10;
    
    while( ts_lt(&current, &stop) )
    {
        counter++;
        clock_gettime( CLOCK_REALTIME, &current );
    }
    cout << " Stop : " << current.tv_sec << "." << current.tv_nsec << endl;

    return 0;

    test_common(&carg, worker_thread);

    cout << "counter is: " << counter << endl;
}


