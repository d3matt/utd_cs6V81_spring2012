/********************************************************
* This work is licensed under the Creative Commons      *
* Attribution-ShareAlike 3.0 Unported License.          *
* To view a copy of this license, visit                 *
* http://creativecommons.org/licenses/by-sa/3.0/ or     *
* send a letter to Creative Commons, 444 Castro Street, *
* Suite 900, Mountain View, California, 94041, USA.     *
*********************************************************/

// Written by James Hall and Matthew Stoltenberg

#ifndef ELIMINATION_H
#define ELIMINATION_H

#include "Node.h"
#include "Stack.h"
#include "Exchanger.h"

class EliminationStack : public Stack
{
    EliminationArray * backoff;
    Node * head;

    bool trypush(Node *node);
    Node * trypop(void);

public:
    void push(Node *n);
    Node * pop(void);

    EliminationStack() : backoff(new EliminationArray(100)), head(new Node()) {}
    ~EliminationStack();
};

#endif /*ELIMINATION_H*/
