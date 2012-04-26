
#include <cstdio>

#include "LockFreeStack.h"

LockFreeStack::~LockFreeStack()
{
    while(head != NULL)
    {
        Node *n = pop();
        delete n;
        n = NULL;
    }
}

bool LockFreeStack::trypush(Node *node)
{
    Node *oldhead = head;
    node->next = oldhead;
    return __sync_bool_compare_and_swap(&head, oldhead, node);
}

Node* LockFreeStack::trypop(void)
{
    Node *newhead = head->next;
    Node *n = head;
    if(__sync_bool_compare_and_swap(&head, n, newhead))
        return n;
    else
        return NULL;
}

void LockFreeStack::backoff(void)
{
    distribution_type dist(1, limit);
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
        //printf("Failed to push node: %p\n", node);
    }
    return;
}

Node* LockFreeStack::pop(void)
{
    Node *n = NULL;
    while(const_cast<volatile Node *>(head) != NULL && (n = trypop()) == NULL);
    return n;
}
