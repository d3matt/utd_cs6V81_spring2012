#include <atomic_ops.h>
#include <pthread.h>

#include "PThreadLock.h"

using namespace std;

PThreadLock::PThreadLock()
{
    pthread_mutex_init(&mutex, NULL);
}

void PThreadLock::lock()
{
    pthread_mutex_lock(&mutex);
}

void PThreadLock::unlock()
{
    pthread_mutex_unlock(&mutex);
}

