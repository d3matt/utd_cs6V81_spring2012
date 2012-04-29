#ifndef LOCKFREESTACK_H
#define LOCKFREESTACK_H

#include <pthread.h>

#include "Node.h"
#include "Stack.h"
#include "Random.h"

class LockFreeStack : public Stack
{
    Node *head;
    
    mt_gen gen;
    uint32_t limit;

    bool trypush(Node *node);
    Node* trypop(void);
    void backoff();

public:
    LockFreeStack() : head(new Node), limit(1) {gen.seed(0x55AAFF00);}
    ~LockFreeStack();
    void push(Node *node);
    Node *pop(void);
};

#endif /*LOCKFREESTACK_H*/
