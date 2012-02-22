#include <iostream>
#include <exception>

extern "C"
{
#include <stdint.h>
#include <pthread.h>
}

using namespace std;

class Lock {
private:
    pthread_mutex_t lock_;
    pthread_cond_t cond_;
public:
    Lock() {pthread_mutex_init(&lock_, 0); pthread_cond_init(&cond_, 0);}
    ~Lock() {pthread_mutex_destroy(&lock_);pthread_cond_destroy(&cond_);}
    void lock() {pthread_mutex_lock(&lock_);}
    void unlock() {pthread_mutex_unlock(&lock_);}
    void wait() {pthread_cond_wait(&cond_, &lock_);}
    void signal() {pthread_cond_signal(&cond_);}
};

template <typename T>
class Queue
{
protected:
    int head_, tail_;
    int capacity_;
    T *items;
public:
    Queue<T>(int capacity) : head_(0),tail_(0),capacity_(capacity)
    {
        items = new T[capacity];
    }
    virtual ~Queue<T>() {delete [] items;}
    virtual void enqueue(T item) = 0;
    virtual T dequeue() = 0;
};

class LockQueue: public Queue<int>
{
private:
    Lock lock;
    pthread_cond_t condition;
public:
    LockQueue(int capacity) : Queue<int>(capacity) {pthread_cond_init(&condition, 0);}

    void enqueue(int item)
    {
        lock.lock();
        if((tail_ - head_) == capacity_) // full
        {
            cout << "Queue is full, waiting..." << endl;
            lock.wait();
        }
        if(tail_ == head_)
        {
            lock.signal();
        }
        items[tail_ % capacity_] = item;
        tail_++;
        cout << "Enqueueing " << item << endl;
        lock.unlock();
    }

    int dequeue() 
    {
        int x;
        lock.lock();
        if(tail_ == head_) // empty
        {
            cout << "Queue is empty, waiting..." << endl;
            lock.wait();
        }
        if( (tail_ - head_) == capacity_)
        {
            lock.signal();
        }
        x = items[head_ % capacity_];
        head_++;
        cout << "Dequeueing " << x << endl;
        lock.unlock();
        return x;
    }
};

typedef struct param_
{
    Queue<int> *q;
}params_t, *pParams_t;

void *producer(void *param)
{
    Queue<int> *q = ((pParams_t)param)->q;
    for(int i = 0; i < 200; i++)
    {
        q->enqueue(i);
    }

    return NULL;
}

void *consumer(void *param)
{
    Queue<int> *q = ((pParams_t)param)->q;
    for(int i = 0; i < 200; i++)
    {
        q->dequeue();
    }

    return NULL;
}

int main()
{
    params_t params;
    pthread_t prodId, consId;

    params.q = new LockQueue(100);
    pthread_create(&prodId, 0, producer, &params);
    pthread_create(&consId, 0, consumer, &params);
    pthread_join(prodId, NULL);
    pthread_join(consId, NULL);
    delete params.q;

    /* not yet...
    params.q = new LockFreeQueue(100);
    pthread_create(&prodId, 0, producer, &params);
    pthread_create(&consId, 0, consumer, &params);
    pthread_join(prodId, NULL);
    pthread_join(consId, NULL);
    delete params.q;
    */

    return 0;
}
