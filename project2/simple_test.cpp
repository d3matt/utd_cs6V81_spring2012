#include <iostream>
#include <cstdio>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
#include <vector>

#include <malloc.h>
#include <stdint.h>
#include <pthread.h>
#include <errno.h>

#include "TASLock.h"
#include "TTASLock.h"
#include "BackoffLock.h"
#include "ALock.h"

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

enum locktype_t
{
    TEST_TASLOCK,
    TEST_TTASLOCK,
    TEST_BACKOFF,
    TEST_ALOCK
};

void *worker_thread(void *Arg);

static uint64_t counter;

int main(int argc, char ** argv)
{
    uint32_t            num_threads;
    uint32_t            minDelay=1;
    uint32_t            maxDelay=1024;
    vector<pthread_t>   v;
    vector<string>      ARGV;
    locktype_t          locktype = TEST_TASLOCK;
    LOCK               *testLock;

    for(int32_t i=1; i < argc; i++) {
        string s = argv[i];
        boost::algorithm::to_upper(s);
        if ( s == "TASLOCK")
            locktype = TEST_TASLOCK;
        else if ( s == "TTASLOCK")
            locktype = TEST_TTASLOCK;
        else if (s == "BACKOFF")
            locktype = TEST_BACKOFF;
        else if ( s == "ALOCK")
            locktype = TEST_ALOCK;
        else
            ARGV.push_back( string(argv[i]) );
    }

    if(ARGV.size() != 1) {
        cerr << "./part2 <num_threads> [TASLOCK] [TTASLOCK] [BACKOFF] [ALOCK]" << endl;
        return -1;
    }
    num_threads = boost::lexical_cast<uint32_t>(ARGV[0]);

    switch(locktype)
    {
    case TEST_TASLOCK:
        cout << "initializing TASLock" << endl;
        testLock = new TASLock();
        break;
    case TEST_TTASLOCK:
        cout << "initializing TTASLock" << endl;
        testLock = new TTASLock();
        break;
    case TEST_BACKOFF:
        cout << "initializing BackoffLock" << endl;
        testLock = new BackoffLock(minDelay, maxDelay);
        break;
    case TEST_ALOCK:
        cout << "initializing ALock" << endl;
        testLock = new ALock(num_threads);
        break;
    }

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
