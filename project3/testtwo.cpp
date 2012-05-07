
#include <iostream>
#include <string>
#include <stdint.h>
#include <stdio.h>
#include <time.h>

#include "GarbageCollector.h"
#include "Common.h"
#include "Random.h"

using namespace std;

mt_gen gen;
real_distribution_type dist(0,1);
real_variate_generator zeroone(gen, dist);

uint64_t totalpushcount, totalpopcount;
Stack *globalstack;
#ifdef USE_GC
GarbageCollector GC;
#else
uint64_t total_allocs=0;
#endif

void *worker(void *args)
{
    ThreadArgs *targs = (ThreadArgs*)args;
    Stack *stack = targs->stack;
    uint32_t tid = targs->tid;
    timespec *stoptime = &targs->options->stoptime;
    #ifdef USE_GC
    GCNode* gcn = GC.reg();
    #else
    uint64_t allocs=0;
    #endif

    uint64_t pushcount = 0, popcount = 0;

    timespec current;
    clock_gettime(CLOCK_REALTIME, &current);
    
    while( current < targs->options->starttime ) 
    {
        clock_gettime(CLOCK_REALTIME, &current);
    }

    globalstack = stack;

    DEBUG1("Thread %u starting\n", tid);
    int i = 0;
    do
    {
        Node *n;
        if(zeroone() < 0.5)
        {
            DEBUG3("%u trying to push\n", tid);
            #ifdef USE_GC
            n = gcn->alloc((tid*10000) + i);
            #else
            n = new Node((tid*10000) + i);
            allocs++;
            #endif
            stack->push(n);
            DEBUG2("%u pushed: %d\n", tid, n->data);
            pushcount++;
        }
        else
        {
            DEBUG3("%u trying to pop\n", tid);
            n = stack->pop();
            if(n != NULL)
            {
                DEBUG2("%u popped: %d\n", tid, n->data);
                popcount++;
                #ifdef USE_GC
                gcn->clean(n);
                #endif
            }
            else
            {
                i--;
            }
        }
        i++;
        clock_gettime(CLOCK_REALTIME, &current);
    } while(current < *stoptime);

    DEBUG1("%u: pushed %lu, popped %lu\n", tid, pushcount, popcount);
    __sync_fetch_and_add(&totalpushcount, pushcount);
    __sync_fetch_and_add(&totalpopcount, popcount);
    #ifdef USE_GC
    GC.dereg(gcn);
    #else
    __sync_fetch_and_add(&total_allocs, allocs);
    #endif
    return NULL;
}

int main(int argc, char *argv[])
{
    Options options;
    uint64_t mypopcount;

    gen.seed(time(0));
    totalpushcount = totalpopcount = mypopcount = 0;
    
    parseArgs(options, argc, argv);
    testCommon(options, worker);

    Node *n;
    while((n = globalstack->pop()) != NULL)
    {
        DEBUG3("%d\n", n->data);
        mypopcount++;
        delete n;
    }

    DEBUG1("%lu pushed, %lu popped, %lu leftover\n", totalpushcount, totalpopcount, mypopcount);

    printf("%lu stack operations... %lu allocs\n", totalpushcount + totalpopcount,
#ifdef USE_GC
    GC.total_allocs
#else
    total_allocs
#endif
    );

    return 0;
}
