#ifndef EXCHANGER_H
#define EXCHANGER_H

#include <cstdlib>
#include <stdint.h>

#include "Node.h"

class Exchanger
{
    enum ExchangeState
    {
        EMPTY,
        WAITING,
        BUSY
    };
    class Slot
    {
        uint64_t data;
    public:
        void set(Node *n, ExchangeState state)
        {
            data = (uint64_t)n | (state & 0x3);
        }

        Node *get(void)
        {
            return (Node*) (data&(~0x3));
        }

        bool compareAndSet(Node *oldN, Node *newN, ExchangeState oldState, ExchangeState newState)
        {
            uint64_t oldData = (uint64_t)oldN | (oldState & 0x3);
            uint64_t newData = (uint64_t)newN | (newState & 0x3);
            return __sync_bool_compare_and_swap(&data, oldData, newData);
        }

        Slot() : data(NULL) {}
    };

    Slot slot;

public:
    Node * exchange(Node *n, uint64_t nanos);
};

class EliminationArray
{
    int maxrange;
    Exchanger *exchanger;
public:
    Node * visit(Node *n, uint32_t range);

    EliminationArray(int m) : maxrange(m), exchanger(new Exchanger[maxrange]) {}
};

#endif /*EXCHANGER_H*/
