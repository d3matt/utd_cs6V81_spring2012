#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
#include <iostream>
#include <string>
#include <vector>

#include <cstdio>
#include <stdint.h>
#include <pthread.h>
#include <errno.h>

#include "common.h"
#include "TASLock.h"
#include "TTASLock.h"
#include "BackoffLock.h"
#include "ALock.h"

using namespace std;

common_args::common_args() :
    locktype(TEST_TASLOCK),
    minDelay(1),
    maxDelay(128),
    num_threads(10),
    num_seconds(1)
    {
    }

void parse_args(int argc, char **argv, common_args *carg)
{

    for(int32_t i=1; i < argc; i++) {
        string s = argv[i];
        boost::algorithm::to_upper(s);
        if ( s == "TASLOCK")
            carg->locktype = TEST_TASLOCK;
        else if ( s == "TTASLOCK")
            carg->locktype = TEST_TTASLOCK;
        else if (s == "BACKOFF")
            carg->locktype = TEST_BACKOFF;
        else if ( s == "ALOCK")
            carg->locktype = TEST_ALOCK;
        else
        {
            try
            {
                carg->num_threads = boost::lexical_cast<uint32_t>(argv[i]);
            }
            catch (...)
            {
                cerr << argv[0] << " [num_threads] [TASLOCK|TTASLOCK|BACKOFF|ALOCK]" << endl;
                exit(-1);
            }
        }
    }
}

LOCK * create_lock(common_args *carg)
{
    switch(carg->locktype)
    {
    case TEST_TASLOCK:
        cout << "initializing TASLock" << endl;
        return new TASLock();
    case TEST_TTASLOCK:
        cout << "initializing TTASLock" << endl;
        return new TTASLock();
    case TEST_BACKOFF:
        cout << "initializing BackoffLock" << endl;
        return new BackoffLock(carg->minDelay, carg->maxDelay);
    case TEST_ALOCK:
        cout << "initializing ALock" << endl;
        return new ALock(carg->num_threads);
    }
    return NULL;
}

int test_common(common_args *carg, void *worker_thread(void *) )
{
    LOCK               *testLock;
    vector<pthread_t>   v;

    testLock = create_lock(carg);
    
    cout << "Spawning " << carg->num_threads << " threads." << endl;

    for (uint32_t i = 0; i < carg->num_threads; i++)
    {
        pthread_t p;
        worker_thread_arg * arg = new worker_thread_arg();
        arg->tnum = i;
        arg->lock = testLock;
        arg->carg = carg;

        printf("Creating thread %d\n", arg->tnum);
        if ( pthread_create(&p, NULL, worker_thread, arg) ) {
            perror("pthread_create()");
            return -1;
        }
        v.push_back(p);
    }

    for (uint32_t i=0; i < carg->num_threads; i++)
        pthread_join(v[i], NULL);

    delete testLock;

    return 0;
}
