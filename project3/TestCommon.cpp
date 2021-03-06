/********************************************************
* This work is licensed under the Creative Commons      *
* Attribution-ShareAlike 3.0 Unported License.          *
* To view a copy of this license, visit                 *
* http://creativecommons.org/licenses/by-sa/3.0/ or     *
* send a letter to Creative Commons, 444 Castro Street, *
* Suite 900, Mountain View, California, 94041, USA.     *
*********************************************************/

// Written by James Hall and Matthew Stoltenberg


// CPP headers
#include <cstdio>
#include <iostream>
#include <string>
#include <vector>

// C headers
#include <pthread.h>
#include <time.h>

// Boost headers
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>

// Our headers
#include "Common.h"
#include "LockStack.h"
#include "LockFreeStack.h"
#include "Elimination.h"

using namespace std;

void usage()
{
    cerr << "USAGE:" << endl
            << "   testone [LOCK|LOCKFREE|ELIMINATION] [NUMTHREADS=<threads>] [SECONDS=<seconds>] [HELP]" << endl
            << "   LOCK        uses the locking stack"             << endl
            << "   LOCKFREE    uses the lockfree stack"            << endl
            << "   ELIMINATION uses the elimination backoff stack" << endl
            << "   SECONDS     determines how long the test runs"  << endl
            << "   NUMTHREADS  determines how many threads run"    << endl
            << "   HELP        prints this message and exits"      << endl;
}

void parseArgs(Options & options, int argc, char *argv[])
{
    for(int i = 1; i < argc; i++)
    {
        string arg = argv[i];
        boost::algorithm::to_upper(arg);

        if(arg == "LOCK")               options.stacktype = LOCK;
        else if(arg == "LOCKFREE")      options.stacktype = LOCKFREE;
        else if(arg == "ELIMINATION")   options.stacktype = ELIMINATION;

        else if(arg.compare(0, 8,  "SECONDS=") == 0)
        {
            options.seconds = boost::lexical_cast<uint32_t>(arg.substr(8));
        }
        else if(arg.compare(0, 11, "NUMTHREADS=") == 0)
        {
            options.numthreads = boost::lexical_cast<uint32_t>(arg.substr(11));
        }
        else if(arg.compare(0, 4, "HELP") == 0)
        {
            usage();
            exit(0);
        }
        else
        {
            cerr << "BAD ARG: " << arg << endl;
            usage();
            exit(1);
        }
    }    
}

void testCommon(Options &options, void *func(void *))
{
    Stack *stack;
    switch(options.stacktype)
    {
    case LOCKFREE:
        DEBUG1("LOCKFREE with %u threads\n", options.numthreads);
        stack = new LockFreeStack();
        break;
    case ELIMINATION:
        DEBUG1("ELIMINATION with %u threads\n", options.numthreads);
        stack = new EliminationStack();
        break;
    case LOCK:
    default:
        DEBUG1("LOCK with %u threads\n", options.numthreads);
        stack = new LockStack();
    }

    vector<pthread_t> ids;
    vector<ThreadArgs*> args;
    timespec start;
    clock_gettime( CLOCK_REALTIME, &options.starttime );
    options.starttime.tv_sec += 100;
    options.stoptime = options.starttime;

    for(uint32_t i = 0; i < options.numthreads; i++)
    {
        pthread_t p;
        ThreadArgs *arg = new ThreadArgs(stack, i, &options);
        pthread_create(&p, NULL, func, arg);
        ids.push_back(p);
        args.push_back(arg);
    }

    clock_gettime( CLOCK_REALTIME, &start );

    options.starttime.tv_sec = start.tv_sec + 1;
    options.stoptime.tv_sec = options.starttime.tv_sec + options.seconds;

    for(uint32_t i = 0; i < options.numthreads; i++)
    {
        pthread_join(ids[i], NULL);
    }

    while(args.size() > 0)
    {
        ThreadArgs *arg = args.back();
        args.pop_back();
        delete arg;
    }
}
