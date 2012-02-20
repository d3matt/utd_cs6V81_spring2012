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
pthread_mutex_t M;

int main(int argc, char ** argv)
{
    uint32_t num_threads;
    vector<pthread_t> v;

    if(argc != 2) {
        cerr << "./part1 <num_threads>" << endl;
        return -1;
    }
    num_threads = boost::lexical_cast<uint32_t>(argv[1]);

    cout << "initializing mutex" << endl;
    pthread_mutex_init(&M, NULL);
    cout << "Spawning " << num_threads << " threads." << endl;

    for (uint32_t i = 0; i < num_threads; i++)
    {
        pthread_t p;
        uint32_t * num = new uint32_t;
        *num=i;
        printf("Creating thread %d\n", *num);
        if ( pthread_create(&p, NULL, worker_thread, num) ) {
            perror("pthread_create()");
            return -1;
        }
        v.push_back(p);
    }

    for (uint32_t i=0; i < num_threads; i++)
        pthread_join(v[i], NULL);

    cout << "counter is: " << counter << endl;
    pthread_mutex_destroy(&M);
}


void *worker_thread(void *threadnum)
{
    uint32_t *tnum = (uint32_t *) threadnum;
    printf("Start of thread %d\n", *tnum);
    for(uint32_t i=0 ; i < 10000; i++) {
        pthread_mutex_lock(&M);
        counter++;
        pthread_mutex_unlock(&M);
    }
    printf("End of thread %d\n", *tnum);
    delete tnum;
    return NULL;
}
