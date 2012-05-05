#ifndef COMMON_H
#define COMMON_H

#include <stdint.h>
#include <time.h>

#include "Stack.h"


#ifdef PROJ_DEBUG3
#define PROJ_DEBUG2
#define DEBUG3(fmt, args...) printf(fmt, ## args)
#else
#define DEBUG3(...)
#endif

#ifdef PROJ_DEBUG2
#define PROJ_DEBUG1
#define DEBUG2(fmt, args...) printf(fmt, ## args)
#else
#define DEBUG2(...)
#endif

#ifdef PROJ_DEBUG1
#define DEBUG1(fmt, args...) printf(fmt, ## args)
#else
#define DEBUG1(...)
#endif

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
    timespec stoptime;
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
