#include "condition.h"

Condition::Condition(MutexLock& mutex) : mutex_(mutex)
{
    assert(pthread_cond_init(&cond_, NULL) == 0);
}

Condition::~Condition()
{
    assert(pthread_cond_destroy(&cond_));
}

void Condition::wait()
{
    MutexLock::UnassignGuard ua(mutex_);
    assert(pthread_cond_wait(&cond_, mutex_.getPthreadMutex()) == 0);
}

void Condition::notify()
{
    MutexLock::UnassignGuard ua(mutex_);
    assert(pthread_cond_signal(&cond_) == 0);
}

void Condition::notifyAll()
{
    MutexLock::UnassignGuard ua(mutex_);
    assert(pthread_cond_broadcast(&cond_) == 0);
}