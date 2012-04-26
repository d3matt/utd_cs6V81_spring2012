
#include "LockStack.h"

#include <stdio.h>

LockStack::LockStack() : head(NULL)
{
    pthread_mutex_init(&mutex, NULL);
}

LockStack::~LockStack()
{
    pthread_mutex_lock(&mutex);
    while(head != NULL)
    {
        Node *n = head;
        head = head->next;
        delete n;
        n = NULL;
    }
    pthread_mutex_unlock(&mutex);
}

void LockStack::push(Node *node)
{
    pthread_mutex_lock(&mutex);
    node->next = head;
    head = node;
    pthread_mutex_unlock(&mutex);
}

Node* LockStack::pop(void)
{
    Node *node = NULL;
    if(head != NULL)
    {
        pthread_mutex_lock(&mutex);
        node = head;
        head = head->next;
        pthread_mutex_unlock(&mutex);
    }

    return node;
}
