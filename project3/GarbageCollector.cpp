#include <cstdio>
#include "GarbageCollector.h"
using namespace std;

GarbageCollector::GarbageCollector() : clock(1)
{
    pthread_rwlock_init(&mutex, NULL);
}

GCNode * GarbageCollector::reg()
{
    GCNode * retval = new GCNode(this);
    retval->clock = getclock();
    pthread_rwlock_wrlock(&mutex);
    clocks.push_back(&retval->clock);
    pthread_rwlock_unlock(&mutex);
    DEBUG1("registering...  clock: %lu\n", retval->clock);
    return retval;
}
void GarbageCollector::dereg(GCNode *n)
{
    while( !n->empty() )
    {
        n->cleanup( getminclock() );
    }
    pthread_rwlock_wrlock(&mutex);
    clocks.remove( &(n->clock) );
    pthread_rwlock_unlock(&mutex);
    delete n;
}

uint64_t GarbageCollector::getclock(void)
{
    uint64_t retval = __sync_fetch_and_add(&clock, 1);
    DEBUG3("getclock(): %lu\n", retval);
    return retval;
}
uint64_t GarbageCollector::getminclock(void)
{
    uint64_t min=0xffffffffffffffff;
    pthread_rwlock_rdlock(&mutex);
    for(UL::iterator it = clocks.begin(); it != clocks.end(); it++)
    {
        if(*(*it) < min)
            min = *(*it);
    }
    pthread_rwlock_unlock(&mutex);
    return min;
}

void GCNode::clean(Node *n)
{
    clock = parent->getclock();
    uint64_t minclock = parent->getminclock();
    if(clock == minclock)
    {
        delete n;
        return;
    }
    count++;
    n->clock = clock;
    dirty_list.push_back(n);
    if(count > 10)
        cleanup(minclock);
}

void GCNode::cleanup(uint64_t minclock)
{
    Node *n;

    DEBUG3("cleanup: %lu\n", minclock);

    while( !dirty_list.empty() )
    {
        n = dirty_list.front();
        if(n->clock < minclock) {
            dirty_list.pop_front();
            //clean_list.push_front(n);
            delete n;
            count--;
        }
        else
            break;
    }

    clock = parent->getclock();
}

GCNode::~GCNode()
{
    Node *n;
    while( !clean_list.empty() )
    {
        n=clean_list.front();
        clean_list.pop_front();
        delete n;
    }
}

Node * GCNode::alloc(int32_t data)
{
    Node *n = NULL;

    if( !clean_list.empty() )
    {
        n = clean_list.front();
        clean_list.pop_front();
        n->data=data;
        n->next=NULL;
        return n;
    }
    else
        n = new Node(data);
    return n;
}
