/********************************************************
* This work is licensed under the Creative Commons      *
* Attribution-ShareAlike 3.0 Unported License.          *
* To view a copy of this license, visit                 *
* http://creativecommons.org/licenses/by-sa/3.0/ or     *
* send a letter to Creative Commons, 444 Castro Street, *
* Suite 900, Mountain View, California, 94041, USA.     *
*********************************************************/

// Written by James Hall and Matthew Stoltenberg

#ifndef LOCKSTACK_H
#define LOCKSTACK_H

#include <pthread.h>

#include "Stack.h"
#include "Node.h"

class LockStack : public Stack
{
    pthread_mutex_t mutex;
    Node *head;

public:
    LockStack();
    ~LockStack();
    void push(Node *node);
    Node *pop(void);
};

#endif /*LOCKSTACK_H*/
