
#include <cstdio>

#include "Elimination.h"

EliminationStack::~EliminationStack()
{
    while(head->next != NULL)
    {
        Node *n = pop();
        delete n;
        n = NULL;
    }
}

bool EliminationStack::trypush(Node *node)
{
    Node *n = head->next;
    node->next = n;
    return __sync_bool_compare_and_swap(&head->next, n, node);
}

Node * EliminationStack::trypop(void)
{
    Node *n = NULL;
    do 
    {
        n = head->next;
        if(n == NULL)
            break;
    } while( !__sync_bool_compare_and_swap(&head->next, n, n->next));
    return n;    
}

void EliminationStack::push(Node *n)
{
    int range = 2;
    while(true)
    {
        if(trypush(n)) // Attempt a LockFree push
        {
#ifdef PROJ_DEBUG3
            printf("trypush() succeeded\n");
#endif
            return;
        }
        else
        {
            try
            {
                Node *other = backoff->visit(n, range);
                range *= 2;
                if(other == NULL)
                {
                    return; // push was successful
                }
                else
                {
                    n = other; // traded with another push (Why would we do this...?)
                }
            }
            catch(...) {}
        }
    }
    return; // Won't reach here
}

Node * EliminationStack::pop(void)
{
    Node *n = NULL;
    int range = 2;

    if(head->next == NULL) // Nothing on the stack right now...
        return n;

    while(true)
    {
        if((n = trypop()) != NULL)
        {
#ifdef PROJ_DEBUG3
            printf("trypop() succeeded\n");
#endif
            return n;
        }
        else
        {
            try
            {
                n = backoff->visit(n, range);
                range *= 2;
                if(n != NULL)
                {
                    return n;
                }
            }
            catch(...) {}
        }
    }

    return NULL;
}

