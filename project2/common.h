
#ifndef __COMMON_H__
#define __COMMON_H__
//our "interface"
class LOCK
{
public:
    LOCK() {};
    virtual ~LOCK() {};
    virtual void lock(void) = 0;
    virtual void unlock(void) = 0;
};

#endif //__COMMON_H__
