/********************************************************
* This work is licensed under the Creative Commons      *
* Attribution-ShareAlike 3.0 Unported License.          *
* To view a copy of this license, visit                 *
* http://creativecommons.org/licenses/by-sa/3.0/ or     *
* send a letter to Creative Commons, 444 Castro Street, *
* Suite 900, Mountain View, California, 94041, USA.     *
*********************************************************/

// Written by James Hall and Matthew Stoltenberg

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
