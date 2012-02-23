#include <iostream>
#include <exception>

extern "C"
{
#include <stdint.h>
#include <pthread.h>
#include <time.h>
}

using namespace std;

static const int DEFAULT_NUM_ITEMS = 200000;
static const int DEFAULT_QUEUE_SIZE = 100;

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
public:
    LockQueue(int capacity) : Queue<int>(capacity) {}

    void enqueue(int item)
    {
        lock.lock();
        if((tail_ - head_) == capacity_) // full
        {
            lock.wait();
        }
        if(tail_ == head_)
        {
            lock.signal();
        }
        items[tail_ % capacity_] = item;
        tail_++;
        lock.unlock();
    }

    int dequeue() 
    {
        int x;
        lock.lock();
        if(tail_ == head_) // empty
        {
            lock.wait();
        }
        if( (tail_ - head_) == capacity_)
        {
            lock.signal();
        }
        x = items[head_ % capacity_];
        head_++;
        lock.unlock();
        return x;
    }
};

class LockFreeQueue: public Queue<int>
{
public:
    LockFreeQueue(int capacity) : Queue<int>(capacity) {}

    void enqueue(int item)
    {
        while((tail_ - head_) == capacity_) usleep(1); // full
        items[tail_ % capacity_] = item;
        tail_++;
    }

    int dequeue() 
    {
        int x;
        while(tail_ == head_) usleep(1); // empty
        x = items[head_ % capacity_];
        head_++;
        return x;
    }
};


typedef struct param_
{
    Queue<int> *q;
    int n;
}params_t;

void *producer(void *param)
{
    Queue<int> *q = ((params_t*)param)->q;
    int n = ((params_t*)param)->n;
    for(int i = 0; i < n; i++)
    {
        q->enqueue(i);
    }
    cout << "   Producer: enqueued " << n << " items" << endl;

    return NULL;
}

void *consumer(void *param)
{
    Queue<int> *q = ((params_t*)param)->q;
    int n = ((params_t*)param)->n;
    for(int i = 0; i < n; i++)
    {
        q->dequeue();
    }
    cout << "   Consumer: dequeued " << n << " items" << endl;

    return NULL;
}

int main(int argc, char *argv[])
{
    params_t params;
    pthread_t prodId, consId;

    int n = DEFAULT_NUM_ITEMS, m = DEFAULT_QUEUE_SIZE;

    params.n = n;

    

    params.q = new LockQueue(m);
    cout << "Using Locking Queue: " << endl;
    cout << "   Creating producer and consumer..." << endl;
    pthread_create(&prodId, 0, producer, &params);
    pthread_create(&consId, 0, consumer, &params);
    pthread_join(prodId, NULL);
    pthread_join(consId, NULL);
    cout << "   Producer and Consumer stopped." << endl;
    delete params.q;

    params.q = new LockFreeQueue(DEFAULT_QUEUE_SIZE);
    cout << "Using Lock-free Queue: " << endl;
    cout << "   Creating producer and consumer..." << endl;
    pthread_create(&prodId, 0, producer, &params);
    pthread_create(&consId, 0, consumer, &params);
    pthread_join(prodId, NULL);
    pthread_join(consId, NULL);
    cout << "   Producer and Consumer stopped." << endl;
    delete params.q;

    return 0;
}
