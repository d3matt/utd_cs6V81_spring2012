#ifndef COMMON_H
#define COMMON_H

#include <stdint.h>
#include <time.h>

#include "Stack.h"

enum StackType
{
    LOCK,
    LOCKFREE,
    ELIMINATION
};

class Options
{
public:
    StackType stacktype;
    uint32_t numthreads;
    uint32_t seconds;
    timespec starttime;
    Options() : stacktype(LOCK), numthreads(2), seconds(1) {}
};

class ThreadArgs
{
public:
    Stack *stack;
    uint32_t tid;
    Options *options;

    ThreadArgs() : stack(NULL), tid(0), options(NULL) {}
    ThreadArgs(Stack *s, uint32_t t, Options *o) : stack(s), tid(t), options(o) {}
};

void parseArgs(Options & options, int argc, char *argv[]);
void testCommon(Options & options, void *func(void *));

inline bool operator< (timespec &left, timespec &right)
{
    if(left.tv_sec > right.tv_sec)
        return false;
    else if(left.tv_nsec < right.tv_nsec)
        return true;
    else if(left.tv_sec < right.tv_sec)
        return true;
    return false;
}

inline bool operator> (timespec &left, timespec &right)
{
    return !(left < right);
}

#endif /*COMMON_H*/
