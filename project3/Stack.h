/********************************************************
* This work is licensed under the Creative Commons      *
* Attribution-ShareAlike 3.0 Unported License.          *
* To view a copy of this license, visit                 *
* http://creativecommons.org/licenses/by-sa/3.0/ or     *
* send a letter to Creative Commons, 444 Castro Street, *
* Suite 900, Mountain View, California, 94041, USA.     *
*********************************************************/

// Written by James Hall and Matthew Stoltenberg

#ifndef STACK_H
#define STACK_H

#include "Node.h"
class Stack
{
public:
    Stack() {};
    virtual ~Stack() {};
    virtual void push(Node *n) = 0;
    virtual Node* pop() = 0;
};

#endif /*STACK_H*/

