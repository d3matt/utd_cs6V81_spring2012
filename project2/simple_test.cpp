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
#include <string>
#include <vector>

#include <stdint.h>
#include <pthread.h>
#include <errno.h>

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
        //pthread_yield();

        DBGDISP("thread %u unlock()", arg->tnum);
        arg->lock->unlock();
        DBGDISP("thread %u back", arg->tnum);
    }
    printf("End of thread %d\n", arg->tnum);
    delete arg;
    return NULL;
}

int main(int argc, char ** argv)
{
    common_args         carg;

    parse_args(argc, argv, &carg);

    test_common(&carg, worker_thread);

    cout << "counter is: " << counter << endl;
}
