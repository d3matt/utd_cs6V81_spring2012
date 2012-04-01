#include <iostream>
#include <cstdio>
#include <boost/lexical_cast.hpp>
#include <vector>

#include <malloc.h>
#include <stdint.h>
#include <pthread.h>
#include <errno.h>

#include "ALock.h"
#include "TASLock.h"

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

class worker_thread_arg
{
public:
    uint32_t    tnum;
    LOCK       *lock;
};

void *worker_thread(void *Arg);

static uint64_t counter;

int main(int argc, char ** argv)
{
    uint32_t num_threads;
    vector<pthread_t> v;

    LOCK * testLock;

    if(argc != 2) {
        cerr << "./part2 <num_threads>" << endl;
        return -1;
    }
    num_threads = boost::lexical_cast<uint32_t>(argv[1]);

    cout << "initializing LOCK" << endl;
    testLock = new TASLock();
    cout << "Spawning " << num_threads << " threads." << endl;

    for (uint32_t i = 0; i < num_threads; i++)
    {
        pthread_t p;
        worker_thread_arg * arg = new worker_thread_arg();
        arg->tnum = i;
        arg->lock = testLock;

        printf("Creating thread %d\n", arg->tnum);
        if ( pthread_create(&p, NULL, worker_thread, arg) ) {
            perror("pthread_create()");
            return -1;
        }
        v.push_back(p);
    }

    for (uint32_t i=0; i < num_threads; i++)
        pthread_join(v[i], NULL);

    cout << "counter is: " << counter << endl;
    delete testLock;
}


void *worker_thread(void *Arg)
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
