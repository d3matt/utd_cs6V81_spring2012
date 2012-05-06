#ifndef __GARBAGE_COLLECTOR_H__
#define __GARBAGE_COLLECTOR_H__

#include <list>

#include <pthread.h>
#include <stdint.h>
#include "Common.h"

typedef std::list<volatile uint64_t *> UL;
typedef std::list< Node * > NLIST;

class GCList
{
    Node * arr[10240];
    uint32_t head;
    uint32_t tail;
public:
    GCList() : head(0), tail(0) {};
    uint32_t size()
    {
        if(head < tail)
            return (head+10240) - tail;
        else
            return head - tail;
        
    }
};

class GCNode;
class GarbageCollector
{
    uint64_t            clock;
    UL                  clocks;
    pthread_rwlock_t    mutex;
public:
    GarbageCollector();
    GCNode*   reg();
    void dereg(GCNode * n);
    uint64_t                getclock();
    uint64_t                getminclock();
};

class GCNode
{
    GarbageCollector   *parent;
    NLIST               dirty_list;
    NLIST               clean_list;
    uint64_t count;
public:
    uint64_t    clock;
                GCNode(GarbageCollector *parent)
                    :parent(parent), count(0) {}
                ~GCNode();
    void        clean(Node *n);
    void        cleanup(uint64_t minclock);
    bool        empty() { return dirty_list.empty(); }
    Node       *alloc(int32_t data);
};

#endif //__GARBAGE_COLLECTOR_H__
