
#include <iostream>
#include <string>
#include <stdint.h>
#include <stdio.h>
#include <time.h>

#include "Common.h"
#include "Random.h"

using namespace std;

mt_gen gen;
real_distribution_type dist(0,1);
real_variate_generator zeroone(gen, dist);

uint64_t totalpushcount, totalpopcount;
Stack *globalstack;

void *worker(void *args)
{
    ThreadArgs *targs = (ThreadArgs*)args;
    Stack *stack = targs->stack;
    uint32_t tid = targs->tid;

    uint64_t pushcount = 0, popcount = 0;

    timespec current;
    clock_gettime(CLOCK_REALTIME, &current);
    
    while( current < targs->options->starttime ) 
    {
        clock_gettime(CLOCK_REALTIME, &current);
    }

    globalstack = stack;

    DEBUG1("Thread %u starting\n", tid);
    for(int i = 0; i < 10000; i++)
    {
        Node *n;
        if(zeroone() < 0.5)
        {
            DEBUG3("%u trying to push\n", tid);
            n = new Node((tid*10000) + i);
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
            }
            else
            {
                i--;
            }
        }
    }

    DEBUG1("%u: pushed %lu, popped %lu\n", tid, pushcount, popcount);
    totalpushcount += pushcount;
    totalpopcount += popcount;

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
    }

    DEBUG1("%lu pushed, %lu popped, %lu leftover\n", totalpushcount, totalpopcount, mypopcount);

    printf("%lu stack operations\n", totalpushcount + totalpopcount);

    return 0;
}
