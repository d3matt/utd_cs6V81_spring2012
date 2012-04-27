
#include <iostream>
#include <string>
#include <stdint.h>
#include <stdio.h>
#include <time.h>

#if BOOST_VERSION == 103301

#include <boost/random/uniform_real.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/variate_generator.hpp>

typedef boost::uniform_real<> distribution_type;
typedef boost::mt19937 mt_gen;
typedef boost::variate_generator variate_generator;

#else

#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_real_distribution.hpp>
#include <boost/random/variate_generator.hpp>

typedef boost::random::uniform_real_distribution<> distribution_type;
typedef boost::random::mt19937 mt_gen;
typedef boost::random::variate_generator<mt_gen, distribution_type> variate_generator;

#endif

#include "Common.h"

using namespace std;

mt_gen gen;
distribution_type dist(0,1);
variate_generator zeroone(gen, dist);

Stack *globalstack;

bool operator< (timespec &left, timespec &right)
{
    if(left.tv_sec > right.tv_sec)
        return false;
    else if(left.tv_nsec < right.tv_nsec)
        return true;
    else if(left.tv_sec < right.tv_sec)
        return true;
    return false;
}

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
            //printf("%u attempting push\n", tid);
            n = new Node((tid*10000) + i);
            stack->push(n);
            //printf("%u pushed: %d\n", tid, n->data);
            pushcount++;
        }
        else
        {
            //printf("%u attempting pop\n", tid);
            n = stack->pop();
            if(n != NULL)
            {
                //printf("%u popped: %d\n", tid, n->data);
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

    printf("%u: pushed %lu, popped %lu\n", tid, pushcount, popcount);

    return NULL;
}

int main(int argc, char *argv[])
{
    Options options;

    gen.seed(time(0));
    
    parseArgs(options, argc, argv);
    testCommon(options, worker);

    Node *n;
    while((n = globalstack->pop()) != NULL)
    {
        printf("%d\n", n->data);
    }

    return 0;
}
