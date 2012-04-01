#include <stdint.h>

#include "common.h"

class ALock : public LOCK
{
    bool * flag;
    uint32_t tail;
    uint32_t size;

    //don't really see a need for TLS for this...
    uint32_t current_slot;

public:
    ALock(uint32_t capacity);
    ~ALock();

    void lock(void);
    void unlock(void);
};
