
#include <iostream>
#include <string>
#include <sstream>
#include <pthread.h>
#include <stdint.h>
#include <stdio.h>

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

int main()
{
    LockStack stack;
    pthread_t ids[6];
    ThreadArgs *args = new ThreadArgs[6];
    for(int i = 0; i < 6; i++)
    {
        args[i].stack = &stack;
        args[i].tid = i;
        pthread_create(&ids[i], NULL, worker, &args[i]);
    }

    for(int i = 0; i < 6; i++)
    {
        pthread_join(ids[i], NULL);
    }

    delete [] args;

    Node *n;
    while ( (n = stack.pop()) != NULL)
    {
        //printf("%d\n", n->data);
        delete n;
        n = NULL;
    }

    return 0;
}
