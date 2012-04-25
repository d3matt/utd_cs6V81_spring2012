#ifndef COMMON_H
#define COMMON_H

#include <stdint.h>

class ThreadArgs
{
public:
    Stack *stack;
    uint32_t tid;

    ThreadArgs() : stack(NULL), tid(0) {}
    ThreadArgs(Stack *s, uint32_t t) : stack(s), tid(t) {}
};

#endif /*COMMON_H*/
