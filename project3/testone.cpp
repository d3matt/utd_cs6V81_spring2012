
#include <iostream>
#include <string>
#include <stdint.h>
#include <stdio.h>
#include <time.h>

#include "Common.h"
#include "Random.h"

#ifdef PROJ_DEBUG3
#define PROJ_DEBUG2
#endif

#ifdef PROJ_DEBUG2
#define PROJ_DEBUG1
#endif

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

    printf("Thread %u starting\n", tid);
    for(int i = 0; i < 10000; i++)
    {
        Node *n;
        if(zeroone() < 0.5)
        {
#ifdef PROJ_DEBUG3
            printf("%u trying to push\n", tid);
#endif
            n = new Node((tid*10000) + i);
            stack->push(n);
#ifdef PROJ_DEBUG2
            printf("%u pushed: %d\n", tid, n->data);
#endif
            pushcount++;
        }
        else
        {
#ifdef PROJ_DEBUG3
            printf("%u trying to pop\n", tid);
#endif
            n = stack->pop();
            if(n != NULL)
            {
#ifdef PROJ_DEBUG2
                printf("%u popped: %d\n", tid, n->data);
#endif
                //delete n;
                //n = NULL;
                popcount++;
            }
            else
            {
                i--;
            }
        }
    }

#ifdef PROJ_DEBUG1
    printf("%u: pushed %lu, popped %lu\n", tid, pushcount, popcount);
#endif
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
#ifdef PROJ_DEBUG3
        printf("%d\n", n->data);
#endif
        mypopcount++;
    }

    printf("%lu pushed, %lu popped, %lu leftover\n", totalpushcount, totalpopcount, mypopcount);

    return 0;
}
