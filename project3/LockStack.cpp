/********************************************************
* This work is licensed under the Creative Commons      *
* Attribution-ShareAlike 3.0 Unported License.          *
* To view a copy of this license, visit                 *
* http://creativecommons.org/licenses/by-sa/3.0/ or     *
* send a letter to Creative Commons, 444 Castro Street, *
* Suite 900, Mountain View, California, 94041, USA.     *
*********************************************************/

// Written by James Hall and Matthew Stoltenberg


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
    pthread_mutex_lock(&mutex);
    if(head != NULL)
    {
        node = head;
        head = head->next;
    }
    pthread_mutex_unlock(&mutex);

    return node;
}
