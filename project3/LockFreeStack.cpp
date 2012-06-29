/********************************************************
* This work is licensed under the Creative Commons      *
* Attribution-ShareAlike 3.0 Unported License.          *
* To view a copy of this license, visit                 *
* http://creativecommons.org/licenses/by-sa/3.0/ or     *
* send a letter to Creative Commons, 444 Castro Street, *
* Suite 900, Mountain View, California, 94041, USA.     *
*********************************************************/

// Written by James Hall and Matthew Stoltenberg


#include <cstdio>

#include "LockFreeStack.h"

LockFreeStack::~LockFreeStack()
{
    while(head->next != NULL)
    {
        Node *n = pop();
        delete n;
        n = NULL;
    }
}

bool LockFreeStack::trypush(Node *node)
{
    Node *n = head->next;
    node->next = n;
    return __sync_bool_compare_and_swap(&head->next, n, node);
}

Node* LockFreeStack::trypop(void)
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

void LockFreeStack::backoff(void)
{
    int_distribution_type dist(1, limit);
    uint32_t delay = dist(gen);

    struct timespec req;
    req.tv_sec = 0;
    req.tv_nsec = delay;
    nanosleep(&req, &req);

    limit *= 2;
    limit %= 1023; // 1 <= limit <= 512
}

void LockFreeStack::push(Node *node)
{
    while(!trypush(node))
    {
        backoff();
#ifdef PROJ_DEBUG2
        printf("Failed to push node: %p\n", node);
#endif
    }
    return;
}

Node* LockFreeStack::pop(void)
{
    Node *n = NULL;
    while(const_cast<volatile Node *>(head->next) != NULL && (n = trypop()) == NULL);
    return n;
}
