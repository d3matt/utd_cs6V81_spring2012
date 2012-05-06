#ifndef NODE_H
#define NODE_H

#include <cstdlib>  // For NULL
#include <stdint.h> //for uint64_t

class Node
{
public:
    int data;
#ifdef USE_GC
    uint64_t clock;
#endif
    Node *next;
    Node() : data(0), next(NULL) {}
    Node(int d) : data(d), next(NULL) {}
};

#endif /*NODE_H*/
