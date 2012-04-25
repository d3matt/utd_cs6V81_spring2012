#ifndef STACK_H
#define STACK_H

#include "Node.h"
class Stack
{
public:
    virtual void push(Node *n) = 0;
    virtual Node* pop() = 0;
};

#endif /*STACK_H*/
