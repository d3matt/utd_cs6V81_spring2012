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
