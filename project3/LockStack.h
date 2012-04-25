#ifndef LOCKSTACK_H
#define LOCKSTACK_H

#include <pthread.h>

#include "Stack.h"
#include "Node.h"

class LockStack : public Stack
{
    pthread_mutex_t mutex;
    Node *head;

public:
    LockStack();
    ~LockStack();
    void push(Node *node);
    Node *pop(void);
};

#endif /*LOCKSTACK_H*/
