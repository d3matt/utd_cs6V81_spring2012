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
        cerr << "./part2a <num_threads>" << endl;
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
    for(uint32_t i=0 ; i < 100000; i++) {
        usleep(1);
        __sync_add_and_fetch (&counter, 1);
    }
    printf("End of thread %d\n", *tnum);
    delete tnum;
    return NULL;
}
