#ifndef __COMMON_H__
#define __COMMON_H__

#include <stdint.h>
#include <time.h>

//our "interface"
class LOCK
{
public:
    LOCK() {};
    virtual ~LOCK() {};
    virtual void lock(void) = 0;
    virtual void unlock(void) = 0;
};

enum locktype_t
{
    TEST_TASLOCK,
    TEST_TTASLOCK,
    TEST_BACKOFF,
    TEST_ALOCK,
    TEST_PTHREAD
};

class common_args
{
public:
    locktype_t  locktype;
    uint32_t    minDelay;
    uint32_t    maxDelay;
    uint32_t    num_threads;
    uint32_t    num_seconds;

    //for second test
    timespec    start;

    common_args();
};

class worker_thread_arg
{
public:
    uint32_t        tnum;
    LOCK           *lock;
    common_args    *carg;
};

void parse_args(int argc, char **argv, common_args *carg);
LOCK * create_lock(common_args *carg);
int test_common(common_args *carg, void *worker_thread(void *) );


#endif //__COMMON_H__
