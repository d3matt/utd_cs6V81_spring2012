#include <iostream>
#include <vector>

#include <malloc.h>

#include <cstdio>
#include <pthread.h>
#include <errno.h>
#include <boost/lexical_cast.hpp>

using namespace std;

void *worker_thread(void *threadnum);

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
        cout << "creating thread " << i << endl;
        if ( pthread_create(&p, NULL, worker_thread, num) ) {
            perror("pthread_create()");
            return -1;
        }
        v.push_back(p);

    }
}


void *worker_thread(void *threadnum)
{
    uint32_t *tnum = (uint32_t *) threadnum;
    cout << "In thread... " << *tnum << endl;
    delete tnum;
    return NULL;
}
