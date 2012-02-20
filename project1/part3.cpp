#include <iostream>
#include <exception>

extern "C"
{
#include <stdint.h>
#include <pthread.h>
}

using namespace std;

class EmptyException : public exception
{
    virtual const char* what() const throw()
    {
        return "Queue is empty!";
    }
};

class FullException : public exception
{

    virtual const char* what() const throw()
    {
        return "Queue is full!";
    }
};

class Lock {
private:
    pthread_mutex_t lock_;
public:
    Lock() {pthread_mutex_init(&lock_, 0);}
    ~Lock() {pthread_mutex_destroy(&lock_);}
    void lock() {pthread_mutex_lock(&lock_);}
    void unlock() {pthread_mutex_unlock(&lock_);}
};

template <typename T>
class Queue
{
protected:
    int head, tail;
    int capacity_;
    T *items;
public:
    Queue<T>(int capacity) : head(0),tail(0),capacity_(capacity)
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
        if((tail - head) == capacity_) // full
        {
            lock.unlock();
            throw FullException();
        }
        else
        {
            items[tail] = item;
            tail++;
            lock.unlock();
        }
    }

    int dequeue() 
    {
        lock.lock();
        if(tail == head) // empty
        {
            lock.unlock();
            throw EmptyException();
        }
        else
        {
            int x = items[head % capacity_];
            head++;
            lock.unlock();
            return x;
        }
        return NULL;
    }
};

int main()
{
    LockQueue queue(100);

    int i = 0;
    try
    {
        while(true)
        {
            queue.enqueue(i);
            i++;
        }
    }
    catch(exception& e)
    {
        cout << e.what() << endl;
    }

    try
    {
        while(true)
        {
            cout << queue.dequeue() << endl;
        }
    }
    catch(exception& e)
    {
        cout << e.what() << endl;
    }
    return 0;
}
