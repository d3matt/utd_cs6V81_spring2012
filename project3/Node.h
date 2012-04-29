#ifndef NODE_H
#define NODE_H

#include <cstdlib> // For NULL

class Node
{
public:
    int data;
    Node *next;
    Node() : data(0), next(NULL) {}
    Node(int d) : data(d), next(NULL) {}
};

#endif /*NODE_H*/
