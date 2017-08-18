#ifndef __THREADPOOL_H_
#define __THREADPOOL_H_

#include <functional>
#include <string>
#include "thread.h"
#include "mutex.h"
#include "condition.h"

class Threadpool : nocopyable
{
public:
    typedef std::function<void()> Task;
    explicit Threadpool(const std::string& name = string("threadpool"));
    ~Threadpool();

    void setMaxQueueSize(int maxSize) { maxQueueSize = maxSize; }
private:

private:
    std::string name_;
    size_t maxQueueSize;
    bool running_;
    mutable MutexLock mutex_;
    Condition notEmpty_;
    Condition notFull_;
    Task threadInitCallback_;
    std::deque<Task> queue_;
};


#endif