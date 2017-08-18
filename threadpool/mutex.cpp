#include "mutex.h"
#include <cassert>

MutexLock::MutexLock() : holder_(0)
{
    assert(pthread_mutex_init(&mu_, NULL) == 0);
}

MutexLock::~MutexLock()
{
    assert(pthread_mutex_destroy(&mu_) == 0);
}

void MutexLock::lock()
{
    assert(pthread_mutex_lock(&mu_) == 0);
    assignHolder();
}

void MutexLock::unlock()
{
    assert(pthread_mutex_unlock(&mu_) == 0);
    unassignHolder();
}

void MutexLock::assignHolder()
{
    holder_ = pthread_self();
}

void MutexLock::unassignHolder()
{
    holder_ = 0;
}

bool MutexLock::isLockedByThisThread() const
{
    return (pthread_equal(holder_, pthread_self()) != 0);
}

void MutexLock::assertLocked() const
{
    assert(isLockedByThisThread());
}