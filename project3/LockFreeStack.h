/********************************************************
* This work is licensed under the Creative Commons      *
* Attribution-ShareAlike 3.0 Unported License.          *
* To view a copy of this license, visit                 *
* http://creativecommons.org/licenses/by-sa/3.0/ or     *
* send a letter to Creative Commons, 444 Castro Street, *
* Suite 900, Mountain View, California, 94041, USA.     *
*********************************************************/

// Written by James Hall and Matthew Stoltenberg

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
