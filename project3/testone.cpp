
#include <iostream>
#include <string>
#include <sstream>
#include <pthread.h>
#include <stdint.h>
#include <stdio.h>

#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>

#include "Node.h"
#include "LockStack.h"
#include "Common.h"

using namespace std;

void *worker(void *args)
{
    ThreadArgs *targs = (ThreadArgs*)args;
    Stack *stack = targs->stack;
    uint32_t tid = targs->tid;
    Node *n;

    printf("Thread %u starting...\n", tid);

    for(int i = 0; i < 10000; i++)
    {
        n = new Node((tid*10000) + i);
        stack->push(n);
        pthread_yield(); //Let's make this interesting...
    }

    return NULL;
}

int main(int argc, char *argv[])
{
    Stack *stack = NULL;
    uint32_t numthreads = 2;

    for(int i = 1; i < argc; i++)
    {
        string arg = argv[i];
        boost::algorithm::to_upper(arg);

        if(arg == "LOCK")               stack = new LockStack();
        else if(arg == "LOCKFREE")      stack = NULL;
        else if(arg == "ELIMINATION")   stack = NULL;
        else if(arg.compare(0, 11, "NUMTHREADS=") == 0) numthreads = boost::lexical_cast<uint32_t>(arg.substr(11));
        else cerr << "BAD ARG: " << arg << endl;

    }

    if(stack == NULL)
    {
        stack = new LockStack();
    }

    pthread_t *ids = new pthread_t[numthreads];
    ThreadArgs *args = new ThreadArgs[numthreads];
    for(int i = 0; i < numthreads; i++)
    {
        args[i].stack = stack;
        args[i].tid = i;
        pthread_create(&ids[i], NULL, worker, &args[i]);
    }

    for(int i = 0; i < numthreads; i++)
    {
        pthread_join(ids[i], NULL);
    }

    Node *n;
    uint32_t items = 0;
    while ( (n = stack->pop()) != NULL)
    {
        //printf("%d\n", n->data);
        items++;
        delete n;
        n = NULL;
    }

    printf("%u\n", items);

    delete [] ids;
    delete [] args;

    return 0;
}
