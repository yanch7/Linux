#include "threadpool.h"
#include <cassert>

Threadpool::Threadpool(const std::string& name)
    : mutex_(), notEmpty_(mutex_), notFull_(mutex_), name_(name),
        maxQueueSize_(5), running_(false) {}

Threadpool::~Threadpool()
{
    stop();
}

size_t Threadpool::queueSize() const
{
    MutexLockGuard lock(mutex_);
    return queue_.size();
}

bool Threadpool::isFull() const
{
    mutex_.assertLocked();
    return (maxQueueSize_ > 0 && queue_.size() >= maxQueueSize_);
}

void Threadpool::start(int numThreads)
{
    assert(threads_.empty());
    running_ = true;
    threads_.reserve(numThreads);
    for(int i = 0; i < numThreads; i++)
    {
        char threadName[128];
        sprintf(threadName, "%s:%d", name_.c_str(), i);
        threads_.push_back(std::shared_ptr<Thread>(
            new Thread(std::bind(&Threadpool::runInThread, this), threadName)
        ));
    }

    for(int i = 0; i < numThreads; i++) threads_[i]->start();
}

Task Threadpool::take()
{
    MutexLockGuard lock(mutex_);
    while(queue_.empty() && running_)
        notEmpty_.wait();
    Task t;
    if(!queue_.empty())
    {
        t = queue_.front();
        queue_.pop();
        if(maxQueueSize_ > 0) notFull_.notify();
    }

    return t;
}

void Threadpool::run(const Task& t)
{
    if(threads_.empty())
    {
        t();
    }
    else
    {
        MutexLockGuard lock(mutex_);
        while(isFull()) notFull_.wait();
        if(!isFull())
        {
            queue_.push(t);
            notEmpty_.notify();
        }
    }    
}

void Threadpool::stop()
{
    {
        MutexLockGuard lock(mutex_);
        running_ = false;
        notEmpty_.notifyAll();
    }
    
    for_each(threads_.begin(), threads_.end(), std::bind(&Thread::join, std::placeholders::_1));
}

/*线程池中线程的线程函数*/
void Threadpool::runInThread()
{
    if(threadInitCallback_)
    {
        threadInitCallback_();
    }

    while(running_)
    {
        Task task(take());
        if(task)
        {
            task();
        }
    }
}