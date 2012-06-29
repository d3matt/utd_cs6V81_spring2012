/********************************************************
* This work is licensed under the Creative Commons      *
* Attribution-ShareAlike 3.0 Unported License.          *
* To view a copy of this license, visit                 *
* http://creativecommons.org/licenses/by-sa/3.0/ or     *
* send a letter to Creative Commons, 444 Castro Street, *
* Suite 900, Mountain View, California, 94041, USA.     *
*********************************************************/

// Written by James Hall and Matthew Stoltenberg

#ifndef __GARBAGE_COLLECTOR_H__
#define __GARBAGE_COLLECTOR_H__

#include <list>

#include <pthread.h>
#include <stdint.h>
#include "Common.h"
#include "boost/circular_buffer.hpp"

typedef std::list<volatile uint64_t *> UL;
typedef boost::circular_buffer<Node *> NCIRC;

#define LISTSIZE 1024*1024

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
    uint64_t                total_allocs;
};

class GCNode
{
    GarbageCollector   *parent;
    NCIRC               dirty_list;
    NCIRC               clean_list;
public:
    uint64_t    clock;
    uint64_t    allocs;
                GCNode(GarbageCollector *parent)
                    :parent(parent), dirty_list(1024*1024), clean_list(1024*1024), allocs(0) {}
                ~GCNode();
    void        clean(Node *n);
    void        cleanup(uint64_t minclock);
    bool        empty() { return dirty_list.empty(); }
    Node       *alloc(int32_t data);
};

#endif //__GARBAGE_COLLECTOR_H__
