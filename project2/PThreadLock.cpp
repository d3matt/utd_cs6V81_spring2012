/********************************************************
* This work is licensed under the Creative Commons      *
* Attribution-ShareAlike 3.0 Unported License.          *
* To view a copy of this license, visit                 *
* http://creativecommons.org/licenses/by-sa/3.0/ or     *
* send a letter to Creative Commons, 444 Castro Street, *
* Suite 900, Mountain View, California, 94041, USA.     *
*********************************************************/

// Written by James Hall and Matthew Stoltenberg

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

