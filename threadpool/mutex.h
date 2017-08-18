#ifndef __MUTEX_H_
#define __MUTEX_H_

#include <pthread.h>
#include "nocopyable.h"

class MutexLock : nocopyable
{
public:
    MutexLock();
    ~MutexLock();
    void lock();
    void unlock();
    bool isLockedByThisThread() const;
    inline pthread_mutex_t* getPthreadMutex() 
    {
        return &mu_;
    }

    void assertLocked() const;
private:
    void assignHolder();
    void unassignHolder();
private:
    pthread_mutex_t mu_;
    pthread_t holder_;

    class UnassignGuard : nocopyable
    {
    public:
        UnassignGuard(MutexLock& mu) : mutex_(mu)
        {
            mutex_.unassignHolder();
        }

        ~UnassignGuard()
        {
            mutex_.assignHolder();
        }
    private:
        MutexLock& mutex_;
    };

    friend class Condition;
};

class MutexLockGuard : nocopyable
{
public:
    explicit MutexLockGuard(MutexLock& mutex) : mutex_(mutex) 
    {
        mutex_.lock();
    }

    ~MutexLockGuard() 
    {
        mutex_.unlock();
    }
private:
    MutexLock& mutex_;
};

#endif