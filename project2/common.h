
//our "interface"
class LOCK
{
public:
    LOCK() {};
    virtual ~LOCK() {};
    virtual void lock(void) = 0;
    virtual void unlock(void) = 0;
};
