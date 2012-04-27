#ifndef LOCKFREESTACK_H
#define LOCKFREESTACK_H

#include <pthread.h>

#include <boost/version.hpp>

#if BOOST_VERSION == 103301

#include <boost/random/uniform_int.hpp>
#include <boost/random/mersenne_twister.hpp>

typedef boost::uniform_int<> distribution_type;
typedef boost::mt19937 mt_gen;

#else

#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>

typedef boost::random::uniform_int_distribution<> distribution_type;
typedef boost::random::mt19937 mt_gen;

#endif

#include "Stack.h"
#include "Node.h"

class LockFreeStack : public Stack
{
    Node *head;
    
    mt_gen gen;
    uint32_t limit;

    bool trypush(Node *node);
    Node* trypop(void);
    void backoff();

public:
    LockFreeStack() : head(new Node), limit(1) {gen.seed(0x55AAFF00);}
    ~LockFreeStack();
    void push(Node *node);
    Node *pop(void);
};

#endif /*LOCKFREESTACK_H*/
