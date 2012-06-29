/********************************************************
* This work is licensed under the Creative Commons      *
* Attribution-ShareAlike 3.0 Unported License.          *
* To view a copy of this license, visit                 *
* http://creativecommons.org/licenses/by-sa/3.0/ or     *
* send a letter to Creative Commons, 444 Castro Street, *
* Suite 900, Mountain View, California, 94041, USA.     *
*********************************************************/

// Written by James Hall and Matthew Stoltenberg

#include <iostream>
#include <vector>

extern "C"
{
#include <malloc.h>
#include <stdint.h>
#include <pthread.h>
#include <errno.h>
}

#include <cstdio>
#include <boost/lexical_cast.hpp>

using namespace std;

void *worker_thread(void *threadnum);

uint32_t counter = 0;

int main(int argc, char ** argv)
{
    uint32_t num_threads;
    vector<pthread_t> v;

    if(argc != 2) {
        cerr << "./part1 <num_threads>" << endl;
        return -1;
    }
    num_threads = boost::lexical_cast<uint32_t>(argv[1]);
    cout << "Spawning " << num_threads << " threads." << endl;

    for (uint32_t i = 0; i < num_threads; i++)
    {
        pthread_t p;
        uint32_t * num = new uint32_t;
        *num=i;
        printf("Creating thread %d\n", *num);
        cout << "creating thread " << i << endl;
        if ( pthread_create(&p, NULL, worker_thread, num) ) {
            perror("pthread_create()");
            return -1;
        }
        v.push_back(p);
    }

    for (uint32_t i=0; i < num_threads; i++)
        pthread_join(v[i], NULL);

    cout << "counter is: " << counter << endl;
}


void *worker_thread(void *threadnum)
{
    uint32_t *tnum = (uint32_t *) threadnum;
    printf("Start of thread %d\n", *tnum);
    for(uint32_t i=0 ; i< 10000; i++) {
        uint32_t local;
        local = counter;
        //usleep(1);
        local++;

        counter = local;
    }
    printf("End of thread %d\n", *tnum);
    delete tnum;
    return NULL;
}
