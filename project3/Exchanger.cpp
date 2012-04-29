
#include <ctime>

#include "Common.h"
#include "Exchanger.h"
#include "Random.h"

static const uint64_t NANO_DUR = 512;  // 512 nanoseconds

inline uint32_t min(uint32_t left, uint32_t right)
{
    if(left <= right)   return left;
    else                return right;
}

Node * Exchanger::exchange(Node *n)
{
    timespec current, bound;
    ExchangeState state;

    clock_gettime(CLOCK_REALTIME, &bound);
    bound.tv_nsec += NANO_DUR;

    while(true)
    {
        clock_gettime(CLOCK_REALTIME, &current);
        if (current > bound) throw TimeoutException();
        Node *otheritem = slot.get(&state);
        switch(state)
        {
        case EMPTY:
            if(slot.compareAndSet(otheritem, n, EMPTY, WAITING))
            {
                while(current < bound)
                {
                    clock_gettime(CLOCK_REALTIME, &current);
                    otheritem = slot.get(&state);
                    if(state == BUSY)
                    {
                        slot.set(NULL, EMPTY);
                        return otheritem;
                    }
                }
                if(slot.compareAndSet(n, NULL, WAITING, EMPTY))
                {
                    throw TimeoutException();
                }
                else
                {
                    otheritem = slot.get(&state);
                    slot.set(NULL, EMPTY);
                    return otheritem;
                }
            }
            break;
        case WAITING:
            if(slot.compareAndSet(otheritem, n, WAITING, BUSY))
            {
                return otheritem;
            }
            break;
        case BUSY:
            break;
        }
    }

    return NULL;
}

Node * EliminationArray::visit(Node *n, uint32_t range)
{
    static mt_gen gen(0xFF00AA55);
    int_distribution_type dist(0, min(range, maxrange));
    uint32_t slot = dist(gen);
    Node *retval;
    try
    {
        retval = exchanger[slot].exchange(n);
    }
    catch(...) 
    {
        retval = n;
    }
    return retval;
}
