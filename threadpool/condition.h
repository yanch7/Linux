#ifndef __CONDITION_H_
#define __CONDITION_H_

#include "mutex.h"
#include <cassert>

class Condition : nocopyable
{
public:
    explicit Condition(MutexLock& mutex);
    ~Condition();
    void wait();
    void notify();
    void notifyAll();
private:
    MutexLock& mutex_;
    pthread_cond_t cond_;
};

#endif